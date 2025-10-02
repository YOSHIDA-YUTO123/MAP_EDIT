//================================================
//
// �O�� [orbit.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "Orbit.h"
#include "manager.h"
#include "renderer.h"
#include <string>
#include "textureManager.h"

using namespace std;	// ���O���std�̎g�p
using namespace Const;	// ���O���Const���g�p����

//================================================
// �R���X�g���N�^
//================================================
CMeshOrbit::CMeshOrbit()
{
	m_Bottom = VEC3_NULL;
	m_Top = VEC3_NULL;
	m_col = WHITE;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nNumIdx = NULL;
	m_nNumPolygon = NULL;
	m_nNumVtx = NULL;
	m_nSegH = 1;
	m_nSegV = 1;
	m_nTextureIdx = -1;
	m_pIdxBuffer = nullptr;
	m_pos = VEC3_NULL;
	m_pVtxBuffer = nullptr;
	m_rot = VEC3_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CMeshOrbit::~CMeshOrbit()
{
}

//================================================
// �O�Ղ̐���
//================================================
CMeshOrbit* CMeshOrbit::Create(const D3DXVECTOR3 Top, const D3DXVECTOR3 Bottom, const int nSegH,const D3DXCOLOR col)
{
	// �O�Ղ𐶐�
	CMeshOrbit* pMesh = new CMeshOrbit;

	// �c�̕�����
	const int nSegV = 1;

	// ���_�̑���
	pMesh->m_nNumVtx = (nSegH + 1) * (nSegV + 1);

	// �C���f�b�N�X�̑���
	pMesh->m_nNumIdx = pMesh->m_nNumVtx;

	// �|���S���̑���
	pMesh->m_nNumPolygon = pMesh->m_nNumVtx - 2;

	// �����Ɏ��s����
	if (FAILED(pMesh->Init()))
	{
		pMesh->Uninit();

		pMesh = nullptr;

		return nullptr;
	}

	// �e�N�X�`����(��)
	float fPosTexH = 1.0f / nSegH;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	// ���_������
	for (int nCnt = 0; nCnt < pMesh->m_nNumVtx; nCnt++)
	{
		// �A���t�@�l�̐ݒ�
		float fAlv = col.a * (1.0f - (float)nCnt / pMesh->m_nNumVtx);

		// ���_�̐ݒ�(��������ꂩ���ɏW�߂�)
		pVtx[nCnt].pos = Top;

		// �@���A�F�A�e�N�X�`���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCnt].col = D3DXCOLOR(col.r,col.g,col.b, fAlv);
		pVtx[nCnt].tex = D3DXVECTOR2(fPosTexH * nCnt, 1.0f);

		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		pIdx[nCnt] = static_cast<WORD>(nCnt);
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	pMesh->m_pIdxBuffer->Unlock();

	// �l�O����n�߂�
	for (int nCnt = pMesh->m_nNumVtx - 4; nCnt >= 0; nCnt -= 2)
	{
		// �O�̒��_���W������
		D3DXVECTOR3 OldVtx0 = pVtx[nCnt].pos;
		D3DXVECTOR3 OldVtx1 = pVtx[nCnt + 1].pos;

		// ���_���W�̐ݒ�
		pVtx[nCnt + 2].pos = OldVtx0;
		pVtx[nCnt + 3].pos = OldVtx1;
	}

	// �V�������_��ǉ��i�擪�ɓ����j
	pVtx[0].pos = Bottom;
	pVtx[1].pos = Top;

	// ���_�o�b�t�@���A�����b�N
	pMesh->m_pVtxBuffer->Unlock();

	// �ݒ菈��
	pMesh->m_Bottom = Bottom;
	pMesh->m_Top = Top;
	pMesh->m_col = col;
	pMesh->m_nSegH = nSegH;

	return pMesh;
}

//================================================
// ����������
//================================================
HRESULT CMeshOrbit::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// �e�N�X�`����ID
	SetTextureID("gradation/orbit.png");

	return S_OK;
}

//================================================
// �I������
//================================================
void CMeshOrbit::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// �������g�̔j��
	CObject::Release();
}

//================================================
// �X�V����
//================================================
void CMeshOrbit::Update(void)
{
	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �l�O����n�߂�
	for (int nCnt = m_nNumVtx - 4; nCnt >= 0; nCnt -= 2)
	{
		// �O�̒��_���W������
		D3DXVECTOR3 OldVtx0 = pVtx[nCnt].pos;
		D3DXVECTOR3 OldVtx1 = pVtx[nCnt + 1].pos;

		// ���_���W�̐ݒ�
		pVtx[nCnt + 2].pos = OldVtx0;
		pVtx[nCnt + 3].pos = OldVtx1;
	}

	// �V�������_��ǉ��i�擪�ɓ����j
	pVtx[0].pos = m_Bottom;
	pVtx[1].pos = m_Top;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();
}

//================================================
// �`�揈��
//================================================
void CMeshOrbit::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �J�����O���I���ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	// a�u�����f�B���O
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffer);

	//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`��������������
	if (m_nTextureIdx == -1)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
	else
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));

	}

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);

	// a�u�����f�B���O�����Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �J�����O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//================================================
// �ʒu�̐ݒ菈��
//================================================
void CMeshOrbit::SetPosition(const D3DXVECTOR3 Top, const D3DXVECTOR3 Bottom)
{
	m_Top = Top;
	m_Bottom = Bottom;
}

//================================================
// �e�N�X�`����ID�̐ݒ�
//================================================
void CMeshOrbit::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`���}�l�[�W���[�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �t�@�C���p�X
	string filePath = "data/TEXTURE/";

	// ������̘A��
	filePath += pTextureName;

	if (pTexture != nullptr)
	{
		// �e�N�X�`����ID�̎擾
		m_nTextureIdx = pTexture->Register(filePath.c_str());
	}
}
