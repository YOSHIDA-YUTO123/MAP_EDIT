//===================================================
//
// プレイヤー [player.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "character3D.h"

//===================================================
// プレイヤークラスの定義
//===================================================
class CPlayer : public CCharacter3D
{
public:
	CPlayer();
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	CCharacter3D* Clone(void) const override;

private:
};
#endif
