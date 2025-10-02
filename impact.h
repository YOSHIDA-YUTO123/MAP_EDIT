//================================================
//
// ���b�V���C���p�N�g [impact.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _IMPACT_H_
#define _IMPACT_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"object.h"

//************************************************
// ���b�V���T�[�N��(����)�N���X�̒�`
//************************************************
class CMeshCircle : public CObject
{
public:

	CMeshCircle(const int nPriority = 4);
	~CMeshCircle();

	static CMeshCircle* Create(const D3DXCOLOR col, const D3DXVECTOR3 pos, const float fInRadius, const float fOutRadius, const int nSegH = 32);
	void SetCircle(const float fHeight, const float fSpeed, const int nLife, const bool bField, const D3DXVECTOR3 rot = Const::VEC3_NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTextureID(const char* pTextureName);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXCOLOR m_Incol, m_Outcol;			// �F
	float m_fHeight;						// ����
	float m_fSpeed;							// ����
	float m_fInRadius;						// �����̔��a
	float m_fOutRadius;						// �O���̔��a
	float m_fDecAlv;						// ���l�̌����X�s�[�h
	int m_nSegH;							// ���̕�����
	int m_nSegV;							// �c�̕�����
	int m_nNumVtx;							// ���_�̑���
	int m_nNumPolygon;						// �|���S����
	int m_nNumIdx;							// �C���f�b�N�X��
	int m_nTextureIdx;						// �e�N�X�`����ID
	int m_nLife;							// ����
	bool m_bFiledCollision;					// �n�ʂ̍����ɍ��킹�邩�ǂ���
};
#endif