//===================================================
//
// �}���`�e�N�X�`��2D�I�u�W�F�N�g�̃A�j���[�V���� [Object2DAnimMT.h]
// Author:YUTO YOSHIDA
// 
// << �}���`�e�N�X�`����2D�|���S���̃e�N�X�`��	>>
// << �A�j���[�V�����ɓ��������N���X			>>
// 
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _OBJECT2DANIMMT_H_
#define _OBJECT2DANIMMT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"

//***************************************************
// �}���`�e�N�X�`��2D�A�j���[�V�����N���X�̒�`
//***************************************************
class CObject2DAnimMT : public CObject
{
public:
	CObject2DAnimMT();
	~CObject2DAnimMT();

	static CObject2DAnimMT* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int UV, const int HV);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	void SetAnim(const int nSpeed,const bool bLoop);
	void SetAnim(const int nUV,const int nHV);
	void SetTextureID(const char* pTextureName0, const char* pTextureName1);

	// �Q�b�^�[
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	
	// �Z�b�^�[
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }

private:

	static constexpr int MAX_TEXTURE = 2;	// �e�N�X�`���̐�

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR2 m_Size;						// �傫��
	int m_nPattern;							// �p�[�^��
	int m_nCounter;							// �J�E���^�[
	int m_nUV;								// �e�N�X�`���̕�����
	int m_nHV;								// �e�N�X�`���̕����c
	int m_nSpeed;							// �A�j���[�V�����X�s�[�h
	int m_nTextureIdx[MAX_TEXTURE];			// �e�N�X�`����ID
	bool m_bLoop;							// ���[�v���邩�ǂ���
};

#endif
