//===================================================
//
// �ҏW���̃I�u�W�F�N�g�̕\���p�N���X [EditMapObject.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "EditMapObject.h"
#include "modelManager.h"
#include "manager.h"
#include "input.h"
#include "imgui.h"
#include "math.h"
#include "renderer.h"
#include "imguimaneger.h"
#include "MapObjectManager.h"
#include "camera.h"

//===================================================
// �R���X�g���N�^
//===================================================
CEditMapObject::CEditMapObject() : CObjectX(7)
{
	m_fMove = NULL;
	m_bMouseUp = true;
	m_bShow = false;
}

//===================================================
// �f�X�g���N�^
//===================================================
CEditMapObject::~CEditMapObject()
{
}

//===================================================
// ��������
//===================================================
CEditMapObject* CEditMapObject::Create(void)
{
	CEditMapObject* pEditMapObject = new CEditMapObject;

	// ����������
	if (FAILED(pEditMapObject->Init()))
	{
		pEditMapObject->Uninit();
		pEditMapObject = nullptr;
		return nullptr;
	}

	return pEditMapObject;
}

//===================================================
// ����������
//===================================================
HRESULT CEditMapObject::Init(void)
{
	// ���f���̃}�l�[�W���[�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = {};

	if (pModelManager != nullptr)
	{
		// �ŏ��̃��f���̎擾
		modelInfo = pModelManager->GetModelInfo(0);

		// �ŏ��̃��f���̃t�@�C���p�X
		m_aModelName = modelInfo.filepath;

		// ���f���̃��[�h
		LoadModel(m_aModelName.c_str());
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CEditMapObject::Uninit(void)
{
	// �I������
	CObjectX::Uninit();
}

//===================================================
// �X�V����
//===================================================
void CEditMapObject::Update(void)
{
}

//===================================================
// �`�揈��
//===================================================
void CEditMapObject::Draw(void)
{
	// ������Ȃ�
	if (m_bShow)
	{
		// �`�揈��
		CObjectX::Draw(D3DXCOLOR(0.4f,1.0f,1.0f,0.5f));
	}
}

//===================================================
// �}�E�X�̓����蔻��
//===================================================
bool CEditMapObject::CollisionMouse(float* pDistance)
{
	BOOL hit = false;
	DWORD faceIndex;
	(*pDistance) = FLT_MAX;
	FLOAT baryU, baryV; // ���������|���S���̈ʒu

	// ���f���̏��̎擾
	const CModelManager::ModelInfo modelInfo = CObjectX::GetModelInfo();

	// ���C�̌��_
	D3DXVECTOR3 rayOrigin, rayDir;

	// ���C�̎擾
	math::GetMouseRay(&rayOrigin, &rayDir);

	// ���f���̃��[���h�s��
	D3DXMATRIX matWorld = CObjectX::GetMatrix();

	// �t�s��
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

	// ���C�����[�J����Ԃɕϊ�
	D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &matInvWorld);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &matInvWorld);
	D3DXVec3Normalize(&rayDir, &rayDir);

	// ���C�ƃ|���S���̓����蔻��
	D3DXIntersect(
		modelInfo.pMesh,
		&rayOrigin,
		&rayDir,
		&hit,
		&faceIndex,
		&baryU,
		&baryV,
		pDistance,
		nullptr,
		nullptr);

	if (hit)
	{
		return true;
	}

	return false;
}

//===================================================
// �ҏW���[�h�̍X�V����
//===================================================
void CEditMapObject::UpdateEdit(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ���_�̑�������Ă���Ƃ��ɏ������Ȃ�
	if (pKeyboard->GetPress(DIK_LALT)) return;

	// �ʒu�̎擾
	D3DXVECTOR3 pos = CObjectX::GetPosition();

	// ����
	float fDistance = 0.0f;

	// �}�E�X�̎擾
	CInputMouse* pMouse = CManager::GetInputMouse();

	// �}�E�X�Ƃ̓����蔻��
	if (CollisionMouse(&fDistance))
	{
		// ���N���b�N�����Ă�����
		if (pMouse->OnMousePress(CInputMouse::LEFT))
		{
			// �}�E�X�������Ă���
			m_bMouseUp = false;
		}
		else
		{
			// �}�E�X�������Ă��Ȃ�
			m_bMouseUp = true;
		}
	}
	// ���N���b�N�����Ă��Ȃ��Ȃ�
	else if (!pMouse->OnMousePress(CInputMouse::LEFT))
	{
		// �}�E�X�𗣂���
		m_bMouseUp = true;
	}

	// �}�E�X�𗣂��Ă��Ȃ��Ȃ�
	if (!m_bMouseUp)
	{
		// �ʒu�ƑO��̈ʒu�̎擾
		D3DXVECTOR2 MousePos = pMouse->GetVelocity();
		D3DXVECTOR2 MousePosOld = pMouse->GetOldVelocity();

		// �ړ��ʂ̐ݒ�
		D3DXVECTOR2 MouseMove = MousePos - MousePosOld;

		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXMATRIX view;
		pDevice->GetTransform(D3DTS_VIEW, &view);

		// �J�����̉E�x�N�g���Ə�x�N�g���𒊏o
		D3DXVECTOR3 camRight(view._11, view._21, view._31);
		D3DXVECTOR3 camUp(view._12, view._22, view._32);

		D3DXVECTOR3 move;

		if (CMapObjectManager::GetInstance()->GetDragMoveState())
		{
			// XZ���ʂł̈ړ�
			camUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ������͖�����
			D3DXVECTOR3 forward = D3DXVECTOR3(view._13, 0.0f, view._33);
			D3DXVec3Normalize(&forward, &forward);

			move = camRight * MouseMove.x + forward * -MouseMove.y;
		}
		else
		{
			// �ʏ�̃J������ړ�
			move = camRight * MouseMove.x + camUp * -MouseMove.y;
		}

		// �ʒu�̍X�V
		pos += move;
	}

	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �J�������擾�o���Ȃ������珈�����Ȃ�
	if (pCamera == nullptr) return;

	// �J�����̊p�x�̎擾
	float fCameraAngle = pCamera->GetRotaition().y;

	if (pKeyboard->GetPress(DIK_A))
	{
		if (pKeyboard->GetPress(DIK_W))
		{
			pos.x += sinf(fCameraAngle - D3DX_PI * 0.25f) * m_fMove;
			pos.z += cosf(fCameraAngle - D3DX_PI * 0.25f) * m_fMove;
		}
		else if (pKeyboard->GetPress(DIK_S))
		{
			pos.x += sinf(fCameraAngle - D3DX_PI * 0.75f) * m_fMove;
			pos.z += cosf(fCameraAngle - D3DX_PI * 0.75f) * m_fMove;
		}
		else
		{
			pos.x += sinf(fCameraAngle - D3DX_PI * 0.5f) * m_fMove;
			pos.z += cosf(fCameraAngle - D3DX_PI * 0.5f) * m_fMove;
		}
	}
	else if (pKeyboard->GetPress(DIK_D))
	{
		if (pKeyboard->GetPress(DIK_W))
		{
			pos.x += sinf(fCameraAngle + D3DX_PI * 0.25f) * m_fMove;
			pos.z += cosf(fCameraAngle + D3DX_PI * 0.25f) * m_fMove;
		}
		else if (pKeyboard->GetPress(DIK_S))
		{
			pos.x += sinf(fCameraAngle + D3DX_PI * 0.75f) * m_fMove;
			pos.z += cosf(fCameraAngle + D3DX_PI * 0.75f) * m_fMove;
		}
		else
		{
			pos.x += sinf(fCameraAngle + D3DX_PI * 0.5f) * m_fMove;
			pos.z += cosf(fCameraAngle + D3DX_PI * 0.5f) * m_fMove;
		}
	}
	else if (pKeyboard->GetPress(DIK_W))
	{
		pos.x += sinf(fCameraAngle) * m_fMove;
		pos.z += cosf(fCameraAngle) * m_fMove;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{
		pos.x += sinf(fCameraAngle + D3DX_PI) * m_fMove;
		pos.z += cosf(fCameraAngle + D3DX_PI) * m_fMove;
	}

	CObjectX::SetPosition(pos);
}
