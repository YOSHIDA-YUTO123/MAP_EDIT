//===================================================
//
// �I�u�W�F�N�g2D�̕`�揈�� [object2D.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include"object.h"

//***************************************************
// �I�u�W�F�N�g2D�N���X�̒�`
//***************************************************
class CObject2D : public CObject
{
public:

	CObject2D(int nPriority = 7);
	virtual ~CObject2D();

	static CObject2D* Create(const float fWidth, const float fHeight, const D3DXVECTOR3 pos,const D3DXVECTOR3 rot = Const::VEC3_NULL);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR2 GetSize(void) const { return m_Size; }

	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotaition(const D3DXVECTOR3 rot) { m_rot = rot; }

	void SetVtx(const D3DXCOLOR col);
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }
	void SetSize(const float fWidth, const float fHeight);
	void SetSize(const float leftWidth,const float rightWdth,const float topHeight,const float buttom);

	void UpdateVertex(void);
	
	void SetColor(const D3DXCOLOR col);
	void SetTextureID(const char* pTextureName = NULL);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR2 m_Size;						// �傫��
	float m_fAngle;							// �p�x
	float m_Length;							// ����
	int m_nTextureIdx;						// �e�N�X�`���̃C���f�b�N�X
};

#endif