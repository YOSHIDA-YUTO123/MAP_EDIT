//===================================================
//
// �����_�[�^�[�Q�b�g�p�e�N�X�`�����Ǘ�����}�l�[�W���[ [TextureMTManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "TextureMTManager.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// �R���X�g���N�^
//===================================================
CTextureMTManager::CTextureMTManager()
{
}

//===================================================
// �f�X�g���N�^
//===================================================
CTextureMTManager::~CTextureMTManager()
{
}

//===================================================
// �I������
//===================================================
void CTextureMTManager::Uninit(void)
{
	// �v�f������
	for (auto& list : m_aInfo)
	{
		// �����_�[�^�[�Q�b�g�p�C���^�[�t�F�[�X�̔j��
		if (list.pRenderMT != nullptr)
		{
			list.pRenderMT->Release();
			list.pRenderMT = nullptr;
		}

		// �����_�[�^�[�Q�b�g�p�e�N�X�`���̔j��
		if (list.pTextureMT != nullptr)
		{
			list.pTextureMT->Release();
			list.pTextureMT = nullptr;
		}
		
		// Z�o�b�t�@�̔j��
		if (list.pZBuffMT != nullptr)
		{
			list.pZBuffMT->Release();
			list.pZBuffMT = nullptr;
		}

		// ���O�̔j��
		list.aName.clear();
	}

	// �R���e�i�̔j��
	m_aInfo.clear();
}

//===================================================
// �A�h���X�̎擾
//===================================================
LPDIRECT3DTEXTURE9 CTextureMTManager::GetAddress(const char* pName)
{
	// �v�f�������ׂ�
	for (auto& list : m_aInfo)
	{
		// ���łɍ쐬�ς݂�������
		if (list.aName == pName)
		{
			return list.pTextureMT;
		}
	}

	// ��������
	CreateRender(pName);

	// �v�f�������ׂ�
	for (auto& list : m_aInfo)
	{
		// ���łɍ쐬�ς݂�������
		if (list.aName == pName)
		{
			return list.pTextureMT;
		}
	}

	return nullptr;
}

//===================================================
// �����_�[�^�[�Q�b�g�̕ύX
//===================================================
void CTextureMTManager::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU,const char *pName)
{
	if (m_aInfo.empty())
	{
		GetAddress(pName);
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (auto& list : m_aInfo)
	{
		if (list.aName == pName)
		{
			D3DXMATRIX mtxView, mtxProjection; // �r���[�}�g���b�N�X�A�v���W�F�N�V�����}�g���b�N�X
			float fAspect;

			// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
			pDevice->SetRenderTarget(0, list.pRenderMT);

			// ��������Z�o�b�t�@�ɐݒ�
			pDevice->SetDepthStencilSurface(list.pZBuffMT);

			// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐ݒ�
			pDevice->SetViewport(&list.viewport);

			// �r���[�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxView);

			// �v���W�F�N�V�����}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxProjection);

			// �r���[�}�g���b�N�X�̍쐬
			D3DXMatrixLookAtLH(
				&mtxView,
				&posV,
				&posR,
				&vecU);

			// �r���[�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_VIEW, &mtxView);

			fAspect = (float)list.viewport.Width / (float)list.viewport.Height;

			// �v���W�F�N�V�����}�g���b�N�X�̍쐬
			D3DXMatrixPerspectiveFovLH(&mtxProjection,
				D3DXToRadian(45.0f),
				fAspect,
				1.0f,
				100000.0f);

			// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
		}
		else
		{
			GetAddress(pName);
		}
	}

	// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);
}

//===================================================
// ��������
//===================================================
void CTextureMTManager::CreateRender(const char *pName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	LPDIRECT3DSURFACE9 pRenderDef, pZBufferDef;

	CTextureMTManager::Info info = {};

	// �����_���[�^�[�Q�b�g�p�e�N�X�`���̐���
	pDevice->CreateTexture(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&info.pTextureMT,
		NULL);

	// �e�N�X�`���̃����_�����O�^�[�Q�b�g�p�C���^�[�t�F�[�X�̐���
	info.pTextureMT->GetSurfaceLevel(0, &info.pRenderMT);

	// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
	pDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&info.pZBuffMT,
		NULL);

	// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	pDevice->GetRenderTarget(0, &pRenderDef);

	// ���݂�Z�o�b�t�@�̎擾(�ۑ�)
	pDevice->GetDepthStencilSurface(&pZBufferDef);

	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	pDevice->SetRenderTarget(0, info.pRenderMT);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�̐ݒ�
	pDevice->SetDepthStencilSurface(info.pZBuffMT);

	// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// �����_�[�^�[�Q�b�g�����Ƃɖ߂�
	pDevice->SetRenderTarget(0, pRenderDef);

	// Z�o�b�t�@�����Ƃɖ߂�
	pDevice->SetDepthStencilSurface(pZBufferDef);
	
	// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐���
	info.viewport.X = 0;
	info.viewport.Y = 0;
	info.viewport.Width = SCREEN_WIDTH;
	info.viewport.Height = SCREEN_HEIGHT;
	info.viewport.MinZ = 0.0f;
	info.viewport.MaxZ = 1.0f;

	info.aName = pName;
	m_aInfo.push_back(info);

	info.pTextureMT->Release();
	pRenderDef->Release();
	pZBufferDef->Release();
}
