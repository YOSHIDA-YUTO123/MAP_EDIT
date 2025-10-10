//================================================
//
// ���b�V���V�����_�[ [cylinder.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _CYLINDER_H_
#define _CYLINDER_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"object.h"

//************************************************
// ���b�V���t�B�[���h�N���X�̒�`
//************************************************
class CMeshCylinder : public CObject
{
public:

	// �V�����_�[�̎�ނ̐ݒ�
	typedef enum
	{
		TYPE_WALL = 0, // ��
		TYPE_VIEW,	   // ��������p
		TYPE_MAX
	}TYPE;

	CMeshCylinder(int nPriority = 5);
	~CMeshCylinder();

	static CMeshCylinder* Create(const D3DXVECTOR3 pos, const int nSegH, const int nSegV,const float fRadius, const float fHeight, const D3DXVECTOR3 rot = Const::VEC3_NULL);
	void Set(const TYPE type) { m_Type = type; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCylinder(const int nSegH, const int nSegV, const float fRadius, const float fHeight);
	bool Collision(D3DXVECTOR3* pPos);
	void SetTextureID(const char* pTextureName);
	void Show(const bool bEnable) { m_bShow = bEnable; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXCOLOR m_col;						// �F
	TYPE m_Type;							// ���
	float m_fRadius;						// ���a
	float m_fHeight;						// ����
	int m_nSegH;							// ���̕�����
	int m_nSegV;							// �c�̕�����
	int m_nNumVtx;							// ���_�̑���
	int m_nNumPolygon;						// �|���S����
	int m_nNumIdx;							// �C���f�b�N�X��
	int m_nTextureIdx;						// �e�N�X�`����ID
	bool m_bShow;							// �\�����邩�ǂ���
};
#endif