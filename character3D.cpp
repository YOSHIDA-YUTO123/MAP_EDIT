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
#include "CharacterManager.h"

using namespace math;	// ���O���math���g�p
using namespace Const;	// ���O���Const���g�p
using namespace std;	// ���O���std���g�p

//===================================================
// �R���X�g���N�^
//===================================================
CCharacter3D::CCharacter3D() : CObject(4)
{
	m_Size = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = NULL;
	m_nHitStopTime = NULL;
}

//===================================================
// �R���X�g���N�^
//===================================================
CCharacter3D::CCharacter3D(const TYPE type) : CObject(4)
{
	m_type = type;
	m_Size = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = NULL;
	m_nHitStopTime = NULL;
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
	return S_OK;
}

//===================================================
// �I������
//===================================================
void CCharacter3D::Uninit(void)
{
	for (int nCnt = 0; nCnt < (int)m_apModel.size(); nCnt++)
	{
		// ���f���̔j��
		if (m_apModel[nCnt] != nullptr)
		{
			// �I������
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];

			m_apModel[nCnt] = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		// ���[�V�����̏I������
		m_pMotion->Uninit();
	}

	// �������g�̔j��
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
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// �`�揈��
			m_apModel[nCnt]->Draw();
		}
	}
}

//===================================================
// �`�揈��(�����x�ݒ�)
//===================================================
void CCharacter3D::Draw(const float fAvl)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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

	// �L�����N�^�[�̃}�l�[�W���[�̎擾
	auto pCharacterManager = CCharacterManager::GetInstance();

	if (pCharacterManager == nullptr) return nullptr;

	// �����̎�ނ̎擾
	TYPE type = GetType();

	// ���[�V�����̃��[�h����
	m_pMotion = CMotion::Load(string.c_str(), m_apModel, &m_nNumModel, nNumMotion, CMotion::LOAD_TEXT);

	// ���[�V�����̐ݒ�
	pCharacterManager->SetMotion(m_pMotion.get(), type);

	int nCnt = 0;

	// ���f���̗v�f�����ׂ�
	for (auto itr = m_apModel.begin(); itr != m_apModel.end(); ++itr)
	{
		// null�Ȃ珈�����Ȃ�
		if ((*itr) == nullptr) continue;

		// ���[�V�����̐ݒ�
		pCharacterManager->SetModel((*itr), type, m_nNumModel,nCnt);

		nCnt++;
	}

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
	D3DXVECTOR3 modelPos = GetPositionFromMatrix(mtx);

	return modelPos;
}

//===================================================
// ���f���̌����̎擾
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelRot(const int nIdx)
{
	// ���f���̈ʒu�̎擾
	D3DXVECTOR3 modelRot = m_apModel[nIdx]->GetRotaition();

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
// �e�̃}�g���b�N�X�̎擾
//===================================================
D3DXMATRIX CCharacter3D::GetParent(const int nIdx) const
{
	D3DXMATRIX OutMtx; // �e�̃}�g���b�N�X

	// �e�̃}�g���b�N�X�̎擾
	OutMtx = m_apModel[nIdx]->GetMatrixWorld();

	return OutMtx;
}

//===================================================
// �L�����N�^�[�̐ݒ菈��
//===================================================
void CCharacter3D::SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 Size)
{
	m_nLife = nLife;
	m_fSpeed = fSpeed;
	m_Size = Size;
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
// ���̃R�s�[
//===================================================
void CCharacter3D::Copy(CCharacter3D* pCharacter)
{
	pCharacter->m_nNumModel = m_nNumModel;

	// ���f���̗v�f���𒲂ׂ�
	for (int nCnt = 0;nCnt < m_nNumModel;nCnt++)
	{
		// null�������珈�����Ȃ�
		if (m_apModel[nCnt] == nullptr) continue;

		// ���f���̖��O�̎擾
		const char* pModelName = m_apModel[nCnt]->GetModelName();

		// ���f���̐���
		auto pModel = CModel::Create(pModelName);

		// �e�̃C���f�b�N�X�̎擾
		int nParentIdx = m_apModel[nCnt]->GetParentID();

		// ���̃R�s�[
		m_apModel[nCnt]->Copy(pModel);

		if (nParentIdx != -1)
		{
			// �e�̃��f���̐ݒ�
			pModel->SetParent(pCharacter->m_apModel[nParentIdx], nParentIdx);
		}
		else
		{
			// �e�̃��f���̐ݒ�
			pModel->SetParent(nullptr, nParentIdx);
		}
		// ���f���̓ǂݍ���
		pCharacter->m_apModel.push_back(pModel);
	}

	m_pMotion->GetInfo(pCharacter->m_pMotion.get());
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
// �����Ă��邩�ǂ���
//===================================================
bool CCharacter3D::GetAlive(void)
{
	// ����ł���Ȃ�
	if (m_nLife <= 0)
	{
		return false;
	}

	return true;
}

//===================================================
// �q�b�g�X�g�b�v�̐ݒ菈��
//===================================================
bool CCharacter3D::HitStop(void)
{
	// �J�E���^�[�����炷
	m_nHitStopTime--;

	// 0�ȉ���������
	if (m_nHitStopTime <= 0)
	{
		// �q�b�g�X�g�b�v���Ă��Ȃ�
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
// �L�����N�^�[�̐ݒ�
//===================================================
void CCharacter3D::SetCharacter(void)
{
	// ���[�V�����̐���
	m_pMotion = make_unique<CMotion>();

	// �L�����N�^�[�̃}�l�[�W���[�̎擾
	auto pCharacterManager = CCharacterManager::GetInstance();

	if (pCharacterManager == nullptr) return;

	// ���[�V�����̎擾
	pCharacterManager->GetMotion(m_pMotion.get(), m_type);

	// ���f���̃T�C�Y�̎擾
	m_nNumModel = pCharacterManager->GetModelSize(m_type);

	// ���f���̃T�C�Y�̊m��
	m_apModel.resize(m_nNumModel);

	// ���f���̑��������ׂ�
	for (int nCnt = 0;nCnt < m_nNumModel;nCnt++)
	{
		// �e�̃C���f�b�N�X
		int nParentIdx = -1;

		// ���f���̎擾
		pCharacterManager->GetModel(&m_apModel[nCnt],m_type,nCnt,&nParentIdx);

		if (nParentIdx != -1)
		{
			// �e�̃��f���̐ݒ�
			m_apModel[nCnt]->SetParent(m_apModel[nParentIdx], nParentIdx);
		}
		else
		{
			// �e�̃��f���̐ݒ�
			m_apModel[nCnt]->SetParent(nullptr, nParentIdx);
		}
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
