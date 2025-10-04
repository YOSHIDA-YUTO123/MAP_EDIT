//===================================================
//
// �I�u�W�F�N�gX�t�@�C�� [objectX.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "objectX.h"
#include"manager.h"
#include"renderer.h"
#include"modelManager.h"
#include"textureManager.h"

using namespace Const;							// ���O���Const���g�p����

//===================================================
// �R���X�g���N�^
//===================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	memset(m_mtxWorld, NULL, sizeof(D3DXMATRIX));

	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_nModelIdx = -1;
	m_nTextureMT = -1;
}

//===================================================
// �f�X�g���N�^
//===================================================
CObjectX::~CObjectX()
{
}

//===================================================
// ��������
//===================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const char* pModelName,const D3DXVECTOR3 rot)
{
	CObjectX* pObjectX = nullptr;

	// 3D�I�u�W�F�N�g�̐���
	pObjectX = new CObjectX;

	if (pObjectX == nullptr) return nullptr;

	pObjectX->Init();
	pObjectX->m_pos = pos;
	pObjectX->m_rot = rot;
	pObjectX->LoadModel(pModelName);

	return pObjectX;
}

//===================================================
// ����������
//===================================================
HRESULT CObjectX::Init(void)
{
	return S_OK;
}

//===================================================
// �I������
//===================================================
void CObjectX::Uninit(void)
{	
	// �������g�̔j��
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CObjectX::Update(void)
{
}

//===================================================
// �`�揈��
//===================================================
void CObjectX::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// ���f���N���X�̎擾
	CModelManager* pModel = CManager::GetModel();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

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

	if (m_nModelIdx == -1)
	{
		//�ۑ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);

		return;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();
	
	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		if (m_nTextureMT != -1)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(1, pTexture->GetAdress(m_nTextureMT));
		}
		else
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(1, NULL);
		}

		//���f��(�p�[�c)�̕`��
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//===================================================
// �`�揈��(�I�[�o�[���[�h)�����x�ύX
//===================================================
void CObjectX::Draw(const float Diffuse)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

	if (m_nModelIdx == -1)
	{
		//�ۑ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);

		return;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL Mat = pMat[nCntMat];

		Mat.MatD3D.Diffuse.a = Diffuse;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&Mat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		// ���f��(�p�[�c)�̕`��
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//===================================================
// �`�揈��(�I�[�o�[���[�h)�F�ύX
//===================================================
void CObjectX::Draw(const D3DXCOLOR Diffuse)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

	if (m_nModelIdx == -1)
	{
		//�ۑ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);

		return;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL Mat = pMat[nCntMat];

		Mat.MatD3D.Diffuse = Diffuse;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&Mat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		// ���f��(�p�[�c)�̕`��
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//===================================================
// �e�̕`�揈��
//===================================================
void CObjectX::DrawShadow(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g�̕���
	D3DXVECTOR4 lightDir = D3DXVECTOR4(0.707f, -0.707f, 0.707f, 0.0f);

	// �e�𗎂Ƃ��n��
	D3DXPLANE plane = D3DXPLANE(0, -1.0f, 0, 0);

	// �e�p�̃}�g���b�N�X���쐬
	D3DXMATRIX mtxShadow, mtxShadowWorld;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	// �e�p�̃}�g���b�N�X�̍쐬
	D3DXMatrixShadow(&mtxShadow, &lightDir, &plane);

	// �e�̃}�g���b�N�X�Ǝ����̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&mtxShadowWorld, &m_mtxWorld, &mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadowWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �������̍��}�e���A�����Z�b�g
	D3DMATERIAL9 matShadow = {};

	// �e�̐F�̐ݒ�
	matShadow.Diffuse.r = 0.0f;
	matShadow.Diffuse.g = 0.0f;
	matShadow.Diffuse.b = 0.0f;
	matShadow.Diffuse.a = 1.0f;

	// ���f���̎擾
	CModelManager* pModel = CManager::GetModel();

	if (m_nModelIdx == -1)
	{
		//�ۑ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);

		return;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	// �e�̕`��
	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		// �e�̃}�e���A���̐ݒ�
		pDevice->SetMaterial(&matShadow);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �e�̕`��
		modelInfo.pMesh->DrawSubset(nCntMat);
	}
	
	// �}�e���A�������Ƃɖ߂�
	pDevice->SetMaterial(&matDef);
}

//===================================================
// ���[���h�}�g���b�N�X�̐ݒ�
//===================================================
void CObjectX::SetUpMatrix(const D3DXVECTOR3 Scal)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScal, Scal.x, Scal.y, Scal.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScal);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===================================================
// ���[���h�}�g���b�N�X�̐ݒ�
//===================================================
void CObjectX::SetUpMatrix(const D3DXMATRIX mtxRot, const D3DXMATRIX mtxTrans, const D3DXVECTOR3 Scal)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxScal;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScal, Scal.x, Scal.y, Scal.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScal);

	// �����𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===================================================
// �`��܂ł̐ݒ菈��
//===================================================
void CObjectX::SetUpDraw(const float Diffuse)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// ���f���N���X�̎擾
	CModelManager* pModel = CManager::GetModel();

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_nModelIdx == -1)
	{
		//�ۑ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);

		return;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL Mat = pMat[nCntMat];

		Mat.MatD3D.Diffuse.a = Diffuse;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&Mat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		// ���f��(�p�[�c)�̕`��
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//===================================================
// �}���`�e�N�X�`����ID�̐ݒ�
//===================================================
void CObjectX::SetTextureMT(const char* pTextureName)
{
	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTexture == nullptr) return;

	// ID�̎擾
	m_nTextureMT = pTexture->Register(pTextureName);
}

//===================================================
// ���f���̏��̎擾
//===================================================
const CModelManager::ModelInfo CObjectX::GetModelInfo(void) const
{
	// ���f���N���X�̎擾
	CModelManager* pModel = CManager::GetModel();

	if (m_nModelIdx == -1)
	{
		return CModelManager::ModelInfo();
	}

	return pModel->GetModelInfo(m_nModelIdx);

}

//===================================================
// ���f���̃��[�h����
//===================================================
HRESULT CObjectX::LoadModel(const char* pXFileName)
{
	// ���f���N���X�̎擾
	CModelManager* pModel = CManager::GetModel();

	// �C���f�b�N�X�̓o�^
	m_nModelIdx = pModel->Register(pXFileName);

	if (m_nModelIdx == -1)
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// �傫���̎擾
//===================================================
D3DXVECTOR3 CObjectX::GetSize(void)
{
	// ���f���N���X�̎擾
	CModelManager* pModel = CManager::GetModel();

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	if (m_nModelIdx == -1)
	{
		return VEC3_NULL;
	}
	// �傫���̎擾
	D3DXVECTOR3 Size = modelInfo.Size;

	// �傫����Ԃ�
	return Size;
}

