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

	static CMapObject* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetALv(const float fAlv) { m_fAlv = fAlv; }
	void Register(const char* pModelFileName);
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void UpdateMove(void);
	void SetMove(const float fMove) { m_fMove = fMove; }

	const char* GetPath(void) const { return m_aModelPath.c_str(); }
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }

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
	static constexpr float MOVE_VALUE = 5.0f; // 移動量

	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	std::string m_aModelPath;	// モデルのパス
	float m_fAlv;				// アルファ値
	float m_fMove;				// 移動量
	int m_nModelIdx;			// モデルのインデックス
	bool m_bMouseUp;			// マウスを離したかどうか
};

#endif
