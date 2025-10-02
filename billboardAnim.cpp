//===================================================
//
// �r���{�[�h�A�j���[�V�����̕`�� [billboardAnim.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "billboardAnim.h"
#include "manager.h"
#include "textureManager.h"
#include "renderer.h"

using namespace Const; // ���O���Const���g�p

//===================================================
// �R���X�g���N�^
//===================================================
CBillboardAnimation::CBillboardAnimation(int nPriority) : CObject(nPriority)
{
	m_bLoop = false;
	m_fPosTexH = NULL;
	m_fPosTexU = NULL;
	m_nAnimSpeed = NULL;
	m_nCounterAnim = NULL;
	m_nPatternAnim = NULL;
	m_nH = 1;
	m_nV = 1;
}

//===================================================
// �f�X�g���N�^
//===================================================
CBillboardAnimation::~CBillboardAnimation()
{
}

//===================================================
// �A�j���[�V�����̐ݒ菈��
//===================================================
void CBillboardAnimation::SetAnim(const int nAnimSpeed, const int U, const int V, const bool bLoop)
{
	m_bLoop = bLoop;
	m_nAnimSpeed = nAnimSpeed;
	m_nH = U;
	m_nV = V;
}

//===================================================
// �e�N�X�`���̍��W�̐ݒ�
//===================================================
void CBillboardAnimation::SetTextureVtx(const D3DXVECTOR2 tex, const D3DXVECTOR2 offtex)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(offtex.x, offtex.y);
	pVtx[1].tex = D3DXVECTOR2(offtex.x + tex.x, offtex.y);
	pVtx[2].tex = D3DXVECTOR2(offtex.x, offtex.y + tex.y);
	pVtx[3].tex = D3DXVECTOR2(offtex.x + tex.x, offtex.y + tex.y);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �F�̐ݒ�
//===================================================
void CBillboardAnimation::SetColor(const D3DXCOLOR col)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �e�N�X�`����ID�̐ݒ�
//===================================================
void CBillboardAnimation::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`���}�l�[�W���[�̎擾
	auto pTexture = CManager::GetTexture();

	// null��������-1
	if (pTextureName == nullptr)
	{
		m_nTextureIdx = -1;
		return;
	}

	// �e�N�X�`����ID�̓o�^
	m_nTextureIdx = pTexture->Register(pTextureName);
}

//===================================================
// ���_���W�̍X�V
//===================================================
void CBillboardAnimation::UpdateVertexPos(const D3DXVECTOR3 pos)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu�̐ݒ�
	m_pos = pos;

	// �傫���̎擾
	D3DXVECTOR2 Size = m_Size;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// ��������
//===================================================
CBillboardAnimation* CBillboardAnimation::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nSpeed, const int U, const int V, const bool bLoop)
{
	CBillboardAnimation* pBillboard = new CBillboardAnimation;

	// �A�j���[�V�����̐ݒ�
	pBillboard->SetAnim(nSpeed, U, V, bLoop);

	pBillboard->SetPosition(pos);
	pBillboard->SetSize(Size);

	// ����������
	if (FAILED(pBillboard->Init()))
	{
		pBillboard->Uninit();
		pBillboard = nullptr;
		return nullptr;;
	}

	return pBillboard;
}

//===================================================
// ����������
//===================================================
HRESULT CBillboardAnimation::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �傫���̎擾
	D3DXVECTOR2 Size = m_Size;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = WHITE;
	pVtx[1].col = WHITE;
	pVtx[2].col = WHITE;
	pVtx[3].col = WHITE;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / m_nH, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / m_nH);
	pVtx[3].tex = D3DXVECTOR2(1.0f / m_nH, 1.0f / m_nH);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CBillboardAnimation::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// �������g�̔j��
	Release();
}

//===================================================
// �X�V����
//===================================================
void CBillboardAnimation::Update(void)
{
	// �A�j���[�V�����̃J�E���^�[��i�߂�
	m_nCounterAnim++;

	m_fPosTexU = 1.0f / m_nH;
	m_fPosTexH = 1.0f / m_nV;

	if (m_nCounterAnim >= m_nAnimSpeed)
	{
		m_nCounterAnim = 0;

		m_nPatternAnim++;

		// Y���W�̊���
		int nRatePosY = m_nPatternAnim / m_nH;

		float UV = m_fPosTexU * m_nPatternAnim;
		float HV = nRatePosY * m_fPosTexH;

		// �e�N�X�`�����W�̍X�V
		SetTextureVtx(D3DXVECTOR2(m_fPosTexU, m_fPosTexH), D3DXVECTOR2(UV, HV));

		// �ő�܂ōs������
		if (m_nPatternAnim >= (m_nH * m_nV))
		{
			// �p�^�[����������
			m_nPatternAnim = 0;

			if (m_bLoop == false)
			{
				Uninit();
				return;
			}
		}
	}
}

//===================================================
// �`�揈��
//===================================================
void CBillboardAnimation::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X��錾
	D3DXMATRIX mtxView;

	// �r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �J�����̋t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//// Z�e�X�g
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
