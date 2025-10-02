//===================================================
//
// 3D�|���S��(�}���`�e�N�X�`��) [Object3DMT.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "Object3DMT.h"
#include"manager.h"
#include"renderer.h"
#include"textureManager.h"
#include<string>

using namespace Const; // ���O���Const���g�p
using namespace std;   // ���O���std���g�p

//===================================================
// �R���X�g���N�^
//===================================================
CObject3DMT::CObject3DMT()
{
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_Size = VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pVtxBuffer = nullptr;
	memset(&m_nTextureIdx, -1, sizeof(m_nTextureIdx));
}

//===================================================
// �f�X�g���N�^
//===================================================
CObject3DMT::~CObject3DMT()
{
}

//===================================================
// ��������
//===================================================
CObject3DMT* CObject3DMT::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 Size)
{
	CObject3DMT* pObject = new CObject3DMT;

	// ����������
	if (FAILED(pObject->Init()))
	{
		pObject->Uninit();
		pObject = nullptr;
		return nullptr;
	}
	pObject->SetPosition(pos);
	pObject->SetRotaition(rot);
	pObject->m_Size = Size;
	pObject->SetVtx(WHITE);

	return pObject;
}

//===================================================
// ����������
//===================================================
HRESULT CObject3DMT::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULT) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CObject3DMT::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// �������g�̔j��
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CObject3DMT::Update(void)
{

}

//===================================================
// �`�揈��
//===================================================
void CObject3DMT::Draw(void)
{
	// �����_���[�̎擾
	CRenderer* pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//	���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ����
	D3DXVECTOR3 rot = m_rot;

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu
	D3DXVECTOR3 pos = m_pos;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D_MULT));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3DMT);

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_nTextureIdx[nCnt] != -1)
		{
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(nCnt, pTexture->GetAdress(m_nTextureIdx[nCnt]));
		}
		else
		{
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(nCnt, NULL);
		}
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���Ƃɖ߂�
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//===================================================
// ���_�o�b�t�@�̐ݒ菈��
//===================================================
void CObject3DMT::SetVtx(const D3DXCOLOR col)
{
	// ���_���̃|�C���^
	VERTEX_3D_MULT* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �傫���̎擾
	D3DXVECTOR3 Size = m_Size;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, -Size.z);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].texMT = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texMT = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texMT = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].texMT = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �e�N�X�`��ID�̐ݒ�
//===================================================
void CObject3DMT::SetTextureID(const char* pTexture0, const char* pTexture1)
{
	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTexture == nullptr) return;

	// �e�N�X�`���̃p�X
	string TexturePath = "data/TEXTURE/";

	// ��������Ȃ���
	TexturePath += pTexture0;

	m_nTextureIdx[0] = pTexture->Register(TexturePath.c_str());

	// �e�N�X�`���̃p�X
	TexturePath = "data/TEXTURE/";

	// ��������Ȃ���
	TexturePath += pTexture1;

	m_nTextureIdx[1] = pTexture->Register(TexturePath.c_str());

}
