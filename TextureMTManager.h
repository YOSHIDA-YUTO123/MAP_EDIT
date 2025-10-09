//===================================================
//
// �����_�[�^�[�Q�b�g�p�e�N�X�`�����Ǘ�����}�l�[�W���[ [TextureMTManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _TEXTUREMTMANAGER_H_
#define _TEXTUREMTMANAGER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "main.h"
#include <string>
#include <vector>

//***************************************************
// �����_�[�^�[�Q�b�g�p�e�N�X�`���̃N���X�̒�`
//***************************************************
class CTextureMTManager
{
public:
	// �e�N�X�`���̏��
	struct Info
	{
		LPDIRECT3DTEXTURE9 pTextureMT;	// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
		LPDIRECT3DSURFACE9 pRenderMT;	// �����_�����O�^�[�Q�b�g�p�C���^�[�t�F�[�X
		LPDIRECT3DSURFACE9 pZBuffMT;	// �����_�����O�^�[�Q�b�g�pZ�o�b�t�@
		D3DVIEWPORT9 viewport;			// �e�N�X�`�������_�����O�p�r���[�|�[�g
		std::string aName;				// ���O
	};

	CTextureMTManager();
	~CTextureMTManager();

	void Uninit(void);
	std::vector<Info>& GetList(void) { return m_aInfo; }
	LPDIRECT3DTEXTURE9 GetAddress(const char* pName);
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU, const char* pName); // �����_�����O�^�[�Q�b�g�̕ύX
private:
	void CreateRender(const char* pName);

	std::vector<Info> m_aInfo;			// ���
};
#endif