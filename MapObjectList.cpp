//===================================================
//
// �}�b�v�̃I�u�W�F�N�g�̃��X�g [MapObjectList.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "MapObjectList.h"
#include "MapObject.h"
#include "manager.h"
#include "modelManager.h"

//===================================================
// �R���X�g���N�^
//===================================================
CMapObjectList::CMapObjectList()
{
}

//===================================================
// �f�X�g���N�^
//===================================================
CMapObjectList::~CMapObjectList()
{
}

//===================================================
// �o�^���鏈��
//===================================================
void CMapObjectList::AddList(CModelManager::ModelInfo modelInfo)
{
	// �v�f�𒲂ׂ�
	for (auto& list : m_pMapObjectList)
	{
		// �o�^�ς݂�������
		if (strcmp(list.filepath ,modelInfo.filepath) == 0)
		{
			return;
		}
	}

	m_pMapObjectList.push_back(modelInfo);
}

//===================================================
// ����������
//===================================================
void CMapObjectList::Register(void)
{
	// ���f���}�l�[�W���[�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pModelManager == nullptr) return;
	
	// �v�f�𒲂ׂ�
	for (auto& list : pModelManager->GetList())
	{
		std::string filepath = list.filepath;

		// �}�b�v�̃I�u�W�F�N�g�t�@�C���ɓ����Ă�����
		if (filepath.find("data/MODEL/MapObjectFile") != std::string::npos)
		{
			// ���X�g�ɒǉ�
			AddList(list);
		}
	}
}
