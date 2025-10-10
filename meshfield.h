//================================================
//
// メッシュフィールド [meshfield.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//************************************************
// インクルードファイル
//************************************************
#include"object.h"
#include<vector>
#include <memory> 

//************************************************
// 前方宣言
//************************************************
class CMeshField;
class CColliderSphere;

//************************************************
// メッシュフィールドクラスの定義
//************************************************
class CMeshField : public CObject
{
public:
	static constexpr int MAX_SEGMENT_H = 150; // 横の分割数
	static constexpr int MAX_SEGMENT_V = 150; // 縦の分割数

	CMeshField(int nPriority = 2);
	~CMeshField();

	static CMeshField* Create(const D3DXVECTOR3 pos,const int nSegH,const int nSegV,const D3DXVECTOR2 Size, const D3DXVECTOR3 rot = Const::VEC3_NULL);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(const D3DXVECTOR3 pos, float* pOutHeight); // 地面との当たり判定

	void UpdateNor(void); // 法線の再設定

	// ゲッター
	D3DXVECTOR3 GetNor(void) const { return m_Nor; }
	D3DXVECTOR3 GetVtxPos(const int nIdx);
	D3DXVECTOR2 GetSize(void) const { return m_Size; }

	int GetSegH(void) const { return m_nSegH; }
	int GetSegV(void) const { return m_nSegV; }

	// セッター
	void SetTextureID(const char* pTextureName);
	void SetSegment(const int nSegH, const int nSegV);
	void SetSegment(void);
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }

	void SetVtxHeight(CColliderSphere* pSphere, const float AddHeightValue);
	void SetVtxColor(CColliderSphere* pSphere, const D3DXCOLOR col);
	void Save(void); // セーブ処理
	void Load(void); // ロード処理
private:
	static constexpr int MAX_VERTEX = (MAX_SEGMENT_H * MAX_SEGMENT_V); // 最大の頂点数

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR2 m_Size;						// 大きさ
	D3DXVECTOR3 m_Nor;						// 法線
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	float m_fSaveHeight[MAX_VERTEX];		// 高さの保持
	int m_nSegH;							// 横の分割数
	int m_nSegV;							// 縦の分割数
	int m_nNumVtx;							// 頂点の総数
	int m_nNumPolygon;						// ポリゴン数
	int m_nNumIdx;							// インデックス数
	int m_nTextureIdx;						// テクスチャのID
};

#endif