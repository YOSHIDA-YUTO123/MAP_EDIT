//===================================================
//
// �}�b�v�̃I�u�W�F�N�g�̃��X�g [MapObjectList.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MAPOBJECTLIST_H_
#define _MAPOBJECTLIST_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include <vector>
#include "modelManager.h"

//***************************************************
// �}�b�v�̃I�u�W�F�N�g�̃��X�g
//***************************************************
class CMapObjectList
{
public:
	CMapObjectList();
	~CMapObjectList();

	static CMapObjectList* Create(void) { return new CMapObjectList; }
	std::vector<CModelManager::ModelInfo>& GetList(void) { return m_pMapObjectList; }
	void AddList(CModelManager::ModelInfo modelInfo);
	void Register(void);

private:
	std::vector<CModelManager::ModelInfo> m_pMapObjectList; // �}�b�v�̃I�u�W�F�N�g�̃��X�g
};

#endif
