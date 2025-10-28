//===================================================
//
// �L�����N�^�[3D [character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "character3D.h"
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"
#include "math.h"
#include "model.h"
#include "motion.h"
#include <string>
#include <memory>
#include "transform.h"
#include "CollisionObject3D.h"

//===================================================
// �R���X�g���N�^
//===================================================
CCharacter3D::CCharacter3D() :
	CObject(4),
	m_pCollObject(nullptr),
	m_nNumModel(NULL),
	m_pMotion(nullptr),
	m_nLife(NULL),
	m_state(STATE::STATE_NORMAL),
	m_fSpeed(NULL),
	m_type(TYPE_NONE)
{
}

//===================================================
// �R���X�g���N�^
//===================================================
CCharacter3D::CCharacter3D(const TYPE type)
	: CObject(4),
	m_pCollObject(nullptr),
	m_nNumModel(NULL),
	m_pMotion(nullptr),
	m_nLife(NULL),
	m_state(STATE::STATE_NORMAL),
	m_fSpeed(NULL),
	m_type(type)
{
}

//===================================================
// �f�X�g���N�^
//===================================================
CCharacter3D::~CCharacter3D()
{
}

//===================================================
// ����������
//===================================================
HRESULT CCharacter3D::Init(void)
{
	if (m_pCollObject == nullptr)
	{
		m_pCollObject = CCollisionObject3D::Create(std::make_unique<CTransform>());
	}

	// ����������
	if (FAILED(m_pCollObject->Init()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//===================================================
// �I������
//===================================================
void CCharacter3D::Uninit(void)
{
	// �v�f�������ׂ�
	for (auto &model : m_apModel)
	{
		// ���f���̔j��
		if (model != nullptr)
		{
			// �I������
			model->Uninit();

			delete model;

			model = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		// ���[�V�����̏I������
		m_pMotion->Uninit();
	}

	m_pCollObject = nullptr;

	// �I������
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CCharacter3D::Update(void)
{
	// ��Ԃ̑J��
	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_MOVE:
		break;
	case STATE_ACTION:

		// ��Ԃ̃J�E���^�[�����炷
		m_nCounterState--; 

		// �J�E���^�[��0�ɂȂ�����
		if (m_nCounterState <= 0)
		{
			// ��Ԃ��m�[�}���ɖ߂�
			m_state = STATE_NORMAL;
		}

		break;
	case STATE_DAMAGE:

		// ��Ԃ̃J�E���^�[�����炷
		m_nCounterState--; 

		// �J�E���^�[��0�ɂȂ�����
		if (m_nCounterState <= 0)
		{
			// ��Ԃ��m�[�}���ɖ߂�
			m_state = STATE_NORMAL;
		}

		break;
	default:
		break;
	}
}

//===================================================
// �`�揈��
//===================================================
void CCharacter3D::Draw(void)
{
	if (m_pCollObject != nullptr)
	{
		// �`�揈��
		m_pCollObject->Draw();
	}

	// ���f���̕`��
	for (auto &model : m_apModel)
	{
		if (model != nullptr)
		{
			// �`�揈��
			model->Draw();
		}
	}
}

//===================================================
// �`�揈��(�����x�ݒ�)
//===================================================
void CCharacter3D::Draw(const float fAvl)
{
	if (m_pCollObject != nullptr)
	{
		// �`�揈��
		m_pCollObject->Draw();
	}

	// ���f���̕`��
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// �`�揈��
			m_apModel[nCnt]->Draw(fAvl);
		}
	}
}

//===================================================
// �`�揈��(�}���`�e�N�X�`��)
//===================================================
void CCharacter3D::DrawMT(void)
{
	if (m_pCollObject != nullptr)
	{
		// �`�揈��
		m_pCollObject->Draw();
	}

	// ���f���̕`��
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// �`�揈��
			m_apModel[nCnt]->DrawMultTexture();
		}
	}
}

//===================================================
// ���[�V�����̃��[�h
//===================================================
CMotion* CCharacter3D::LoadMotion(const char* pFileName,const int nNumMotion)
{
	// �ȗ��p
	std::string string = "data/MOTION/";

	// �����������
	string += pFileName;

	// ���[�V�����̃��[�h����
	m_pMotion = CMotion::Load(string.c_str(), m_apModel, &m_nNumModel, nNumMotion, CMotion::LOAD_TEXT);

	return m_pMotion.get();
}

//===================================================
// ���f���̈ʒu�̎擾
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelPos(const int nIdx)
{
	// ���f���̃}�g���b�N�X�̎擾
	D3DXMATRIX mtx = m_apModel[nIdx]->GetMatrixWorld();

	// ���[���h�}�g���b�N�X�̈ʒu�̎擾
	D3DXVECTOR3 modelPos = math::GetPositionFromMatrix(mtx);

	return modelPos;
}

//===================================================
// ���f���̌����̎擾
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelRot(const int nIdx)
{
	// ���f���̈ʒu�̎擾
	D3DXVECTOR3 modelRot = m_apModel[nIdx]->GetRotation();

	return modelRot;
}

//===================================================
// ���f���̑傫���̎擾
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelSize(const int nIdx)
{
	// ���f���̑傫���̎擾
	D3DXVECTOR3 modelSize = m_apModel[nIdx]->GetSize();

	return modelSize;
}

//===================================================
// �L�����N�^�[�̐ݒ菈��
//===================================================
void CCharacter3D::SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 Size)
{
	m_nLife = nLife;
	m_fSpeed = fSpeed;

	// null�������珈�����Ȃ�
	if (m_pCollObject == nullptr) return;

	// ��ԏ��̎擾
	CTransform* pTransform = m_pCollObject->GetTransform();

	if (pTransform != nullptr)
	{
		// ���̎擾
		CTransform::Info info = pTransform->GetInfo();

		info.Size = Size;

		// ���̐ݒ�
		pTransform->SetInfo(info);
	}
}

//===================================================
// ���f���̃}���`�e�N�X�`���̐ݒ�
//===================================================
void CCharacter3D::SetModelMT(const char* pTextureName)
{
	// ���f���̑��������ׂ�
	for (auto itr = m_apModel.begin(); itr != m_apModel.end(); ++itr)
	{
		// �e�N�X�`���̐ݒ�
		(*itr)->SetTextureMT(pTextureName);
	}
}

//===================================================
// �L�����N�^�[�̃q�b�g����
//===================================================
bool CCharacter3D::Hit(int nDamage)
{
	// �̗͂����炷
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{
		m_state = STATE_DEATH;

		return false;
	}

	return true;
}

//===================================================
// ���[�V�����̍X�V����
//===================================================
void CCharacter3D::UpdateMotion(void)
{
	if (m_pMotion != nullptr)
	{
		// ���[�V�����̍X�V����
		m_pMotion->Update(&m_apModel[0], m_nNumModel);
	}
}

//===================================================
// ��Ԃ̐ݒ菈��
//===================================================
void CCharacter3D::SetState(const STATE state, const int nTime)
{
	m_state = state;
	m_nCounterState = nTime;
}
