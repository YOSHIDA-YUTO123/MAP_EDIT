//===================================================
//
// �}���`�e�N�X�`��2D�I�u�W�F�N�g�̃A�j���[�V���� [Object2DAnimMT.cpp]
// Author:YUTO YOSHIDA
// 
// << �}���`�e�N�X�`����2D�|���S���̃e�N�X�`��	>>
// << �A�j���[�V�����ɓ��������N���X			>>
// 
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "Object2DAnimMT.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include <string>

using namespace Const;	// ���O���Const���g�p
using namespace std;	// ���O���std���g�p

//===================================================
// �R���X�g���N�^
//===================================================
CObject2DAnimMT::CObject2DAnimMT() : CObject(7)
{
	m_pVtxBuffer = nullptr;
	m_pos = VEC3_NULL;
	m_Size = VEC2_NULL;
	m_nCounter = NULL;
	m_nHV = 1;
	m_nUV = 1;
	m_nPattern = NULL;
	m_nSpeed = NULL;
	m_bLoop = false;

	// �e�N�X�`���̐�����
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_nTextureIdx[nCnt] = -1;
	}
}

//===================================================
// �f�X�g���N�^
//===================================================
CObject2DAnimMT::~CObject2DAnimMT()
{
}

//===================================================
// ��������
//===================================================
CObject2DAnimMT* CObject2DAnimMT::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int UV, const int HV)
{
	CObject2DAnimMT* pObject2D = new CObject2DAnimMT;

	pObject2D->m_pos = pos;
	pObject2D->m_Size = Size;
	pObject2D->m_nUV = UV;
	pObject2D->m_nHV = HV;

	// ����������
	if (FAILED(pObject2D->Init()))
	{
		pObject2D->Uninit();
		pObject2D = nullptr;
		return nullptr;
	}

	return pObject2D;
}

//===================================================
// ����������
//===================================================
HRESULT CObject2DAnimMT::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_MULT) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2DMT,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_���̃|�C���^
	VERTEX_2D_MULT* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Size.x, m_pos.y - m_Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Size.x, m_pos.y - m_Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Size.x, m_pos.y + m_Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Size.x, m_pos.y + m_Size.y, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].texMT = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texMT = D3DXVECTOR2(1.0f / m_nUV, 0.0f);
	pVtx[2].texMT = D3DXVECTOR2(0.0f, 1.0f / m_nHV);
	pVtx[3].texMT = D3DXVECTOR2(1.0f / m_nUV, 1.0f / m_nHV);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CObject2DAnimMT::Uninit(void)
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
void CObject2DAnimMT::Update(void)
{
	// ���_���̃|�C���^
	VERTEX_2D_MULT* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Size.x, m_pos.y - m_Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Size.x, m_pos.y - m_Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Size.x, m_pos.y + m_Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Size.x, m_pos.y + m_Size.y, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	// �A�j���[�V�����̃J�E���^�[��i�߂�
	m_nCounter++;

	float fPosTexU = 1.0f / m_nUV;
	float fPosTexH = 1.0f / m_nHV;

	if (m_nCounter >= m_nSpeed)
	{
		m_nCounter = 0;

		m_nPattern++;

		// Y���W�̊���
		int nRatePosY = m_nPattern / m_nUV;

		float UV = fPosTexU * m_nPattern;
		float HV = nRatePosY * fPosTexH;

		// ���_�o�b�t�@�̃��b�N
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].texMT = D3DXVECTOR2(UV, HV);
		pVtx[1].texMT = D3DXVECTOR2(UV + fPosTexU, HV);
		pVtx[2].texMT = D3DXVECTOR2(UV, HV + fPosTexH);
		pVtx[3].texMT = D3DXVECTOR2(UV + fPosTexU, HV + fPosTexH);

		// ���_�o�b�t�@�̃A�����b�N
		m_pVtxBuffer->Unlock();

		// �ő�܂ōs������
		if (m_nPattern >= (m_nUV * m_nHV))
		{
			// �p�^�[����������
			m_nPattern = 0;

			// ���[�v���Ȃ��Ȃ�
			if (!m_bLoop)
			{
				// �j��
				Uninit();

				return;
			}
		}
	}
}

//===================================================
// �`�揈��
//===================================================
void CObject2DAnimMT::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D_MULT));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2DMT);

	// �e�N�X�`���̐�����
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// �e�N�X�`������������
		if (m_nTextureIdx[nCnt] != -1)
		{
			// �e�N�X�`����ݒ�
			pDevice->SetTexture(nCnt, pTexture->GetAdress(m_nTextureIdx[nCnt]));
		}
		else
		{
			// �e�N�X�`�����Ȃ�
			pDevice->SetTexture(nCnt, NULL);
		}
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // �v���~�e�B�u�̎��	

	// ���Ƃɖ߂�
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//===================================================
// �ݒ菈��
//===================================================
void CObject2DAnimMT::SetAnim(const int nSpeed, const bool bLoop)
{
	m_bLoop = bLoop;
	m_nSpeed = nSpeed;
}

//===================================================
// �ݒ菈��
//===================================================
void CObject2DAnimMT::SetAnim(const int nUV, const int nHV)
{
	m_nHV = nHV;
	m_nUV = nUV;
}

//===================================================
// �e�N�X�`����ID�̐ݒ�
//===================================================
void CObject2DAnimMT::SetTextureID(const char* pTextureName0, const char* pTextureName1)
{
	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTexture == nullptr) return;

	// �ȗ��p�p�X
	string filePath = "data/TEXTURE/";

	// ������̘A��
	filePath += pTextureName0;

	// ID�̐ݒ�
	m_nTextureIdx[0] = pTexture->Register(filePath.c_str());

	// �ȗ��p�p�X
	filePath = "data/TEXTURE/";

	// ������̘A��
	filePath += pTextureName1;

	// ID�̐ݒ�
	m_nTextureIdx[1] = pTexture->Register(filePath.c_str());
}
