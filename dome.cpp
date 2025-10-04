//================================================
//
// ���b�V���h�[�� [dome.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "dome.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include <string>

using namespace std;
using namespace Const; // ���O���Const���g�p

//================================================
// �R���X�g���N�^
//================================================
CMeshDome::CMeshDome()
{
	m_nOffsetIdx = NULL;
	m_fHeight = NULL;
	m_fRadius = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nNumIdx = NULL;
	m_nNumPolygon = NULL;
	m_nNumVtx = NULL;
	m_nSegH = 1;
	m_nSegV = 1;
	m_nTextureIdx = -1;
	m_pIdxBuffer = nullptr;
	m_pVtxBuffer = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CMeshDome::~CMeshDome()
{
}

//================================================
// ��������
//================================================
CMeshDome* CMeshDome::Create(const D3DXVECTOR3 pos, const int nSegH, const int nSegV, const float fRadius, const float fHeight, const D3DXVECTOR3 rot)
{
	// ���b�V���h�[���𐶐�
	CMeshDome* pMesh = new CMeshDome;

	if (pMesh == nullptr) return nullptr;

	// ���_���̐ݒ�
	int nNumFanVtx = nSegH + 2;

	int nNumIdxFan = 3 * (nSegH + 1);

	// ���_���̐ݒ�
	int nNumDomeVtx = (nSegH + 1) * (nSegV + 1);

	// �|���S�����̐ݒ�
	int nNumDomePolygon = (((nSegH * nSegV) * 2)) + (4 * (nSegV - 1));

	// �C���f�b�N�X���̐ݒ�
	int nNumDomeIndex = nNumDomePolygon + 2;

	// ���_���̐ݒ�
	int nNumVtx = nNumFanVtx + nNumDomeVtx;

	// �C���f�b�N�X���̌v�Z
	int nNumIdx = nNumDomeIndex + nNumIdxFan;

	// �v�f�̐ݒ�
	pMesh->m_nNumIdx = nNumIdx;
	pMesh->m_nNumVtx = nNumVtx;
	pMesh->m_nNumPolygon = nNumDomePolygon;
	pMesh->m_nSegH = nSegH;
	pMesh->m_nSegV = nSegV;
	pMesh->m_pos = pos;
	pMesh->m_rot = rot;
	pMesh->m_fHeight = fHeight;
	pMesh->m_fRadius = fRadius;

	// ����������
	if (FAILED(pMesh->Init()))
	{
		pMesh->Uninit();
		pMesh = nullptr;
		return nullptr;
	}

	return pMesh;
}

//================================================
// ����������
//================================================
HRESULT CMeshDome::Init(void)
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

	// �h�[���̐ݒ�
	SetDome(m_nSegH, m_nSegV, m_fRadius, m_fHeight);

	// �e�N�X�`����ID�̐ݒ�
	SetTextureID("sky000.jpg");

	return S_OK;
}

//================================================
// �I������
//================================================
void CMeshDome::Uninit(void)
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
void CMeshDome::Update(void)
{
	// ��]����
	m_rot.y += 0.001f;
}

//================================================
// �`�揈��
//================================================
void CMeshDome::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	int nSegH = m_nSegH;
	int nSegV = m_nSegV;

	// ���_���̐ݒ�
	int nNumFanVtx = nSegH + 2;

	// �|���S�����̐ݒ�
	int nNumFanPolygon = nSegH;

	// ���_���̐ݒ�
	int nNumDomeVtx = (nSegH + 1) * (nSegV + 1);

	// �|���S�����̐ݒ�
	int nNumDomePolygon = (((nSegH * nSegV) * 2)) + (4 * (nSegV - 1));

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
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, nNumFanVtx, 0, nNumFanPolygon);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumDomeVtx, m_nOffsetIdx, nNumDomePolygon);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================================
// �h�[���̐ݒ菈��
//================================================
void CMeshDome::SetDome(const int nSegH, const int nSegV, const float fRadius, const float fHeight)
{
	int nCntVtx = 0;

	// �e�N�X�`���̃I�t�Z�b�g
	float fTexX = 1.0f / nSegH;
	float fTexY = 1.0f / nSegV;

	float fNowRadius = fRadius / (nSegV + 1);

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ����̍��W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, fHeight + (fHeight / nSegV + 1), 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �F�̐ݒ�
	pVtx[0].col = WHITE;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.5f);

	nCntVtx++;

	// �W�̕����̍쐬
	for (int nCntX = 0; nCntX <= nSegH; nCntX++)
	{
		float fAngle = (D3DX_PI * 2.0f) / nSegH * nCntX;

		D3DXVECTOR3 posWk = VEC3_NULL;

		posWk.x = cosf(fAngle) * fNowRadius;
		posWk.y = fHeight;
		posWk.z = sinf(fAngle) * fNowRadius;

		// ����̍��W�̐ݒ�
		pVtx[nCntVtx].pos = posWk;

		// �@���̐ݒ�
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�̐ݒ�
		pVtx[nCntVtx].col = WHITE;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexX * nCntX, 0.0f);

		nCntVtx++;
	}
	
	// �C���f�b�N�X�̃I�t�Z�b�g�̕ۑ�
	int OffsetIdx = nCntVtx;

	int nCntIdx = 0;

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	// ���_����
	for (int nCnt = 0; nCnt < nSegH + 2; nCnt++)
	{
		// �C���f�b�N�X�̐ݒ�
		pIdx[nCntIdx++] = static_cast<WORD>(nCnt);
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuffer->Unlock();

	// �C���f�b�N�X�̃I�t�Z�b�g��ݒ�
	m_nOffsetIdx = nCntIdx;

	// �e�N�X�`���̃I�t�Z�b�g
	fTexX = 1.0f / nSegH;
	fTexY = 1.0f / nSegV;

	// ���a
	fNowRadius = fRadius / (nSegV + 1);
	float fRateRadius = fNowRadius;

	//�c
	for (int nCntZ = 0; nCntZ <= nSegV; nCntZ++)
	{
		//��
		for (int nCntX = 0; nCntX <= nSegH; nCntX++)
		{
			float fAngel = (D3DX_PI * 2.0f) / nSegH * nCntX;

			D3DXVECTOR3 posWk = VEC3_NULL;

			posWk.x = sinf(fAngel) * fNowRadius;
			posWk.y = fHeight - (fHeight / nSegV) * nCntZ;
			posWk.z = cosf(fAngel) * fNowRadius;
			
			// ����̍��W�̐ݒ�
			pVtx[nCntVtx].pos = posWk;

			// �@���̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�̐ݒ�
			pVtx[nCntVtx].col = WHITE;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[nCntVtx].tex = D3DXVECTOR2(fTexX * nCntX, fTexY * nCntZ);

			nCntVtx++;//���Z
		}

		// ���a���g��
		fNowRadius += fRateRadius / (nCntZ + 1);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	int IndxCount3 = nSegH + 1;//X

	int IdxCnt = m_nOffsetIdx;//�z��

	int Num = 0;//

	int Index0 = 0;
	int Index1 = 0;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < nSegV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegH; IndxCount2++, IndxCount3++, Num++)
		{
			Index0 = IndxCount3 + OffsetIdx;
			Index1 = Num + OffsetIdx;

			// �C���f�b�N�X�o�b�t�@�̐ݒ�
			pIdx[IdxCnt] = static_cast<WORD>(Index0);
			pIdx[IdxCnt + 1] = static_cast<WORD>(Index1);
			IdxCnt += 2;
		}

		if (IndxCount1 < nSegV - 1)
		{
			// �C���f�b�N�X�o�b�t�@�̐ݒ�
			pIdx[IdxCnt] = static_cast<WORD>((Num - 1 + OffsetIdx));
			pIdx[IdxCnt + 1] = static_cast<WORD>(IndxCount3 + OffsetIdx);
			IdxCnt += 2;
		}
	}
	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuffer->Unlock();
}

//================================================
// �e�N�X�`����ID�̐ݒ�
//================================================
void CMeshDome::SetTextureID(const char* pTextureName)
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
