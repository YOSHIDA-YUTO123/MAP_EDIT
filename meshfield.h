//================================================
//
// ���b�V���t�B�[���h [meshfield.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"object.h"
#include<vector>
#include <memory> 

//************************************************
// �O���錾
//************************************************
class CMeshField;

//************************************************
// ���b�V���t�B�[���h�N���X�̒�`
//************************************************
class CMeshField : public CObject
{
public:
	CMeshField(int nPriority = 2);
	~CMeshField();

	static CMeshField* Create(const D3DXVECTOR3 pos,const int nSegH,const int nSegV,const D3DXVECTOR2 Size, const D3DXVECTOR3 rot = Const::VEC3_NULL);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(const D3DXVECTOR3 pos, float* pOutHeight); // �n�ʂƂ̓����蔻��

	void UpdateNor(void); // �@���̍Đݒ�

	// �Q�b�^�[
	D3DXVECTOR3 GetNor(void) const { return m_Nor; }
	D3DXVECTOR3 GetVtxPos(const int nIdx);

	// �Z�b�^�[
	void SetTextureID(const char* pTextureName);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR2 m_Size;						// �傫��
	D3DXVECTOR3 m_Nor;						// �@��
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	int m_nSegH;							// ���̕�����
	int m_nSegV;							// �c�̕�����
	int m_nNumVtx;							// ���_�̑���
	int m_nNumPolygon;						// �|���S����
	int m_nNumIdx;							// �C���f�b�N�X��
	int m_nTextureIdx;						// �e�N�X�`����ID
};

#endif