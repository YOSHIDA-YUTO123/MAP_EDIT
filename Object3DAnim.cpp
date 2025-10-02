//===================================================
//
// �A�j���[�V����3D�|���S�� [Object3DAnim.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "Object3DAnim.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include <string>

using namespace Const;	// ���O���Const���g�p
using namespace std;	// ���O���std���g�p

//===================================================
// �R���X�g���N�^
//===================================================
CObject3DAnim::CObject3DAnim(const int nPriority) : CObject(nPriority)
{
	m_pVtxBuffer = nullptr;			 // ���_�o�b�t�@�ւ̃|�C���^
	D3DXMatrixIdentity(&m_mtxWorld); // ���[���h�}�g���b�N�X
	m_rot = VEC3_NULL;				 // ����
	m_pos = VEC3_NULL;				 // �ʒu
	m_Size = VEC3_NULL;				 // �傫��
	m_nSpeed = NULL;				 // �A�j���[�V�����̃X�s�[�h
	m_nCounter = NULL;				 // �J�E���^�[
	m_nPattern = NULL;				 // �p�^�[��
	m_nSegX = 1;					 // ���̕�����
	m_nSegY = 1;					 // �c�̕�����
	m_nTextureIdx = -1;				 // �e�N�X�`����ID
	m_bLoop = false;				 // ���[�v���邩�ǂ���
}

//===================================================
// �f�X�g���N�^
//===================================================
CObject3DAnim::~CObject3DAnim()
{
}

//===================================================
// ��������
//===================================================
CObject3DAnim* CObject3DAnim::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 Size, const int nSegX, const int nSegY, const int nSpeed)
{
	CObject3DAnim* pObject3D = new CObject3DAnim;

	pObject3D->SetPosition(pos);
	pObject3D->SetSize(Size);
	pObject3D->SetParam(nSegX, nSegY, nSpeed,true);

	// ����������
	if (FAILED(pObject3D->Init()))
	{
		pObject3D->Uninit();
		pObject3D = nullptr;
		return nullptr;
	}

	return pObject3D;
}

//===================================================
// ����������
//===================================================
HRESULT CObject3DAnim::Init(void)
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
	pVtx[0].col = WHITE;
	pVtx[1].col = WHITE;
	pVtx[2].col = WHITE;
	pVtx[3].col = WHITE;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / m_nSegX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / m_nSegY);
	pVtx[3].tex = D3DXVECTOR2(1.0f / m_nSegX, 1.0f / m_nSegY);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CObject3DAnim::Uninit(void)
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
void CObject3DAnim::Update(void)
{
	// �A�j���[�V�����̃J�E���^�[��i�߂�
	m_nCounter++;

	float fPosTexU = 1.0f / m_nSegX;
	float fPosTexH = 1.0f / m_nSegY;

	if (m_nCounter >= m_nSpeed)
	{
		m_nCounter = 0;

		m_nPattern++;

		// Y���W�̊���
		int nRatePosY = m_nPattern / m_nSegX;

		float UV = fPosTexU * m_nPattern;
		float HV = nRatePosY * fPosTexH;

		// ���_���̃|�C���^
		VERTEX_3D* pVtx;

		// ���_�o�b�t�@�̃��b�N
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(UV, HV);
		pVtx[1].tex = D3DXVECTOR2(UV + fPosTexU, HV);
		pVtx[2].tex = D3DXVECTOR2(UV, HV + fPosTexH);
		pVtx[3].tex = D3DXVECTOR2(UV + fPosTexU, HV + fPosTexH);

		// ���_�o�b�t�@�̃A�����b�N
		m_pVtxBuffer->Unlock();

		// �ő�܂ōs������
		if (m_nPattern >= (m_nSegX * m_nSegY))
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
void CObject3DAnim::Draw(void)
{
	// �����_���[�̎擾
	CRenderer* pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

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
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

#if 0
	LPDIRECT3DTEXTURE9 pTextureMT = pRenderer->GetTextureMT();

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTextureMT);
#else
	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));
#endif

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===================================================
// �e�N�X�`����ID�̐ݒ�
//===================================================
void CObject3DAnim::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �ȗ��p�p�X
	string filePath = "data/TEXTURE/";

	// ������̘A��
	filePath += pTextureName;

	// ID�̐ݒ�
	m_nTextureIdx = pTexture->Register(filePath.c_str());
}

//===================================================
// �p�����[�^�̐ݒ�
//===================================================
void CObject3DAnim::SetParam(const int nSegX, const int nSegY, const int nSpeed, const bool bLoop)
{
	m_nSegX = nSegX;
	m_nSegY = nSegY;
	m_nSpeed = nSpeed;
	m_bLoop = bLoop;
}
