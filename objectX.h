//===================================================
//
// �I�u�W�F�N�gX�t�@�C�� [objectX.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"
#include "modelManager.h"

//***************************************************
// �I�u�W�F�N�gX�t�@�C���̃N���X�̒�`
//***************************************************
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 2);
	virtual ~CObjectX();

	static CObjectX* Create(const D3DXVECTOR3 pos, const char* pModelName,const D3DXVECTOR3 rot);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	
	void Draw(const float Diffuse); // �����x��ύX�ł���`�揈��
	void Draw(const D3DXCOLOR Diffuse); // �F��ύX�ł���`�揈��
	void DrawShadow(void);
	void SetUpMatrix(const D3DXVECTOR3 Scal = D3DXVECTOR3(1.0f,1.0f,1.0f));
	void SetUpMatrix(const D3DXMATRIX mtxRot, const D3DXMATRIX mtxTrans,const D3DXVECTOR3 Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void SetUpDraw(const float Diffuse = 1.0f);
	void SetTextureMT(const char* pTextureName);
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }

	HRESULT LoadModel(const char* pXFileName);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	D3DXMATRIX GetMatrix(void) const { return m_mtxWorld; }
	D3DXVECTOR3 GetSize(void);
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	const CModelManager::ModelInfo GetModelInfo(void) const;

private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	int m_nModelIdx;		// ���f���̃C���f�b�N�X
	int m_nTextureMT;		// �}���`�e�N�X�`���̃C���f�b�N�X
};
#endif
