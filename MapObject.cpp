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
#include "MapObjectManager.h"
#include "camera.h"
#include "meshfield.h"
#include "transform.h"
#include "MapObjectList.h"
#include "Collider.h"

//***************************************************
// �萔�錾
//***************************************************
namespace MAPOBJ_INFO
{
	// �I�u�W�F�N�g�̏��
	const char* TYPE[CMapObject::TYPE_MAX] =
	{
		"static",
		"collision"
	};
};

//===================================================
// �R���X�g���N�^
//===================================================
CMapObject::CMapObject()
{
	m_fMove = MOVE_VALUE;
	m_bMouseUp = true;
	m_fAlv = 1.0f;
	m_nModelIdx = -1;
	m_aType = MAPOBJ_INFO::TYPE[CMapObject::TYPE_STATIC];
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
CMapObject* CMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const char *pModelFileName,const char *pType)
{
	CMapObject* pMapObject = new CMapObject;

	// �ԍ��̓o�^
	pMapObject->Register(pModelFileName);
	pMapObject->m_aType = pType;

	// ����������
	if (FAILED(pMapObject->Init()))
	{
		pMapObject->Uninit();
		pMapObject = nullptr;
		return nullptr;
	}

	// ��ԏ��̎擾
	CTransform* pTransform = pMapObject->GetTransform();

	if (pTransform == nullptr)
	{
		assert(false && "��ԏ�񖳂�");
	}

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	info.pos = pos;
	info.rot = rot;
	
	// ���̐ݒ�
	pTransform->SetInfo(info);

	return pMapObject;
}

//===================================================
// ����������
//===================================================
HRESULT CMapObject::Init(void)
{
	// ����������
	if (FAILED(CCollisionObject3D::Init()))
	{
		return E_FAIL;
	}

	// ���f�����Ȃ��Ȃ�
	if (m_nModelIdx == -1)
	{
		return E_FAIL;
	}

	// ���f���}�l�[�W���[�N���X�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// ��ԏ��̎擾
	CTransform *pTransform = CCollisionObject3D::GetTransform();

	CTransform::Info transform = pTransform->GetInfo();

	transform.Size = modelInfo.Size;

	pTransform->SetInfo(transform);

	// �R���C�_�[�̒ǉ�
	CCollisionObject3D::AddCollider(CColliderOBB::Create(), "mapObject");

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CMapObject::Uninit(void)
{
	// �I������
	CCollisionObject3D::Uninit();
}

//===================================================
// �X�V����
//===================================================
void CMapObject::Update(void)
{
	// ���b�V���t�B�[���h�̎擾
	CMeshField* pMeshField = CManager::GetMeshField();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pMeshField == nullptr) return;

	float fHeight = 0.0f;

	// �X�V����
	CCollisionObject3D::Update();

	// ��ԏ��̎擾
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// null�������珈�����΂�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �t�B�[���h�̓����蔻��
	if (pMeshField->Collision(info.pos, &fHeight))
	{
		info.pos.y = fHeight;
	}

	if (m_nModelIdx == -1) return;

	// ���f���}�l�[�W���[�N���X�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	info.Size = modelInfo.Size;

	// ��ԏ��̐ݒ�
	pTransform->SetInfo(info);
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

	// �X�V����
	CCollisionObject3D::Draw();

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
// �ړ�����
//===================================================
void CMapObject::UpdateMove(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// ��ԏ��̎擾
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// null�������珈�����΂�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �J�������擾�o���Ȃ������珈�����Ȃ�
	if (pCamera == nullptr) return;

	// �J�����̊p�x�̎擾
	float fCameraAngle = pCamera->GetRotaition().y;

	if (pKeyboard->GetPress(DIK_LEFT))
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			info.pos.x += sinf(fCameraAngle - D3DX_PI * 0.25f) * m_fMove;
			info.pos.z += cosf(fCameraAngle - D3DX_PI * 0.25f) * m_fMove;
		}
		else if (pKeyboard->GetPress(DIK_DOWN))
		{
			info.pos.x += sinf(fCameraAngle - D3DX_PI * 0.75f) * m_fMove;
			info.pos.z += cosf(fCameraAngle - D3DX_PI * 0.75f) * m_fMove;
		}
		else
		{
			info.pos.x += sinf(fCameraAngle - D3DX_PI * 0.5f) * m_fMove;
			info.pos.z += cosf(fCameraAngle - D3DX_PI * 0.5f) * m_fMove;
		}
	}
	else if (pKeyboard->GetPress(DIK_RIGHT))
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			info.pos.x += sinf(fCameraAngle + D3DX_PI * 0.25f) * m_fMove;
			info.pos.z += cosf(fCameraAngle + D3DX_PI * 0.25f) * m_fMove;
		}
		else if (pKeyboard->GetPress(DIK_DOWN))
		{
			info.pos.x += sinf(fCameraAngle + D3DX_PI * 0.75f) * m_fMove;
			info.pos.z += cosf(fCameraAngle + D3DX_PI * 0.75f) * m_fMove;
		}
		else
		{
			info.pos.x += sinf(fCameraAngle + D3DX_PI * 0.5f) * m_fMove;
			info.pos.z += cosf(fCameraAngle + D3DX_PI * 0.5f) * m_fMove;
		}
	}
	else if (pKeyboard->GetPress(DIK_UP))
	{
		info.pos.x += sinf(fCameraAngle) * m_fMove;
		info.pos.z += cosf(fCameraAngle) * m_fMove;
	}
	else if (pKeyboard->GetPress(DIK_DOWN))
	{
		info.pos.x += sinf(fCameraAngle + D3DX_PI) * m_fMove;
		info.pos.z += cosf(fCameraAngle + D3DX_PI) * m_fMove;
	}

	// ���̐ݒ�
	pTransform->SetInfo(info);
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

	// ��ԏ��̎擾
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// null�������珈�����΂�
	if (pTransform == nullptr) return false;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// ���f���̃��[���h�s��
	D3DXMATRIX matWorld = info.mtxWorld;

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
	// ��ԏ��̎擾
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// null�������珈�����΂�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// ���̐ݒ�
	ImGui::DragFloat3(u8"�ʒu", info.pos, 0.5f);

	D3DXVECTOR3 rot = D3DXToDegree(info.rot);

	// ���̐ݒ�
	if (ImGui::DragFloat3(u8"����", rot, 0.5f, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI)))
	{
		// �͈͓������[�v
		rot.x = Wrap(rot.x, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.y = Wrap(rot.y, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.z = Wrap(rot.z, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
	}
	
	static int nSelect = 0;

	if (ImGui::BeginCombo(u8"###state", m_aType.c_str()))
	{
		int nCnt = 0;

		for (auto& list : MAPOBJ_INFO::TYPE)
		{
			bool bSelect = (nSelect == nCnt);

			if (ImGui::Selectable(list, bSelect)) // ���ڂ��N���b�N�����Ƃ�
			{
				nSelect = nCnt; // �I�����X�V
				m_aType = list;
			}
			if (bSelect)
			{
				ImGui::SetItemDefaultFocus(); // �f�t�H���g�t�H�[�J�X�ݒ�i���F������j
			}
			nCnt++;
		}
		ImGui::EndCombo();
	}

	// �p�x��ݒ�
	info.rot = D3DXToRadian(rot);

	// ���̐ݒ�
	pTransform->SetInfo(info);
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

	// ��ԏ��̎擾
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// null�������珈�����΂�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

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

		D3DXVECTOR3 move;

		// �h���b�O�̏�Ԃ����ʈړ���������
		if (CMapObjectManager::GetInstance()->GetDragMoveState())
		{
			// XZ���ʂł̈ړ�
			camUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ������͖�����
			D3DXVECTOR3 forward = D3DXVECTOR3(view._13, 0.0f, view._33);
			D3DXVec3Normalize(&forward, &forward);

			move = camRight * MouseMove.x + forward * -MouseMove.y;
		}
		else
		{
			// �ʏ�̃J������ړ�
			move = camRight * MouseMove.x + camUp * -MouseMove.y;
		}
		// �ʒu�̍X�V
		info.pos += move;
	}

	// ���̐ݒ�
	pTransform->SetInfo(info);
}
