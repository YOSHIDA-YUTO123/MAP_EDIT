//===================================================
//
// マップのオブジェクトのリスト [MapObjectList.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MAPOBJECTLIST_H_
#define _MAPOBJECTLIST_H_

//***************************************************
// インクルードファイル
//***************************************************
#include <vector>
#include "modelManager.h"

//***************************************************
// マップのオブジェクトのリスト
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
	std::vector<CModelManager::ModelInfo> m_pMapObjectList; // マップのオブジェクトのリスト
};

#endif
