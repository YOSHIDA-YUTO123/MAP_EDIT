//==============================================
//
// プレイヤーの基底クラス [player.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "object.h"

//**********************************************
// 前方宣言
//**********************************************
class CCharacter3D;
class CTransform;

//**********************************************
// プレイヤークラスの定義
//**********************************************
class CPlayer : public CObject
{
public:

	// プレイヤーの種類
	typedef enum
	{
		TYPE_BASE = 0, // 基底クラス
		TYPE_GAME,	   // ゲーム
		TYPE_MAX
	}TYPE;

	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0, // ニュートラル
		MOTIONTYPE_MOVE,		// 移動
		MOTIONTYPE_ACTION,		// アクション
		MOTIONTYPE_JUMP,		// ジャンプ
		MOTIONTYPE_LANDING,		// 着地
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	CPlayer();
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CPlayer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pMotionTxt, const int nNumMotion);

	CCharacter3D* GetCharacter(void) { return m_pCharacter; }
private:
	CCharacter3D* m_pCharacter;		// キャラクタークラスへのポインタ

	CTransform* m_pTransformRef;	// 参照用空間情報
	D3DXVECTOR3 m_move;				// 移動量
	float m_fMove;					// 移動スピード
	float m_fRotDest;				// 目的の向き
};
#endif
