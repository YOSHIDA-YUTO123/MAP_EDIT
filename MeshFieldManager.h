//===================================================
//
// メッシュフィールドのマネージャークラスの定義 [MeshFieldManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESHFIELDMANAGER_H_
#define _MESHFIELDMANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include <memory>

//***************************************************
// 前方宣言
//***************************************************
class CMeshCylinder;
class CColliderSphere;
class CMeshField;
class CTransform;

//***************************************************
// メッシュフィールドのマネージャークラスの定義
//***************************************************
class CMeshFieldManager
{
public:
	~CMeshFieldManager();

	static CMeshFieldManager* GetInstance(void);

	HRESULT Init(void);
	void Update(void);
	void SetShow(const bool bEnable);
private:
	static constexpr float ADD_HEIGHT = 5.0f;		// 高さの加算値
	static constexpr float BRUSH_RADIUS = 100.0f;	// ブラシの半径

	CMeshFieldManager();
	void SetBrush(void);
	void SetVtx(CMeshField *pMeshField);

	static std::unique_ptr<CMeshFieldManager> m_pInstance; // 自分のインスタンス
	std::unique_ptr<CColliderSphere> m_pBrushSphere;	   // ブラシの円の判定
	std::unique_ptr<CTransform> m_pTransform;			   // 空間情報のポインタ
	CMeshCylinder* m_pMeshCylinder;						   // シリンダーの取得
	D3DXCOLOR m_col;									   // 色
	D3DXVECTOR3 m_BrushPos;								   // ブラシの位置
	float m_fAddHeightValue;							   // 高さの加算値
	float m_fBrushRadius;								   // ブラシの半径
	bool m_bShow;										   // 表示するかどうか
};

#endif
