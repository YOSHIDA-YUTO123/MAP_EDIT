//===================================================
//
// ���f�� [model.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "main.h"
#include <string>

//***************************************************
// ���f���N���X�̒�`
//***************************************************
class CModel
{
public:
	CModel();
	~CModel();
	static CModel* Create(const char* pModelName);

	HRESULT Init(const char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(const float fAlv);
	void DrawMultTexture(void);
	void DrawShadow(void);
	void SetTextureMT(const char* pTextureName);
	bool CollisionMouse(void);

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetOffPos(void) const { return m_offpos; }
	D3DXVECTOR3 GetOffRot(void) const { return m_offrot; }

	D3DXVECTOR3 GetSize(void) const { return m_Size; }

	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotaition(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetOffPos(const D3DXVECTOR3 pos) { m_offpos = pos; }
	void SetOffRot(const D3DXVECTOR3 rot) { m_offrot = rot; }

	void SetParent(CModel* pModel,const int nIdx);
	D3DXMATRIX GetMatrixWorld(void) const { return m_mtxWorld; }

	const char* GetModelName(void) { return m_aModelName.c_str(); }
	void SetModelName(const char* pModelName) { m_aModelName = pModelName; }
	void Copy(CModel* pModel); 
	int GetParentID(void) const { return m_nParentIdx; }
private:

	D3DXVECTOR3 m_Size;			// �傫��
	D3DXVECTOR3 m_offpos;			// ��̈ʒu
	D3DXVECTOR3 m_offrot;			// ��̌���
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_rot;				// ����
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	CModel* m_pParent;				// �e���f���ւ̃|�C���^
	std::string m_aModelName;		// ���f���̖��O
	int m_nParentIdx;				// �e�̃C���f�b�N�X
	int m_nModelIdx;				// ���f���̃C���f�b�N�X
	int m_nTextureMTIdx;			// �}���`�e�N�X�`���̃C���f�b�N�X
};
#endif