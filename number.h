//================================================
//
// �����𑀍삷�鏈�� [number.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"object.h"

//************************************************
// �i���o�[�N���X�̒�`
//************************************************
class CNumber
{
public:

	CNumber();
	~CNumber();

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }
	void SetPos(const D3DXVECTOR3 pos);
	void SetUV(const int nNum);
	void SetColor(const D3DXCOLOR col);
	void SetTextureID(const char* pFileName);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR2 m_Size;						// �傫��
	int m_nTextureIdx;						// �e�N�X�`���̃C���f�b�N�X
};
#endif