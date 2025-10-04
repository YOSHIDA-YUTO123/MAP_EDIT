//===================================================
//
// マップオブジェクトのマネージャー [MapObjectManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MAPOBJECTMANAGER_H_
#define _MAPOBJECTMANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "main.h"
#include <vector>
#include <memory>
#include <string>

//***************************************************
// 前方宣言
//***************************************************
class CMapObject;
class CImGuiManager;
class CEditMapObject;

//***************************************************
// マップオブジェクトのリストクラス
//***************************************************
class CMapObjectManager
{
public:
	~CMapObjectManager();

	/// <summary>
	/// 自分のインスタンスの取得
	/// </summary>
	/// <returns>自分のインスタンス</returns>
	static CMapObjectManager* GetInstance(void);

	/// <summary>
	/// マップオブジェクトの生成処理
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="pModelFileName"></param>
	/// <returns>マップオブジェクトのインスタンス</returns>
	CMapObject* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName);

	/// <summary>
	/// マップオブジェクトのリストの取得
	/// </summary>
	/// <returns>vector型のオブジェクトのリスト</returns>
	std::vector<CMapObject*>& GetList(void) { return m_pMapObjectList; }

	HRESULT Init(void);
	void Update(void);

	/// <summary>
	/// ドラッグ移動の状態の取得
	/// </summary>
	/// <returns>判定結果</returns>
	bool GetDragMoveState(void) const { return m_bDragMoveXZ; }
private:
	CMapObjectManager();

	void GetModelPath(void);
	void SetModelPathList(void);
	void SetSelectObjAlv(void);
	void SetInspector(void);
	void UpdateEditMapObj(void);
	bool CollisionMouse(void);
	HRESULT Register(void);
	void UpdateSelectObj(void);
	void Save(void);
	void Load(void);
	void Erase(CMapObject* pObj);
	void SetFilePath(void);
	void SetCamerafocus(const bool bMode);

	static std::unique_ptr<CMapObjectManager> m_pInstance; // 自分のインスタンス
	static std::vector<CMapObject*> m_pMapObjectList;	   // マップオブジェクトのリスト
	static std::vector<std::string> m_aModelPath;		   // モデルのパスのリスト
	CEditMapObject* m_pEditMapObj;						   // 編集用マップオブジェクト
	CMapObject* m_pSelect;								   // 選択中のマップオブジェクト
	int m_nType;										   // モデルの種類
	bool m_bDragMoveXZ;									   // ドラッグ移動をXZ平面にするか
};

#endif
