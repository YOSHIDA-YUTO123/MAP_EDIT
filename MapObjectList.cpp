//===================================================
//
// マップのオブジェクトのリスト [MapObjectList.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "MapObjectList.h"
#include "MapObject.h"
#include "manager.h"
#include "modelManager.h"

//===================================================
// コンストラクタ
//===================================================
CMapObjectList::CMapObjectList()
{
}

//===================================================
// デストラクタ
//===================================================
CMapObjectList::~CMapObjectList()
{
}

//===================================================
// 登録する処理
//===================================================
void CMapObjectList::AddList(CModelManager::ModelInfo modelInfo)
{
	// 要素を調べる
	for (auto& list : m_pMapObjectList)
	{
		// 登録済みだったら
		if (strcmp(list.filepath ,modelInfo.filepath) == 0)
		{
			return;
		}
	}

	m_pMapObjectList.push_back(modelInfo);
}

//===================================================
// 初期化処理
//===================================================
void CMapObjectList::Register(void)
{
	// モデルマネージャーの取得
	CModelManager* pModelManager = CManager::GetModel();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr) return;
	
	// 要素を調べる
	for (auto& list : pModelManager->GetList())
	{
		std::string filepath = list.filepath;

		// マップのオブジェクトファイルに入っていたら
		if (filepath.find("data/MODEL/MapObjectFile") != std::string::npos)
		{
			// リストに追加
			AddList(list);
		}
	}
}
