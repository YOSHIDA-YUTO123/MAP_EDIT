//===================================================
//
// 3D�|���S��(�}���`�e�N�X�`��) [Object3DMT.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _OBJECT3DMT_H_
#define _OBJECT3DMT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"

//***************************************************
// �I�u�W�F�N�g3DMT�N���X�̒�`
//***************************************************
class CObject3DMT : public CObject
{
public:
	CObject3DMT();
	~CObject3DMT();

	static CObject3DMT* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 Size);
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetVtx(const D3DXCOLOR col);

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotaition(void) const { return m_rot; }

	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotaition(const D3DXVECTOR3 rot) { m_rot = rot; }

	void SetTextureID(const char* pTexture0, const char* pTexture1);
private:

	static constexpr int MAX_TEXTURE = 2;	// �e�N�X�`���̐�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_Size;						// �傫��
	int m_nTextureIdx[MAX_TEXTURE];			// �e�N�X�`���̃C���f�b�N�X
};

#endif
