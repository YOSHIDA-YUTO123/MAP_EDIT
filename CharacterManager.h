
//===================================================
//
// キャラクター [character.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include<memory>
#include<vector>
#include <string>
#include "character3D.h"

//***************************************************
// 前方宣言
//***************************************************
class CCharacter3D;
class CMotion;
class CModel;

//***************************************************
// キャラクターのマネージャークラスの定義
//***************************************************
class CCharacterManager
{
public:
	~CCharacterManager();
	static void Create(void);
	static CCharacterManager* GetInstance(void) { return m_pInstance.get(); }
	void SetCharacter(CCharacter3D* pCaracther);
	CCharacter3D* CreateCharacter(CCharacter3D::TYPE type);
private:
	void Init(void);

	CCharacterManager();
	static std::unique_ptr<CCharacterManager> m_pInstance;	// 自分のインスタンス
	CCharacter3D* m_pCharacter[CCharacter3D::TYPE_MAX];
};

#endif