//===================================================
//
// ��ԏ��̏��������N���X [transform.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "transform.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// �R���X�g���N�^
//===================================================
CTransform::CTransform()
{
	// �l�̃N���A
	ZeroMemory(&m_Info, sizeof(m_Info));
}

//===================================================
// �f�X�g���N�^
//===================================================
CTransform::~CTransform()
{
}

//===================================================
// �ʒu�̍X�V����
//===================================================
void CTransform::UpdatePosition(const D3DXVECTOR3 move)
{
	// �O��̈ʒu�̕ۑ�
	m_Info.posOld = m_Info.pos;

	// �ʒu�̍X�V
	m_Info.pos += move;
}

//===================================================
// �}�g���b�N�X�̐ݒ�
//===================================================
void CTransform::SetMatrix(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);
}
