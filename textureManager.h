//==============================================
//
// �e�N�X�`���}�l�[�W���[ [textureManager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// ���d�C���N���[�h�h�~
//**********************************************
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include"main.h"
#include "object.h"
#include<vector>

//**********************************************
// �e�N�X�`���̃}�l�[�W���[�N���X�̒�`
//**********************************************
class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	int Register(const char* pFilename);
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
	HRESULT Load(void);
	void UnLoad(void);
private:

	// �e�N�X�`���̏��̒�`
	struct TextureInfo
	{
		LPDIRECT3DTEXTURE9 pTexture;			 // �e�N�X�`���ւ̃|�C���^
		char filepath[Const::MAX_WORD];				 // �t�@�C���p�X
	}; 

	std::vector<TextureInfo> m_apTextureInfo;	 // �e�N�X�`���̏��
	static int m_nNumAll;						 // �e�N�X�`���̔ԍ�
};

#endif