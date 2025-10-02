//===================================================
//
// �r���{�[�h�A�j���[�V�����̕`�� [billboardAnim.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _BILLBOARDANIM_H_
#define _BILLBOARDANIM_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include"object.h"

//***************************************************
// �r���{�[�h(�A�j���[�V����)�N���X�̒�`
//***************************************************
class CBillboardAnimation : public CObject
{
public:
	CBillboardAnimation(int nPriority = 4);
	virtual ~CBillboardAnimation();

	static CBillboardAnimation* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size,const int nSpeed,const int U,const int V,const bool bLoop);
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR2 GetSize(void) const { return m_Size; }

	void SetAnim(const int nAnimSpeed, const int U, const int V, const bool bLoop);
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(const D3DXVECTOR2 size) { m_Size = size; }

	void SetTextureVtx(const D3DXVECTOR2 tex, const D3DXVECTOR2 offtex);
	void SetColor(const D3DXCOLOR col);
	void SetTextureID(const char* pTextureName);
	void UpdateVertexPos(const D3DXVECTOR3 pos);

private:
	D3DXVECTOR3 m_pos;						// �ʒu�̃N���X�̃|�C���^
	D3DXVECTOR2 m_Size;						// �傫��
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�ւ̃|�C���^
	float m_fPosTexU;						// �e�N�X�`�����WUV
	float m_fPosTexH;						// �e�N�X�`�����WHV
	int m_nAnimSpeed;						// �A�j���[�V�����̑���
	int m_nPatternAnim;						// �A�j���[�V�����̃p�^�[��
	int m_nCounterAnim;						// �A�j���[�V�����̃J�E���^�[
	int m_nH, m_nV;							// �e�N�X�`����UV�̕�����
	int m_nTextureIdx;						// �e�N�X�`���̃C���f�b�N�X
	bool m_bLoop;							// ���[�v���邩�ǂ���
};

#endif