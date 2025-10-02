//================================================
//
// �����𑀍삷�鏈�� [number.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "number.h"
#include "object2D.h"
#include"manager.h"
#include"renderer.h"
#include"textureManager.h"

//************************************************
// �ÓI�����o�ϐ��錾
//************************************************

using namespace Const;							// ���O���Const���g�p����

//================================================
// �R���X�g���N�^
//================================================
CNumber::CNumber()
{
	m_pVtxBuffer = NULL;
	m_pos = VEC3_NULL;
	m_Size = VEC2_NULL;
	m_nTextureIdx = NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CNumber::~CNumber()
{
}

//================================================
// ����������
//================================================
HRESULT CNumber::Init(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// �ʒu�Ƒ傫���̐ݒ�
	m_pos = pos;
	m_Size =  Size;

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - Size.x, pos.y - Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + Size.x, pos.y - Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - Size.x, pos.y + Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + Size.x, pos.y + Size.y, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = WHITE;
	pVtx[1].col = WHITE;
	pVtx[2].col = WHITE;
	pVtx[3].col = WHITE;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / 9.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 9.0f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//================================================
// �I������
//================================================
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//================================================
// �X�V����
//================================================
void CNumber::Update(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Size.x, m_pos.y - m_Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Size.x, m_pos.y - m_Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Size.x, m_pos.y + m_Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Size.x, m_pos.y + m_Size.y, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//================================================
// �`�揈��
//================================================
void CNumber::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���Ƃɖ߂��Ă���
	pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));

	// �v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // �v���~�e�B�u�̎��
}

//================================================
// �ʒu�̐ݒ菈��
//================================================
void CNumber::SetPos(const D3DXVECTOR3 pos)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;
	
	// �ʒu�̐ݒ�
	m_pos = pos;

	D3DXVECTOR2 Size = m_Size;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - Size.x, pos.y - Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + Size.x, pos.y - Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - Size.x, pos.y + Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + Size.x, pos.y + Size.y, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//================================================
// UV���W�̍X�V
//================================================
void CNumber::SetUV(const int nNum)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	float fPosTexU = 0.1f * nNum;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(fPosTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + fPosTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fPosTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + fPosTexU, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//================================================
// �F�̐ݒ�
//================================================
void CNumber::SetColor(const D3DXCOLOR col)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

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

//================================================
// �e�N�X�`����ID
//================================================
void CNumber::SetTextureID(const char* pFileName)
{
	// �e�N�X�`���̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	if (pFileName == NULL)
	{
		m_nTextureIdx = -1;
		return;
	}
	m_nTextureIdx = pTexture->Register(pFileName);
}
