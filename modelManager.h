//==============================================
//
// ���f���}�l�[�W���[ [modelManager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// ���d�C���N���[�h�h�~
//**********************************************
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include"main.h"
#include<vector>

//**********************************************
// ���f���̃}�l�[�W���[�N���X�̒�`
//**********************************************
class CModelManager
{
public:
	// ���f���̏��̒�`
	struct ModelInfo
	{
		D3DXVECTOR3 Size;				  // �傫��
		LPD3DXMESH pMesh;				  // ���b�V��(���_���)�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;			  // �}�e���A���ւ̃|�C���^
		DWORD dwNumMat;					  // �}�e���A���̐�
		std::vector<int> nTextureIdx;	  // �e�N�X�`���̃C���f�b�N�X
		char filepath[Const::MAX_WORD];   // �t�@�C���p�X
	};

	CModelManager();
	~CModelManager();

	int Register(const char* pFilename);
	ModelInfo GetModelInfo(const int nIdx);
	std::vector<ModelInfo>& GetList(void) { return m_apModelInfo; }
	HRESULT Load(void);
	void UnLoad(void);
private:
	const char* JSON_FILE = "data/model_list.json"; // �g�p����jsonFIle

	void SaveList(void);
	void SetSize(ModelInfo* pModelInfo);
	void SetMaterial(ModelInfo* pModelInfo);

	std::vector<ModelInfo> m_apModelInfo; // ���f���̏��
	static int m_nNumAll;				  // �e�N�X�`���̔ԍ�
};

#endif
