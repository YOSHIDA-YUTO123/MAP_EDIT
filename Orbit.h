//================================================
//
// �O�� [orbit.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _ORBIT_H_
#define _ORBIT_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"object.h"

//************************************************
// �O�Ղ̃N���X�̒�`
//************************************************
class CMeshOrbit : public CObject
{
public:
	CMeshOrbit();
	~CMeshOrbit();

	static CMeshOrbit* Create(const D3DXVECTOR3 Top, const D3DXVECTOR3 Bottom, const int nSegH, const D3DXCOLOR col);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(const D3DXVECTOR3 Top, const D3DXVECTOR3 Bottom);
	void SetTextureID(const char* pTextureName);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_Top;						// ��̈ʒu
	D3DXVECTOR3 m_Bottom;					// ���̈ʒu
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXCOLOR m_col;						// �F
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	int m_nSegH;							// ���̕�����
	int m_nSegV;							// �c�̕�����
	int m_nNumVtx;							// ���_�̑���
	int m_nNumPolygon;						// �|���S����
	int m_nNumIdx;							// �C���f�b�N�X��
	int m_nTextureIdx;						// �e�N�X�`����ID
};

#endif
