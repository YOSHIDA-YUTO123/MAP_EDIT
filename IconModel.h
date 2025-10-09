//===================================================
//
// ���f���̃A�C�R���̍쐬 [IconModel.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _ICONMODEL_H_
#define _ICONMODEL_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"

//***************************************************
// ���f���̃A�C�R���̍쐬
//***************************************************
class CIconModel : public CObject
{
public:
	~CIconModel();

	// ��������
	static CIconModel* Create(void) { return new CIconModel; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	CIconModel();
	void SetDraw(void);

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
};

#endif