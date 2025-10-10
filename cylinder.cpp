//================================================
//
// ���b�V���V�����_�[ [cylinder.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "cylinder.h"
#include "manager.h"
#include "math.h"
#include "renderer.h"
#include <string>
#include "textureManager.h"

using namespace Const; // ���O���Const���g�p
using namespace math;  // ���O���math���g�p
using namespace std;   // ���O���std���g�p

//================================================
// �R���X�g���N�^
//================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CObject(nPriority)
{
	m_bShow = false;
	m_fHeight = NULL;
	m_Type = TYPE_WALL;
	m_pos = VEC3_NULL;
	m_col = WHITE;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nNumIdx = NULL;
	m_nNumPolygon = NULL;
	m_nNumVtx = NULL;
	m_nSegH = 1;
	m_nSegV = 1;
	m_nTextureIdx = -1;
	m_pIdxBuffer = nullptr;
	m_pVtxBuffer = nullptr;
	m_rot = VEC3_NULL;
	m_fRadius = NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CMeshCylinder::~CMeshCylinder()
{
}

//================================================
// ��������
//================================================
CMeshCylinder* CMeshCylinder::Create(const D3DXVECTOR3 pos, const int nSegH, const int nSegV,const float fRadius, const float fHeight, const D3DXVECTOR3 rot)
{
	// ���b�V���t�B�[���h�𐶐�
	CMeshCylinder* pMesh = new CMeshCylinder;

	// ���_���̐ݒ�
	pMesh->m_nNumVtx = (nSegH + 1) * (nSegV + 1);

	// �|���S�����̐ݒ�
	pMesh->m_nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

	// �C���f�b�N�X���̐ݒ�
	pMesh->m_nNumIdx = pMesh->m_nNumPolygon + 2;

	pMesh->m_nSegH = nSegH;
	pMesh->m_nSegV = nSegV;
	pMesh->m_pos = pos;
	pMesh->m_fRadius = fRadius;
	pMesh->m_rot = rot;
	pMesh->m_fHeight = fHeight;

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
HRESULT CMeshCylinder::Init(void)
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

	float fTexPosX = 1.0f / m_nSegH;
	float fTexPosY = 1.0f / m_nSegV;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �c�̕���������
	for (int nCntV = 0; nCntV <= m_nSegV; nCntV++)
	{
		for (int nCntH = 0; nCntH <= m_nSegH; nCntH++)
		{
			// �v�Z�p�̈ʒu
			D3DXVECTOR3 posWk = VEC3_NULL;

			// ���̕�����
			float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntH;

			posWk.x = sinf(fAngle) * m_fRadius;
			posWk.y = m_fHeight - (m_fHeight / m_nSegV * nCntV);
			posWk.z = cosf(fAngle) * m_fRadius;

			// �ʒu�̐ݒ�
			pVtx[nCntVtx].pos = posWk;

			// ���_�܂ł̕��������߂�
			D3DXVECTOR3 nor = pVtx[nCntVtx].pos - m_pos;

			// �����x�N�g���ɂ���
			D3DXVec3Normalize(&nor, &nor);

			// �@���̐ݒ�
			pVtx[nCntVtx].nor = nor;

			// �F�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntH), (fTexPosY * nCntV));

			nCntVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	int IndxNum = m_nSegH + 1; // �C���f�b�N�X�̐��l1

	int IdxCnt = 0; // �z��

	int Num = 0; // �C���f�b�N�X�̐��l2

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

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
void CMeshCylinder::Uninit(void)
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
void CMeshCylinder::Update(void)
{
	int nCntVtx = 0;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �c�̕���������
	for (int nCntV = 0; nCntV <= m_nSegV; nCntV++)
	{
		for (int nCntH = 0; nCntH <= m_nSegH; nCntH++)
		{
			// �v�Z�p�̈ʒu
			D3DXVECTOR3 posWk = VEC3_NULL;

			// ���̕�����
			float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntH;

			posWk.x = sinf(fAngle) * m_fRadius;
			posWk.y = m_fHeight - (m_fHeight / m_nSegV * nCntV);
			posWk.z = cosf(fAngle) * m_fRadius;

			// �ʒu�̐ݒ�
			pVtx[nCntVtx].pos = posWk;

			// ���_�܂ł̕��������߂�
			D3DXVECTOR3 nor = pVtx[nCntVtx].pos - m_pos;

			// �����x�N�g���ɂ���
			D3DXVec3Normalize(&nor, &nor);

			// �@���̐ݒ�
			pVtx[nCntVtx].nor = nor;

			nCntVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();
}

//================================================
// �`�揈��
//================================================
void CMeshCylinder::Draw(void)
{
	// �����Ȃ��Ȃ珈�����Ȃ�
	if (!m_bShow) return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// ���C�g�̉e���������Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// ���Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================================
// �����蔻��
//================================================
bool CMeshCylinder::Collision(D3DXVECTOR3 *pPos)
{
	// ���̕������̎擾
	int nSegH = m_nSegH;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���̕����������ׂ�
	for (int nCnt = 0; nCnt <= nSegH; nCnt++)
	{
		int nNextIndx = (nCnt + 1) % nSegH; // ���̒��_�̃C���f�b�N�X
		int nIndx = nCnt;					// ���̒��_�̃C���f�b�N�X

		// ���_���W�̎擾
		D3DXVECTOR3 vtx0 = pVtx[nIndx].pos, vtx1 = pVtx[nNextIndx].pos;

		// �Ӄx�N�g���̍쐬
		D3DXVECTOR3 edge = GetVector(vtx1, vtx0);

		// �v���C���[�܂ł̃x�N�g�����쐬
		D3DXVECTOR3 playerVec = GetVector(*pPos, vtx0);

		D3DXVECTOR3 cross; // �O��

		// �O�ς���
		D3DXVec3Cross(&cross, &edge, &playerVec);

		// ���K������
		D3DXVec3Normalize(&cross, &cross);

		// �v���C���[���V�����_�[�̊O�ɏo����
		if (cross.y <= 0.0f)
		{
			D3DXVECTOR3 objectPos = *pPos;

			// �V�����_�[�̒��S�܂ł̃x�N�g�����쐬
			D3DXVECTOR3 CenterDir = GetVector(objectPos, m_pos);

			// ���S����v���C���[�܂ł̋��������߂�
			float fDistance = GetDistance(objectPos - m_pos);

			// �߂荞�񂾐[�������߂�
			float fDepth = (m_fRadius - 4.0f) - fDistance;

			// �v���C���[�̈ʒu���璆�S�܂ł̕����~�߂荞�񂾐[���𑫂�
			D3DXVECTOR3 pos = objectPos + CenterDir * fDepth;
			
			// �ʒu��ݒ�
			pPos->x = pos.x;
			pPos->z = pos.z;

			return true;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	return false;
}

//================================================
// �e�N�X�`����ID�̐ݒ�
//================================================
void CMeshCylinder::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`���}�l�[�W���[�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �ȗ��p�t�@�C���p�X
	string filePath = "data/TEXTURE/";

	// ������̘A��
	filePath += pTextureName;

	if (pTexture != nullptr)
	{
		// �e�N�X�`����ID�̐ݒ�
		m_nTextureIdx = pTexture->Register(filePath.c_str());
	}
}
