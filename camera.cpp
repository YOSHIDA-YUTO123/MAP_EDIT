//===================================================
//
// �J���� [camera.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "camera.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "math.h"
#include "meshfield.h"
#include "cylinder.h"
#include "input.h"
#include "pause.h"
#include "Collider.h"

using namespace Const; // ���O���Const���g�p

namespace
{
	constexpr float MAX_VIEW_TOP = 2.90f;	// �J�����̐���(��)
	constexpr float MAX_VIEW_BOTTOM = 0.0f; // �J�����̐���(��)
	constexpr float HEIGHT_OFFSET = 20.0f;	// �J�����̍����̃I�t�Z�b�g
	constexpr float ROCKON_HEIGHT = 200.0f;	// ���b�N�I���̎��̃J�����̍���
	constexpr float FOV_BASE = 45.0f;		// ����p�x�[�X
	constexpr float FOV_COLLISION = 65.0f;	// ����p�Ǎۂ̎�
	constexpr float WHEEL_ZOOM = 100.0f;	// wheel�̃Y�[��
}

//===================================================
// �R���X�g���N�^
//===================================================
CCamera::CCamera()
{
	m_posVOld = VEC3_NULL;
	m_fFov = FOV_BASE;
	m_bCollision = false;
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProjection);
	m_posR = VEC3_NULL;
	m_posV = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_posRDest = VEC3_NULL;
	m_posVDest = VEC3_NULL;
	m_vecU = VEC3_NULL;
	m_fDistance = NULL;
}

//===================================================
// �f�X�g���N�^
//===================================================
CCamera::~CCamera()
{
}

//===================================================
// ����������
//===================================================
HRESULT CCamera::Init(void)
{
	m_posR = VEC3_NULL;

	m_posV = VEC3_NULL;		// ���_

	m_rot = VEC3_NULL;	// �p�x

	//m_posV = D3DXVECTOR3(0.0f, 250.0f, -350.0f);		// ���_

	//m_rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	// �p�x

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CCamera::Uninit(void)
{
}

//===================================================
// �X�V����
//===================================================
void CCamera::Update(void)
{
	// �}�E�X�̎��_�ړ�
	MouseView();

	// �p�x�̐��K��
	NormalizeRot(&m_rot.x);
	NormalizeRot(&m_rot.y);
	NormalizeRot(&m_rot.z);
}

//===================================================
// �ݒ�,�`�揈��
//===================================================
void CCamera::SetCamera(void)
{
	CRenderer* pRenderer = CManager::GetRenderer();

	// �����_���[��null�������珈�����Ȃ�
	if (pRenderer == nullptr) return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(m_fFov),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		100000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//===================================================
// �J�����̐ݒ菈��
//===================================================
void CCamera::SetCamera(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot)
{
	// �ݒ菈��
	m_posV = posV;
	m_posR = posR;
	m_rot = rot;

	float fRotX = m_posV.x - m_posR.x;
	float fRotY = m_posV.y - m_posR.y;
	float fRotZ = m_posV.z - m_posR.z;

	// ���������߂�
	m_fDistance = sqrtf((fRotX * fRotX) + (fRotY * fRotY) + (fRotZ * fRotZ));

	//�J�����̒����_�������W��ݒ�
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// �J�����̎��_�̏��
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// �}�E�X�z�C�[���̏���
//===================================================
void CCamera::MouseWheel(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();

	// �}�E�X�z�C�[���̉�]�ʂ��擾
	int nWheel = pMouse->GetMouseWeel();

	// �z�C�[�����񂵂ĂȂ�������
	if (nWheel == 0)
	{
		return;
	}

	if (nWheel < 0)
	{
		m_fDistance += WHEEL_ZOOM;
	}
	else if (nWheel > 0)
	{
		m_fDistance -= WHEEL_ZOOM;
	}
	
	// �͈͂̐���
	m_fDistance = Clamp(m_fDistance, 5.0f, 999999.0f);

	// ���_�̍X�V����
	UpdatePositionV();
}

//===================================================
// �}�E�X�̎��_�ړ�����
//===================================================
void CCamera::MouseView(void)
{
	// �}�E�X�̎擾
	CInputMouse* pMouse = CManager::GetInputMouse();

	// �}�E�X�z�C�[��
	CCamera::MouseWheel();

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetPress(DIK_LALT) == false) return;

	D3DXVECTOR2 Move = pMouse->GetVelocity();
	D3DXVECTOR2 MoveOld = pMouse->GetOldVelocity();

	D3DXVECTOR2 fAngle = Move - MoveOld;

	CCamera::PadView();

	// ��������
	if (pMouse->OnMousePress(0) && pMouse->OnMousePress(1))
	{
		D3DXVECTOR3 eye = m_posR - m_posV;
		D3DXVec3Normalize(&eye, &eye);

		D3DXVECTOR3 cross,cameraUp;
		D3DXVec3Cross(&cross, &m_vecU, &eye);
		
		D3DXVec3Cross(&cameraUp, &eye, &cross);
		D3DXVECTOR3 move = cross * (-fAngle.x);

		// �c�ړ�
		move += cameraUp * fAngle.y;

		// �J�����̈ʒu�X�V
		m_posV += move;
		m_posR += move;
	}
	else if (pMouse->OnMousePress(1))
	{
		//��]�ʂ��X�V
		m_rot.y += fAngle.x * 0.01f;
		m_rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (m_rot.x > MAX_VIEW_TOP)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}
		else if (m_rot.x < MAX_VIEW_BOTTOM)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}

		//�J�������W���X�V
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	}
	else if (pMouse->OnMousePress(0))
	{
		//��]�ʂ��X�V
		m_rot.y += fAngle.x * 0.01f;
		m_rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (m_rot.x > MAX_VIEW_TOP)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}
		else if (m_rot.x < MAX_VIEW_BOTTOM)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}

		// �J�����̎��_�̏��
		m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
		m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	}
}

//===================================================
// �p�b�h�̎��_�ړ�����
//===================================================
void CCamera::PadView(void)
{
	// �R���g���[���[�̎擾
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// �X�e�B�b�N
	XINPUT_STATE* pStick = pJoypad->GetJoyStickAngle();

	if (pJoypad->GetJoyStickR() == true)
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag >= DeadZone)
		{
			float NormalizeX = RStickAngleX / fMag;
			float NormalizeY = RStickAngleY / fMag;

			float fAngle = fMag * 0.000003f;

			// ��]�ʂ��v�Z
			float fAngleX = NormalizeX * 0.5f * fAngle;
			float fAngleY = NormalizeY * 0.5f * fAngle;

			m_rot.y += fAngleX;
			m_rot.x -= fAngleY;

			// ��]�ʂ𐧌�
			if (m_rot.x > MAX_VIEW_TOP)
			{
				m_rot.x += fAngleY;
			}
			else if (m_rot.x < MAX_VIEW_BOTTOM)
			{
				m_rot.x += fAngleY;
			}

			// ���_�̍X�V����
			UpdatePositionV();
		}
	}
}

//===================================================
// ���_�̍X�V����
//===================================================
void CCamera::UpdatePositionV(void)
{
	// �O��̈ʒu�̐ݒ�
	m_posVOld = m_posV;

	// �J�����̎��_�̍X�V
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// �����_�̍X�V����
//===================================================
void CCamera::UpdatePositionR(void)
{
	// �J�����̒����_�̍X�V
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// �ʒu��ړI�̈ʒu�ɋ߂Â���
//===================================================
void CCamera::LerpPos(const D3DXVECTOR3 posRDest, const D3DXVECTOR3 posVDest, const float fCoef)
{
	// �O��̈ʒu�̐ݒ�
	m_posVOld = m_posV;

	// �ړI�̒����_�̐ݒ�
	m_posRDest.x = posRDest.x;
	m_posRDest.y = posRDest.y;
	m_posRDest.z = posRDest.z;

	// �ړI�̎��_�̐ݒ�
	m_posVDest.x = posVDest.x;
	m_posVDest.y = posVDest.y;
	m_posVDest.z = posVDest.z;

	// �ړI�̒����_�ɋ߂Â���
	m_posR.x += (m_posRDest.x - m_posR.x) * fCoef;
	m_posR.y += (m_posRDest.y - m_posR.y) * fCoef;
	m_posR.z += (m_posRDest.z - m_posR.z) * fCoef;

	// �ړI�̎��_�ɋ߂Â���
	m_posV.x += (m_posVDest.x - m_posV.x) * fCoef;
	m_posV.y += (m_posVDest.y - m_posV.y) * fCoef;
	m_posV.z += (m_posVDest.z - m_posV.z) * fCoef;
}