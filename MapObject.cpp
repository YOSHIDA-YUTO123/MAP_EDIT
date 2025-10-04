//===================================================
//
// �}�b�v�ɔz�u����I�u�W�F�N�g�̃N���X [MapObject.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "MapObject.h"
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "math.h"
#include "imgui.h"
#include "input.h"

//===================================================
// �R���X�g���N�^
//===================================================
CMapObject::CMapObject()
{
	m_bMouseUp = true;
	m_fAlv = 1.0f;
	m_pos = Const::VEC3_NULL;
	m_rot = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nModelIdx = -1;
}

//===================================================
// �f�X�g���N�^
//===================================================
CMapObject::~CMapObject()
{
	m_aModelPath.clear();
}

//===================================================
// ��������
//===================================================
CMapObject* CMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const char *pModelFileName)
{
	CMapObject* pMapObject = new CMapObject;

	pMapObject->m_pos = pos;
	pMapObject->m_rot = rot;

	// �ԍ��̓o�^
	pMapObject->Register(pModelFileName);

	// ����������
	if (FAILED(pMapObject->Init()))
	{
		pMapObject->Uninit();
		pMapObject = nullptr;
		return nullptr;
	}

	return pMapObject;
}

//===================================================
// ����������
//===================================================
HRESULT CMapObject::Init(void)
{
	// ���f�����Ȃ��Ȃ�
	if (m_nModelIdx == -1)
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CMapObject::Uninit(void)
{
	// �I������
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CMapObject::Update(void)
{
}

//===================================================
// �`�揈��
//===================================================
void CMapObject::Draw(void)
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

		Mat.MatD3D.Diffuse.a = m_fAlv;

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
// ���f���̔ԍ��̓o�^
//===================================================
void CMapObject::Register(const char* pModelFileName)
{
	// ���f���N���X�̎擾
	CModelManager* pModel = CManager::GetModel();

	// �C���f�b�N�X�̓o�^
	m_nModelIdx = pModel->Register(pModelFileName);

	// ���f���̃p�X�̐ݒ�
	m_aModelPath = pModelFileName;
}

//===================================================
// �}�E�X�̓����蔻��
//===================================================
bool CMapObject::CollisionMouse(float* pDistance)
{
	BOOL hit = false;
	DWORD faceIndex;
	(*pDistance) = FLT_MAX;
	FLOAT baryU, baryV; // ���������|���S���̈ʒu

	// ���f���}�l�[�W���[�̎擾
	CModelManager *pModelManager = CManager::GetModel();

	// ���f�����Ȃ��Ȃ�
	if (m_nModelIdx == -1)
	{
		return false;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// ���C�̌��_
	D3DXVECTOR3 rayOrigin,rayDir;

	// ���C�̎擾
	math::GetMouseRay(&rayOrigin, &rayDir);

	// ���f���̃��[���h�s��
	D3DXMATRIX matWorld = m_mtxWorld;

	// �t�s��
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

	// ���C�����[�J����Ԃɕϊ�
	D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &matInvWorld);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &matInvWorld);
	D3DXVec3Normalize(&rayDir, &rayDir);

	// ���C�ƃ|���S���̓����蔻��
	D3DXIntersect(
		modelInfo.pMesh,
		&rayOrigin,
		&rayDir,
		&hit,
		&faceIndex,
		&baryU,
		&baryV,
		pDistance,
		nullptr,
		nullptr);
	
	if (hit)
	{
		return true;
	}
	
	return false;
}

//===================================================
// ���̐ݒ�
//===================================================
void CMapObject::SetInfo(void)
{
	// ���̐ݒ�
	ImGui::DragFloat3(u8"�ʒu", m_pos, 0.5f);

	D3DXVECTOR3 rot = D3DXToDegree(m_rot);

	// ���̐ݒ�
	if (ImGui::DragFloat3(u8"����", rot, 0.5f, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI)))
	{
		// �͈͓������[�v
		rot.x = Wrap(rot.x, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.y = Wrap(rot.y, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.z = Wrap(rot.z, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
	}
	
	
	// �p�x��ݒ�
	m_rot = D3DXToRadian(rot);
}

//===================================================
// �}�E�X�̃h���b�O�̐ݒ�
//===================================================
void CMapObject::SetMouseDrag(void)
{
	// ����
	float fDistance = 0.0f;

	// �}�E�X�̎擾
	CInputMouse* pMouse = CManager::GetInputMouse();

	// �}�E�X�Ƃ̓����蔻��
	if (CollisionMouse(&fDistance))
	{
		// ���N���b�N�����Ă�����
		if (pMouse->OnMousePress(CInputMouse::LEFT))
		{
			// �}�E�X�������Ă���
			m_bMouseUp = false;
		}
		else
		{
			// �}�E�X�������Ă��Ȃ�
			m_bMouseUp = true;
		}
	}
	// ���N���b�N�����Ă��Ȃ��Ȃ�
	else if (!pMouse->OnMousePress(CInputMouse::LEFT))
	{
		// �}�E�X�𗣂���
		m_bMouseUp = true;
	}

	// �}�E�X�𗣂��Ă��Ȃ��Ȃ�
	if (!m_bMouseUp)
	{
		// �ʒu�ƑO��̈ʒu�̎擾
		D3DXVECTOR2 MousePos = pMouse->GetVelocity();
		D3DXVECTOR2 MousePosOld = pMouse->GetOldVelocity();

		// �ړ��ʂ̐ݒ�
		D3DXVECTOR2 MouseMove = MousePos - MousePosOld;

		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXMATRIX view;
		pDevice->GetTransform(D3DTS_VIEW, &view);

		// �J�����̉E�x�N�g���Ə�x�N�g���𒊏o
		D3DXVECTOR3 camRight(view._11, view._21, view._31);
		D3DXVECTOR3 camUp(view._12, view._22, view._32);

		// �ړ��ʂ̐ݒ�
		D3DXVECTOR3 move = camRight * MouseMove.x + camUp * -MouseMove.y;

		// �ʒu�̍X�V
		m_pos += move;
	}
}
