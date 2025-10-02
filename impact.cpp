//================================================
//
// ���b�V���C���p�N�g [impact.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "impact.h"
#include "manager.h"
#include "renderer.h"
#include "meshfield.h"
#include "textureManager.h"
#include <string>

using namespace std;	// ���O���std���g�p����
using namespace Const;	// ���O���Const���g�p����

//================================================
// �R���X�g���N�^
//================================================
CMeshCircle::CMeshCircle(const int nPriority) : CObject(nPriority)
{
	m_Incol = m_Outcol = WHITE;
	m_fDecAlv = NULL;
	m_bFiledCollision = true;
	m_fHeight = NULL;
	m_fInRadius = NULL;
	m_fOutRadius = NULL;
	m_fSpeed = NULL;
	m_nLife = NULL;
	m_pos = VEC3_NULL;
	m_nNumIdx = NULL;
	m_nNumPolygon = NULL;
	m_nNumVtx = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nSegH = 1;
	m_nSegV = 1;
	m_nTextureIdx = -1;
	m_pIdxBuffer = nullptr;
	m_pVtxBuffer = nullptr;
	m_rot = VEC3_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CMeshCircle::~CMeshCircle()
{
}

//================================================
// ��������
//================================================
CMeshCircle* CMeshCircle::Create(const D3DXCOLOR col, const D3DXVECTOR3 pos,const float fInRadius,const float fOutRadius, const int nSegH)
{
	// ���b�V���C���p�N�g�𐶐�
	CMeshCircle* pMesh = new CMeshCircle;

	// z�̕�����1�Œ�
	const int nSegV = 1;

	// ���_���̐ݒ�
	pMesh->m_nNumVtx = (nSegH + 1) * (nSegV + 1);

	// �|���S�����̐ݒ�
	pMesh->m_nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

	// �C���f�b�N�X���̐ݒ�
	pMesh->m_nNumIdx = pMesh->m_nNumPolygon + 2;

	pMesh->m_nSegH = nSegH;
	pMesh->m_nSegV = nSegV;
	pMesh->m_fInRadius = fInRadius;
	pMesh->m_fOutRadius = fOutRadius;
	pMesh->m_Outcol = col;
	pMesh->m_Incol = D3DXCOLOR(col.r, col.g, col.b, col.a * 0.5f);
	pMesh->m_pos = pos;

	return pMesh;
}

//================================================
// �T�[�N���̐ݒ菈��
//================================================
void CMeshCircle::SetCircle(const float fHeight, const float fSpeed, const int nLife, const bool bField, const D3DXVECTOR3 rot)
{
	// �����ɉ����������x�̌����l�̌v�Z
	m_fDecAlv = m_Outcol.a / nLife;

	// �v�f�̐ݒ菈��
	m_fHeight = fHeight;
	m_fSpeed = fSpeed;
	m_nLife = nLife;
	m_bFiledCollision = bField;
	m_rot = rot;

	// �������Ɏ��s������
	if (FAILED(Init()))
	{
		Uninit();
		return;
	}
}

//================================================
// ����������
//================================================
HRESULT CMeshCircle::Init(void)
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

	// ���_�̃J�E���g
	int nCntVtx = 0;

	// �e�N�X�`���̍��W�̊���
	float fTexPosX = 1.0f / m_nSegH;

	// �v�Z�p�̈ʒu
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���̕���������
	for (int nCntH = 0; nCntH < m_nSegH; nCntH++)
	{
		// ����̊��������Ƃ߂�
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntH;

		// �ʒu�̐ݒ�
		posWk.x = sinf(fAngle) * m_fInRadius;
		posWk.y = m_fHeight;
		posWk.z = cosf(fAngle) * m_fInRadius;

		// �ʒu�̐ݒ�
		pVtx[nCntVtx].pos = posWk;

		// �@���̐ݒ�
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		// �����̐F�̐ݒ�
		pVtx[nCntVtx].col = m_Incol;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntH, 1.0f);

		nCntVtx++;
	}

	// ���̕���������
	for (int nCntH = 0; nCntH < m_nSegH; nCntH++)
	{
		// ����̊��������Ƃ߂�
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntH;

		// �ʒu�̐ݒ�
		posWk.x = sinf(fAngle) * m_fOutRadius;
		posWk.y = m_fHeight;
		posWk.z = cosf(fAngle) * m_fOutRadius;

		// �ʒu�̐ݒ�
		pVtx[nCntVtx].pos = posWk;

		// �@���̐ݒ�
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �����̐F�̐ݒ�
		pVtx[nCntVtx].col = m_Outcol;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntH, 1.0f);

		nCntVtx++;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = m_nSegH + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < m_nSegV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_nSegH; IndxCount2++, IndxNum++, Num++)
		{
			// �C���f�b�N�X�o�b�t�@�̐ݒ�
			pIdx[IdxCnt] = static_cast<WORD>(IndxNum);
			pIdx[IdxCnt + 1] = static_cast<WORD>(Num);
			IdxCnt += 2;
		}

		// NOTE:�Ō�̍s����Ȃ�������
		if (IndxCount1 < m_nSegV - 1)
		{
			pIdx[IdxCnt] = static_cast<WORD>(Num - 1);
			pIdx[IdxCnt + 1] = static_cast<WORD>(IndxNum);
			IdxCnt += 2;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuffer->Unlock();

	return S_OK;
}

//================================================
// �I������
//================================================
void CMeshCircle::Uninit(void)
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
void CMeshCircle::Update(void)
{
	int nCntVtx = 0; // ���_���̃J�E���^�[

	// ���a���g�傷��
	m_fInRadius += m_fSpeed;
	m_fOutRadius += m_fSpeed;

	// ���_��y���W
	float fHeight = 0.0f;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �����̒��_�̍X�V
	for (int nCntH = 0; nCntH <= m_nSegH; nCntH++)
	{
		D3DXVECTOR3 posWk = VEC3_NULL;

		// ����̊��������߂�
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntH;

		// �ό`�ɒ��_������
		posWk.x = sinf(fAngle) * m_fInRadius;
		posWk.y = m_fHeight;
		posWk.z = cosf(fAngle) * m_fInRadius;

		// �ʒu�̐ݒ�
		pVtx[nCntVtx].pos = posWk;

		// �F�̐ݒ�
		pVtx[nCntVtx].col = m_Incol;

		// ���_�̃J�E���g�����Z
		nCntVtx++;
	}

	// �O���̒��_�̍X�V
	for (int nCntH = 0; nCntH <= m_nSegH; nCntH++)
	{
		D3DXVECTOR3 posWk = VEC3_NULL;

		// ����̊��������߂�
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntH;

		// �ό`�ɒ��_������
		posWk.x = sinf(fAngle) * m_fOutRadius;
		posWk.y = 0.0f;
		posWk.z = cosf(fAngle) * m_fOutRadius;

		// �ʒu�̐ݒ�
		pVtx[nCntVtx].pos = posWk;

		// �F�̐ݒ�
		pVtx[nCntVtx].col = m_Outcol;

		// ���_�̃J�E���g�����Z
		nCntVtx++;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	// a�l�����炷
	m_Incol.a -= m_fDecAlv;
	m_Outcol.a -= m_fDecAlv;

	// ���������炷
	m_nLife--;

	// �������s������
	if (m_nLife <= 0)
	{
		// �I������
		Uninit();

		return;
	}
}

//================================================
// �`�揈��
//================================================
void CMeshCircle::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �J�����O���I���ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================================
// �e�N�X�`����ID�̐ݒ�
//================================================
void CMeshCircle::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`���}�l�[�W���[�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �t�@�C���p�X
	string filePath = "data/TEXTURE/gradation/";

	// ������̘A��
	filePath += pTextureName;

	if (pTexture != nullptr)
	{
		// �e�N�X�`����ID�̎擾
		m_nTextureIdx = pTexture->Register(filePath.c_str());
	}
}