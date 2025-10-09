//===================================================
//
// モデルのアイコンの作成 [IconModel.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ICONMODEL_H_
#define _ICONMODEL_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// モデルのアイコンの作成
//***************************************************
class CIconModel : public CObject
{
public:
	~CIconModel();

	// 生成処理
	static CIconModel* Create(void) { return new CIconModel; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	CIconModel();
	void SetDraw(void);

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
};

#endif