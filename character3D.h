//===================================================
//
// キャラクターの構成 [character.h]
// Author:YUTO YOSHIDA
// 
// << 3Dキャラクターの見た目、モーションを作る基底クラス >>
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER3D_H_
#define _CHARACTER3D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include"object.h"
#include<memory>
#include<vector>

//***************************************************
// 前方宣言
//***************************************************
class CShadowS;
class CRotation;
class CMotion;
class CModel;
class CCollisionObject3D;

//***************************************************
// キャラクター3Dクラスの定義
//***************************************************
class CCharacter3D : public CObject
{
public:

	// 状態の種類
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_MOVE,
		STATE_ACTION,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	// キャラクターの種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_MAX
	}TYPE;

	CCharacter3D();
	CCharacter3D(const TYPE type);
	virtual ~CCharacter3D();

	static CCharacter3D* Create(void) { return new CCharacter3D; }

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	/// <summary>
	/// クローンの作成処理
	/// </summary>
	/// <returns>キャラクターのポインタ</returns>
	void Draw(const float fAvl);
	void DrawMT(void);
	
	void SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 Size);
	void SetModelMT(const char* pTextureName);

	void SetState(const STATE state,const int nTime);
	bool Hit(int nDamage);		// ヒット時の処理
	void UpdateMotion(void);

	CCollisionObject3D* GetCollisionObject(void) { return m_pCollObject; }

	TYPE GetType(void) const { return m_type; }
	CMotion* LoadMotion(const char* pFileName, const int nNumMotion);

	STATE GetState(void) { return m_state; }
	CMotion* GetMotion(void) { return m_pMotion.get(); } // モーションの取得
	float GetSpeed(void) { return m_fSpeed; }
	int GetLife(void) const { return m_nLife; }
	void SetType(const TYPE type) { m_type = type; }

	D3DXVECTOR3 GetModelPos(const int nIdx);
	D3DXVECTOR3 GetModelRot(const int nIdx);
	D3DXVECTOR3 GetModelSize(const int nIdx);

private:
	CCollisionObject3D* m_pCollObject;		// 当たり判定するオブジェクト
	std::unique_ptr<CMotion> m_pMotion;		// モーションのクラスへのポインタ
	std::vector<CModel*> m_apModel;			// モデルクラスのポインタ
	STATE m_state;							// 状態
	TYPE m_type;							// 種類
	float m_fSpeed;							// 足の速さ
	int m_nNumModel;						// モデルの最大数
	int m_nCounterState;					// 状態のカウンター
	int m_nLife;							// 寿命
};
#endif