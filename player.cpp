//==============================================
//
// �v���C���[�̏���������N���X [player.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "player.h"
#include "input.h"
#include "manager.h"
#include "character3D.h"
#include "transform.h"
#include "CollisionObject3D.h"
#include "camera.h"
#include "math.h"
#include "motion.h"
#include "Collider.h"
#include "meshfield.h"

//==============================================
// �R���X�g���N�^
//==============================================
CPlayer::CPlayer() :
    CObject(4),
    m_pCharacter(nullptr),
    m_move(),
    m_pTransformRef(nullptr),
	m_fMove(NULL),
	m_fRotDest(NULL)
{
}

//==============================================
// �f�X�g���N�^
//==============================================
CPlayer::~CPlayer()
{
}

//==============================================
// ��������
//==============================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pMotionTxt, const int nNumMotion)
{
    CPlayer* pInstance = new CPlayer;

    // ����������
    if (FAILED(pInstance->Init()))
    {
        pInstance->Uninit();
        pInstance = nullptr;
        return nullptr;;
    }

    // ���[�V�����̃��[�h����
    pInstance->m_pCharacter->LoadMotion(pMotionTxt, nNumMotion);

    // �����蔻��̎擾
    CCollisionObject3D *pCollObj = pInstance->m_pCharacter->GetCollisionObject();

    // ��ԏ��̎擾
    CTransform *pTransform = pCollObj->GetTransform();

    if (pTransform != nullptr)
    {
        // ���̎擾
        CTransform::Info info = pTransform->GetInfo();

        info.pos = pos;
        info.rot = rot;

        pTransform->SetInfo(info);
    }

    return pInstance;
}

//==============================================
// ����������
//==============================================
HRESULT CPlayer::Init(void)
{
    // �L�����N�^�[�̐���
    m_pCharacter = CCharacter3D::Create();

    if (m_pCharacter != nullptr)
    {
        if (FAILED(m_pCharacter->Init()))
        {
            return E_FAIL;
        }
        m_pCharacter->SetType(CCharacter3D::TYPE_PLAYER);
    }

	// �����蔻��I�u�W�F�N�g�̎擾
	CCollisionObject3D* pCollisionObject = m_pCharacter->GetCollisionObject();

	// ��ԏ��̎擾
	CTransform* pTransform = pCollisionObject->GetTransform();

	// ��ԏ��̐ݒ�
	m_pTransformRef = pTransform;

	m_pCharacter->SetCharacter(0, 1.0f, D3DXVECTOR3(20.0f, 100.0f, 20.0f));

	// �R���C�_�[�̒ǉ�
	pCollisionObject->AddCollider(CColliderAABB::Create(), "player");
	
	m_fMove = 1.0f;

    return S_OK;
}

//==============================================
// �I������
//==============================================
void CPlayer::Uninit(void)
{
    m_pCharacter = nullptr;

    // �I������
    CObject::Release();
}

//==============================================
// �X�V����
//==============================================
void CPlayer::Update(void)
{
    // �L�����N�^�[����������Ă��Ȃ��Ȃ�
    if (m_pCharacter == nullptr) return;
    
    // ���̎擾
    CTransform::Info transform = m_pTransformRef->GetInfo();

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �p�b�h�̎擾
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �J�����̌����̎擾
	float fCameraRotY = pCamera->GetRotaition().y;

	bool bMove = false;

	if (pKeyboard->GetPress(DIK_A))
	{
		//�v���C���[�̈ړ�(��)
		if (pKeyboard->GetPress(DIK_W) == true)
		{
			m_move.x += sinf(fCameraRotY - D3DX_PI * 0.25f) * m_fMove;
			m_move.z += cosf(fCameraRotY - D3DX_PI * 0.25f) * m_fMove;

			m_fRotDest = fCameraRotY + D3DX_PI * 0.75f;

			bMove = true;
		}
		//�v���C���[�̈ړ�(��)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(fCameraRotY - D3DX_PI * 0.75f) * m_fMove;
			m_move.z += cosf(fCameraRotY - D3DX_PI * 0.75f) * m_fMove;

			m_fRotDest = fCameraRotY + D3DX_PI * 0.25f;

			bMove = true;
		}
		// �v���C���[�̈ړ�(��)
		else
		{
			m_move.z += sinf(fCameraRotY) * m_fMove;
			m_move.x -= cosf(fCameraRotY) * m_fMove;

			m_fRotDest = fCameraRotY + D3DX_PI * 0.5f;

			bMove = true;
		}
	}
	//�v���C���[�̈ړ�(�E)
	else if (pKeyboard->GetPress(DIK_D))
	{
		//�v���C���[�̈ړ�(��)
		if (pKeyboard->GetPress(DIK_W))
		{
			m_move.x += sinf(fCameraRotY + D3DX_PI * 0.25f) * m_fMove;
			m_move.z += cosf(fCameraRotY + D3DX_PI * 0.25f) * m_fMove;

			m_fRotDest = fCameraRotY - D3DX_PI * 0.75f;

			bMove = true;
		}
		//�v���C���[�̈ړ�(��)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(fCameraRotY + D3DX_PI * 0.75f) * m_fMove;
			m_move.z += cosf(fCameraRotY + D3DX_PI * 0.75f) * m_fMove;

			m_fRotDest = fCameraRotY - D3DX_PI * 0.25f;

			bMove = true;
		}
		// �v���C���[�̈ړ�(�E)
		else
		{
			m_move.z -= sinf(fCameraRotY) * m_fMove;
			m_move.x += cosf(fCameraRotY) * m_fMove;

			m_fRotDest = fCameraRotY - D3DX_PI * 0.5f;

			bMove = true;
		}
	}
	//�v���C���[�̈ړ�(��)
	else if (pKeyboard->GetPress(DIK_W) == true)
	{
		m_move.x += sinf(fCameraRotY) * m_fMove;
		m_move.z += cosf(fCameraRotY) * m_fMove;

		m_fRotDest = fCameraRotY + D3DX_PI;

		bMove = true;
	}
	//�v���C���[�̈ړ�(��)
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		m_move.x -= sinf(fCameraRotY) * m_fMove;
		m_move.z -= cosf(fCameraRotY) * m_fMove;

		m_fRotDest = fCameraRotY;

		bMove = true;
	}

	// ���[�V�����̎擾
	CMotion* pMotion = m_pCharacter->GetMotion();

	if (bMove)
	{
		pMotion->SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if(pMotion->GetType() == MOTIONTYPE_MOVE)
	{
		pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 10);
	}

    // �ړ��ʂ̌���
    m_move.x += (0.0f - m_move.x) * 0.25f;
    m_move.z += (0.0f - m_move.z) * 0.25f;

    // �ړ��ʂ̍X�V
    transform.pos += m_move;

	// ���b�V���t�B�[���h�̐���
	CMeshField* pMeshField = CManager::GetMeshField();

	float fHeight = 0.0f;

	if (pMeshField->Collision(transform.pos, &fHeight))
	{
		transform.pos.y = fHeight;
	}

	m_move.y += -Const::MAX_GRABITY;

	// �p�x�̐��K��
	NormalizeDiffRot(m_fRotDest - transform.rot.y, &transform.rot.y);

	// �ړI�̌����ɕ��
	transform.rot.y += (m_fRotDest - transform.rot.y) * 0.1f;

    // ���[�V�����̍X�V����
    m_pCharacter->UpdateMotion();

    m_pTransformRef->SetInfo(transform);
}

//==============================================
// �`�揈��
//==============================================
void CPlayer::Draw(void)
{
    // �L�����N�^�[����������Ă��Ȃ��Ȃ�
    if (m_pCharacter == nullptr) return;

    // �`�揈��
    m_pCharacter->Draw();
}