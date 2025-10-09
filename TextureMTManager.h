//===================================================
//
// レンダーターゲット用テクスチャを管理するマネージャー [TextureMTManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTUREMTMANAGER_H_
#define _TEXTUREMTMANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "main.h"
#include <string>
#include <vector>

//***************************************************
// レンダーターゲット用テクスチャのクラスの定義
//***************************************************
class CTextureMTManager
{
public:
	// テクスチャの情報
	struct Info
	{
		LPDIRECT3DTEXTURE9 pTextureMT;	// レンダリングターゲット用テクスチャ
		LPDIRECT3DSURFACE9 pRenderMT;	// レンダリングターゲット用インターフェース
		LPDIRECT3DSURFACE9 pZBuffMT;	// レンダリングターゲット用Zバッファ
		D3DVIEWPORT9 viewport;			// テクスチャレンダリング用ビューポート
		std::string aName;				// 名前
	};

	CTextureMTManager();
	~CTextureMTManager();

	void Uninit(void);
	std::vector<Info>& GetList(void) { return m_aInfo; }
	LPDIRECT3DTEXTURE9 GetAddress(const char* pName);
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU, const char* pName); // レンダリングターゲットの変更
private:
	void CreateRender(const char* pName);

	std::vector<Info> m_aInfo;			// 情報
};
#endif