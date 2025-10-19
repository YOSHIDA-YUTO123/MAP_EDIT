//===================================================
//
// �L�����N�^�[�}�l�[�W���[ [CharacterManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "CharacterManager.h"
#include "character3D.h"
#include "motion.h"
#include "player.h"

using namespace std; // ���O���std�̎g�p

//***************************************************
// �ÓI�����o�ϐ��錾
//***************************************************
unique_ptr<CCharacterManager> CCharacterManager::m_pInstance = nullptr; // �����̃C���X�^���X

//===================================================
// �R���X�g���N�^
//===================================================
CCharacterManager::CCharacterManager()
{
	for (auto& list : m_pCharacter)
	{
		list = nullptr;
	}
}

//===================================================
// �f�X�g���N�^
//===================================================
CCharacterManager::~CCharacterManager()
{
}

//===================================================
// ��������
//===================================================
void CCharacterManager::Create(void)
{
	if (m_pInstance == nullptr)
	{
		// �����̐���
		m_pInstance.reset(new CCharacterManager);
		m_pInstance->Init();
	}
}

//===================================================
// �L�����N�^�[�̐ݒ�
//===================================================
void CCharacterManager::SetCharacter(CCharacter3D* pCaracther)
{
	// ��ނ̎擾
	auto Type = pCaracther->GetType();

	if (m_pCharacter[Type] == nullptr)
	{
		// ��\��
		pCaracther->SetOffShow();
		m_pCharacter[Type] = pCaracther;
	}
}

//===================================================
// �L�����N�^�[�̐���
//===================================================
CCharacter3D* CCharacterManager::CreateCharacter(CCharacter3D::TYPE type)
{
	CCharacter3D* templateChar = m_pCharacter[type];

	if (m_pCharacter[type] != nullptr)
	{
		return templateChar->Clone();
	}

	return nullptr;
}

//===================================================
// ����������
//===================================================
void CCharacterManager::Init(void)
{
	auto templateChar = new CPlayer;

	templateChar->LoadMotion("motion.txt", 5);

	SetCharacter(templateChar);

	CCharacter3D* p = CreateCharacter(CCharacter3D::TYPE_PLAYER);
}


////===================================================
//// ���[�V�����̏��̐ݒ�
////===================================================
//void CCharacterManager::SetMotion(CMotion* pMotion, const CCharacter3D::TYPE type)
//{
//	// ���[�V�����̏��̐ݒ�
//	pMotion->GetInfo(&m_aConfig[type].Motion);
//	m_aConfig[type].type = type;
//}
//
////===================================================
//// ���f���̐ݒ�
////===================================================
//void CCharacterManager::SetModel(CModel* pModel, const CCharacter3D::TYPE type, const int nNumModel,const int nIdx)
//{
//	CModel modelWk = {};
//
//	// ���f���̏��̃R�s�[
//	pModel->Copy(&modelWk);
//
//	// �T�C�Y�̊m��
//	m_aConfig[type].aModelList.resize(nNumModel);
//
//	// �e���f����ID
//	int nParentIdx = pModel->GetParentID();
//
//	if (nParentIdx != -1)
//	{
//		// �e���f���̐ݒ�
//		modelWk.SetParent(&m_aConfig[type].aModelList[nParentIdx], nParentIdx);
//	}
//	else
//	{
//		// �e���f���̐ݒ�
//		modelWk.SetParent(nullptr, nParentIdx);
//	}
//
//	// ���f���̖��O�̎擾
//	const char* pModelName = pModel->GetModelName();
//
//	// ���f���̖��O�̎擾
//	modelWk.SetModelName(pModelName);
//
//	// ���̐ݒ�
//	m_aConfig[type].aModelList[nIdx] = modelWk;
//
//	m_aConfig[type].type = type;
//}
//
////===================================================
//// ���[�V�����̃��X�g�̐ݒ�
////===================================================
//void CCharacterManager::GetMotion(CMotion* pMotion, const CCharacter3D::TYPE type)
//{
//	// ���̎擾
//	m_aConfig[type].Motion.GetInfo(pMotion);
//}
//
////===================================================
//// ���f���̎擾
////===================================================
//void CCharacterManager::GetModel(CModel** pModel, const CCharacter3D::TYPE type, const int nIdx,int *pParentIndx)
//{
//	// ���f���̖��O�̎擾
//	const char* pModelName = m_aConfig[type].aModelList[nIdx].GetModelName();
//
//	// ���f���̐���
//	(*pModel) = CModel::Create(pModelName);
//
//	// ���f���̏��̃R�s�[
//	m_aConfig[type].aModelList[nIdx].Copy((*pModel));
//
//	// �e�̃C���f�b�N�X�̎擾
//	int nParentIdx = m_aConfig[type].aModelList[nIdx].GetParentID();
//
//	*pParentIndx = nParentIdx;
//}
