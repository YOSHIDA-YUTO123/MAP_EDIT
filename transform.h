//===================================================
//
// ��ԏ��̏��������N���X [transform.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "main.h"

//***************************************************
// ��ԏ��N���X�̒�`
//***************************************************
class CTransform
{
public:

	// ���
	struct Info
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 posOld;		// �O��̈ʒu
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 Size;		// �傫��
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
		float fRadius;			// ���a
	};

	CTransform();
	~CTransform();

	static CTransform* Create(void) { return new CTransform; }
	void UpdatePosition(const D3DXVECTOR3 move);
	void SetMatrix(void);
	Info GetInfo(void) const { return m_Info; }
	void SetInfo(const Info& info) { m_Info = info; }

private:
	Info m_Info; // ���
};
#endif
