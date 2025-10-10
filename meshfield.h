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
class CColliderSphere;

//************************************************
// ���b�V���t�B�[���h�N���X�̒�`
//************************************************
class CMeshField : public CObject
{
public:
	static constexpr int MAX_SEGMENT_H = 150; // ���̕�����
	static constexpr int MAX_SEGMENT_V = 150; // �c�̕�����

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
	D3DXVECTOR2 GetSize(void) const { return m_Size; }

	int GetSegH(void) const { return m_nSegH; }
	int GetSegV(void) const { return m_nSegV; }

	// �Z�b�^�[
	void SetTextureID(const char* pTextureName);
	void SetSegment(const int nSegH, const int nSegV);
	void SetSegment(void);
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }

	void SetVtxHeight(CColliderSphere* pSphere, const float AddHeightValue);
	void SetVtxColor(CColliderSphere* pSphere, const D3DXCOLOR col);
	void Save(void); // �Z�[�u����
	void Load(void); // ���[�h����
private:
	static constexpr int MAX_VERTEX = (MAX_SEGMENT_H * MAX_SEGMENT_V); // �ő�̒��_��

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR2 m_Size;						// �傫��
	D3DXVECTOR3 m_Nor;						// �@��
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	float m_fSaveHeight[MAX_VERTEX];		// �����̕ێ�
	int m_nSegH;							// ���̕�����
	int m_nSegV;							// �c�̕�����
	int m_nNumVtx;							// ���_�̑���
	int m_nNumPolygon;						// �|���S����
	int m_nNumIdx;							// �C���f�b�N�X��
	int m_nTextureIdx;						// �e�N�X�`����ID
};

#endif