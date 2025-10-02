//===================================================
//
// マップに配置するオブジェクトのクラス [MapObject.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include <string>

//***************************************************
// マップのオブジェクトのクラスの定義
//***************************************************
class CMapObject : public CObject
{
public:
	CMapObject();
	~CMapObject();

	static CMapObject* Create(const D3DXVECTOR3 pos, const char* pModelFileName);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetALv(const float fAlv) { m_fAlv = fAlv; }
	void Register(const char* pModelFileName);

	/// <summary>
	/// マウスとモデルの当たり判定
	/// </summary>
	/// <param name="pDistance"></param>
	/// <returns>判定結果</returns>
	bool CollisionMouse(float *pDistance);

	/// <summary>
	/// 情報の設定(Imguiを使用する)
	/// </summary>
	void SetInfo(void);

	void SetMouseDrag(void);
private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	float m_fAlv;				// アルファ値
	int m_nModelIdx;			// モデルのインデックス
	bool m_bMouseUp;			// マウスを離したかどうか
};

#endif
