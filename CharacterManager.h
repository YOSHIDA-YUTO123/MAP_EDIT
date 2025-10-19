
//===================================================
//
// �L�����N�^�[ [character.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include<memory>
#include<vector>
#include <string>
#include "character3D.h"

//***************************************************
// �O���錾
//***************************************************
class CCharacter3D;
class CMotion;
class CModel;

//***************************************************
// �L�����N�^�[�̃}�l�[�W���[�N���X�̒�`
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
	static std::unique_ptr<CCharacterManager> m_pInstance;	// �����̃C���X�^���X
	CCharacter3D* m_pCharacter[CCharacter3D::TYPE_MAX];
};

#endif