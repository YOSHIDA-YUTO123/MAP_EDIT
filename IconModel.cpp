//===================================================
//
// ���f���̃A�C�R���̍쐬 [IconModel.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "IconModel.h"
#include "MapObjectManager.h"
#include "MapObject.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "modelManager.h"
#include "textureManager.h"
#include "TextureMTManager.h"

//===================================================
// �`��̐ݒ菈��
//===================================================
void CIconModel::SetDraw(void)
{
}

//===================================================
// �R���X�g���N�^
//===================================================
CIconModel::CIconModel()
{
	m_pos = Const::VEC3_NULL;
	m_rot = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
}

//===================================================
// �f�X�g���N�^
//===================================================
CIconModel::~CIconModel()
{
}

//===================================================
// ����������
//===================================================
HRESULT CIconModel::Init(void)
{
	return S_OK;
}

//===================================================
// �I������
//===================================================
void CIconModel::Uninit(void)
{
	// �������g�̔j��
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CIconModel::Update(void)
{
}

//===================================================
// �`�揈��
//===================================================
void CIconModel::Draw(void)
{
	// �}�b�v�I�u�W�F�N�g�̃}�l�[�W���[�̐���
	CModelManager* pModelManager = CManager::GetModel();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pModelManager == nullptr) return;

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffer;
	D3DVIEWPORT9 viepowtDef;
	D3DXMATRIX mtxViewDef, mtxProjectionDef;
	
	// �}�b�v�I�u�W�F�N�g�̃��X�g�𒲂ׂ�
	for (auto& modelInfo : pModelManager->GetList())
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// ���݂̃����_�[�^�[�Q�b�g�̎擾
		pDevice->GetRenderTarget(0, &pRenderDef);

		// ���݂�Z�o�b�t�@�̎擾
		pDevice->GetDepthStencilSurface(&pZBuffer);

		// ���݂̃r���[�|�[�g�̎擾
		pDevice->GetViewport(&viepowtDef);

		// ���݂̃r���[�}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);

		// ���݂̃v���W�F�N�V�����}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

		D3DXVECTOR3 posV, posR, vecU;

		// �T�C�Y�̎擾
		D3DXVECTOR3 modelSize = modelInfo.Size;

		float fRadius = D3DXVec3Length(&modelSize);

		posV = D3DXVECTOR3(0.0f, fRadius, fRadius);

		posR = D3DXVECTOR3(0.0f, fRadius * 0.25f, 0.0f);

		vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �����_�����O�^�[�Q�b�g�̕ύX
		CManager::GetTextureMT()->ChangeTarget(posV, posR, vecU, modelInfo.filepath);

		// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);

		// �e�N�X�`���N���X�̎擾
		CTextureManager* pTexture = CManager::GetTexture();

		// ���f���N���X�̎擾
		CModelManager* pModel = CManager::GetModel();

		//�v�Z�p�̃}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

		D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

		D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

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

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

			// ���f��(�p�[�c)�̕`��
			modelInfo.pMesh->DrawSubset(nCntMat);
		}

		// �����_�[�^�[�Q�b�g�����Ƃɖ߂�
		pDevice->SetRenderTarget(0, pRenderDef);

		// Z�o�b�t�@�����Ƃɖ߂�
		pDevice->SetDepthStencilSurface(pZBuffer);

		pRenderDef->Release();
		pZBuffer->Release();

		// �r���[�|�[�g�����Ƃɖ߂�
		pDevice->SetViewport(&viepowtDef);

		pDevice->SetTransform(D3DTS_VIEW, &mtxViewDef);

		pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

		//�ۑ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}
