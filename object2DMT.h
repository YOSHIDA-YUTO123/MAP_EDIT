//================================================
//
// 2D�I�u�W�F�N�g(�}���`�e�N�X�`��)�̕`�� [object2DMT.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _OBJECT2DMT_H_
#define _OBJECT2DMT_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "object.h"

//************************************************
// �I�u�W�F�N�g2D(�}���`�e�N�X�`��)�N���X�̒�`
//************************************************
class CObject2DMT : public CObject
{
public:

	CObject2DMT();
	virtual ~CObject2DMT();

	static CObject2DMT* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	
	// �Q�b�^�[
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotaition(void) const { return m_rot; }
	D3DXVECTOR2 GetSize(void) const { return m_Size; }
	D3DXCOLOR GetColor(void) const { return m_col; }

	// �Z�b�^�[
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotaition(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }
	void SetColor(const D3DXCOLOR col) { m_col = col; }
	void SetTextureID(const char* pTexture1, const char* pTexture2);
	void SetTexture(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex1, const D3DXVECTOR2 tex2, const D3DXVECTOR2 tex3, const int nCnt);
private:

	static constexpr int MAX_TEXTURE = 2; // �e�N�X�`���̍ő吔

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer; // ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;					  // �ʒu
	D3DXVECTOR3 m_rot;					  // ����
	D3DXCOLOR m_col;					  // �F
	D3DXVECTOR2 m_Size;					  // �傫��
	int m_nTextureIdx[MAX_TEXTURE];		  // �e�N�X�`����ID
};

#endif