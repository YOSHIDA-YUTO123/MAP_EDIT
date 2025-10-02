
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
#include "motion.h"

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
	void SetMotion(CMotion* pMotion, const CCharacter3D::TYPE type);
	void SetModel(CModel* pModel, const CCharacter3D::TYPE type, const int nNumModel, const int nIdx);
	void GetMotion(CMotion* pMotion, const CCharacter3D::TYPE type);
	void GetModel(CModel** pModel, const CCharacter3D::TYPE type, const int nIdx, int* pParentIndx);
	int GetModelSize(const CCharacter3D::TYPE type) { return static_cast<int>(m_aConfig[type].aModelList.size()); }
private:

	struct Config
	{
		std::vector<CModel> aModelList;
		CMotion Motion;
		CCharacter3D::TYPE type;
	};

	CCharacterManager();
	static std::unique_ptr<CCharacterManager> m_pInstance;	// �����̃C���X�^���X
	Config m_aConfig[CCharacter3D::TYPE_MAX];				// �L�����N�^�[�̍\�����

	//std::vector<CModel> m_aModelList;						// �ۑ��p�̃��f�����
};

#endif