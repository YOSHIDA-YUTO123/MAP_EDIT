//===================================================
//
// �}�b�v�I�u�W�F�N�g�̃}�l�[�W���[ [MapObjectManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "MapObjectManager.h"
#include "imguimaneger.h"
#include "manager.h"
#include "renderer.h"
#include "MapObject.h"
#include "modelManager.h"
#include "input.h"
#include "EditMapObject.h"

//***************************************************
// �萔�錾
//***************************************************
constexpr int MAX_WORD = 256; // �ő�̕�����

//***************************************************
// �ÓI�����o�ϐ�
//***************************************************
std::unique_ptr< CMapObjectManager> CMapObjectManager::m_pInstance = nullptr;	// �����̃C���X�^���X
std::vector<CMapObject*> CMapObjectManager::m_pMapObjectList = {};				// �}�b�v�I�u�W�F�N�g�̃��X�g
std::vector<std::string> CMapObjectManager::m_aModelPath = {};					// ���f���̃p�X�̃��X�g

//===================================================
// �C���X�^���X�̎擾
//===================================================
CMapObjectManager* CMapObjectManager::GetInstance(void)
{
	// ��������������Ă��Ȃ�������
	if (m_pInstance == nullptr)
	{
		// �C���X�^���X�̐���
		m_pInstance.reset(new CMapObjectManager);
		m_pInstance->Init();
	}

	return m_pInstance.get();
}

//===================================================
// �}�b�v�I�u�W�F�N�g�̐�������
//===================================================
CMapObject* CMapObjectManager::Create(const D3DXVECTOR3 pos, const char* pModelFileName)
{
	// �}�b�v�I�u�W�F�N�g�̐���
	CMapObject* pMapObject = CMapObject::Create(pos, pModelFileName);

	if (pMapObject == nullptr)
	{
		MessageBox(NULL, "���f��������܂���", "���f����o�^���Ă�������", MB_OK | MB_ICONWARNING);
		return nullptr;
	}

	// ���X�g�ɒǉ�
	m_pMapObjectList.push_back(pMapObject);

	return pMapObject;
}

//===================================================
// ����������
//===================================================
HRESULT CMapObjectManager::Init(void)
{
	// ���f���̃}�l�[�W���[�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pModelManager == nullptr)
	{
		assert(false && "���f���}�l�[�W���[�擾���s");

		return E_FAIL;
	}

	// ���f���̏��̎擾
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(0);

	// �ҏW�p�}�b�v�I�u�W�F�N�g�̐���
	m_pEditMapObj = CEditMapObject::Create();

	//// �}�b�v�I�u�W�F�N�g�̐���
	//m_pSelect = CMapObjectManager::Create({ -500.0f,0.0f,0.0f }, modelInfo.filepath);

	return S_OK;
}

//===================================================
// �X�V����
//===================================================
void CMapObjectManager::Update(void)
{
	// Imgui�̎擾
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pImgui == nullptr) return;

	// �E�B���h�E�̐ݒ�
	pImgui->SetPosition(ImVec2(0.0f, 0.0f));
	pImgui->SetSize(ImVec2(300.0f, 200.0f));
	pImgui->Start("aaa", CImGuiManager::TYPE_NOMOVEANDSIZE);

	// ���f���̃p�X�̎擾
	GetModelPath();

	// ���f���̃p�X�̓o�^
	Register();

	// ���f���̃p�X�̃��X�g�̕\��
	SetModelPathList();

	if (ImGui::BeginTabBar("test0000"))
	{
		if (ImGui::BeginTabItem(u8"�z�u"))
		{
			if (m_pEditMapObj != nullptr)
			{
				// �\������
				m_pEditMapObj->SetShow(true);
			}

			// �ҏW�p�I�u�W�F�Ƃ̍X�V����
			UpdateEditMapObj();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�C���X�y�N�^�["))
		{
			if (m_pEditMapObj != nullptr)
			{
				// �\�����Ȃ�
				m_pEditMapObj->SetShow(false);
			}

			// �I�𒆂̃��f���̏���
			SetSelectObj();

			// ���f���̃C���X�y�N�^�[
			SetInspector();

			ImGui::EndTabItem();
		}

		// �^�u�o�[�̏I������
		ImGui::EndTabBar();
	}

	// �L�[�{�[�h�̎擾
	CInputMouse* pMouse = CManager::GetInputMouse();

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	ImGuiIO& io = ImGui::GetIO();
	
	// IMGUI�̃E�B���h�E�𑀍삵�Ă��Ȃ��Ȃ�
	if (!io.WantCaptureMouse && ImGui::IsMouseClicked(0))
	{
		// ���N���b�N����������&&���_��������Ă��Ȃ��Ȃ�
		if (pMouse->OnMouseTriggerDown(CInputMouse::LEFT) && !pKeyboard->GetPress(DIK_LALT))
		{
			// �}�E�X�Ƃ̓����蔻��
			CollisionMouse();
		}
	}

	// �I������
	pImgui->End();
}

//===================================================
// �R���X�g���N�^
//===================================================
CMapObjectManager::CMapObjectManager()
{
	m_pSelect = nullptr;
	m_nType = 0;
	m_pEditMapObj = nullptr;
}

//===================================================
// ���f���̃p�X�̎擾
//===================================================
void CMapObjectManager::GetModelPath(void)
{
	// ���f���̃}�l�[�W���[�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pModelManager == nullptr) return;

	// ���f���̑��������ׂ�
	for (auto itr = pModelManager->GetList().begin(); itr != pModelManager->GetList().end(); itr++)
	{
		// �t�@�C���p�X�̎擾
		std::string modelpath = itr->filepath;

		// �p�X�̃��X�g�𒲂ׂ�p
		for (auto pathList = m_aModelPath.begin(); pathList != m_aModelPath.end(); pathList++)
		{
			// ���łɓo�^�ς݂�������
			if (pathList->find(modelpath) != std::string::npos)
			{
				// �֐��𔲂���
				return;
			}
		}

		m_aModelPath.push_back(modelpath);
	}
}

//===================================================
// ���f���̃p�X�̃��X�g
//===================================================
void CMapObjectManager::SetModelPathList(void)
{
	if (m_aModelPath.empty()) return;

	if (ImGui::BeginCombo(u8"���f���̃��X�g", m_aModelPath[m_nType].c_str()))
	{
		int nCnt = 0;

		// �p�X�̃��X�g�𒲂ׂ�p
		for (auto pathList = m_aModelPath.begin(); pathList != m_aModelPath.end(); pathList++)
		{
			// �I�����Ă�����̂�
			const bool bSelect = m_nType == nCnt;

			if (ImGui::Selectable(pathList->c_str(), bSelect))
			{
				m_nType = nCnt;
			}

			if (bSelect)
			{
				ImGui::SetItemDefaultFocus();
			}

			nCnt++;
		}
		ImGui::EndCombo();
	}
}

//===================================================
// �I�𒆂̃��f���̏���
//===================================================
void CMapObjectManager::SetSelectObj(void)
{
	// �v�f�𒲂ׂ�
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// null�������珈�����΂�
		if ((*itr) == nullptr) continue;

		// �I�𒆂�������
		if (m_pSelect == (*itr) && m_pSelect != nullptr)
		{
			m_pSelect->SetALv(0.5f);
		}
		// �I������Ă��Ȃ��Ȃ�
		else
		{
			(*itr)->SetALv(1.0f);
		}
	}

	// �I������Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_pSelect == nullptr) return;

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ���_��������Ă��Ȃ��Ȃ�
	if (!pKeyboard->GetPress(DIK_LALT))
	{
		m_pSelect->SetMouseDrag();
	}
}

//===================================================
// �C���X�y�N�^�[�̐ݒ�
//===================================================
void CMapObjectManager::SetInspector(void)
{
	if (m_pSelect != nullptr)
	{
		m_pSelect->SetInfo();
	}
}

//===================================================
// �ҏW�p�̃}�b�v�I�u�W�F�N�g�̍X�V����
//===================================================
void CMapObjectManager::UpdateEditMapObj(void)
{
	// null�Ȃ珈�����Ȃ�
	if (m_pEditMapObj == nullptr) return;

	// �ҏW�p���f���̈ʒu�̎擾
	D3DXVECTOR3 pos = m_pEditMapObj->GetPosition();

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (ImGui::Button(u8"����") || pKeyboard->GetTrigger(DIK_RETURN))
	{
		// ���f���̐���
		Create(pos, m_aModelPath[m_nType].c_str());
	}

	// �ʒu�̏��
	ImGui::DragFloat3("", pos, 0.5f);

	// �ʒu�̐ݒ�
	m_pEditMapObj->SetPosition(pos);

	// ���f���̎�ނ̐ݒ�
	m_pEditMapObj->LoadModel(m_aModelPath[m_nType].c_str());
}

//===================================================
// �}�E�X�̓����蔻��
//===================================================
bool CMapObjectManager::CollisionMouse(void)
{
	// �����΂�߂�����
	float fDistanceNear = 0.0f;

	// �ŏ��ɑ���������ǂ���
	bool bFirst = false;
	bool bHit = false;

	// �v�f�𒲂ׂ�
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// null�������珈�����΂�
		if ((*itr) == nullptr) continue;

		float fDistance = 0.0f;

		// ����������
		if ((*itr)->CollisionMouse(&fDistance))
		{
			bHit = true;
		}
		// �ŏ��̑�����I����Ă��Ȃ��Ȃ�
		if (!bFirst)
		{
			// �߂������̐ݒ�
			fDistanceNear = fDistance;

			// �I�𒆂̃I�u�W�F�N�g�̃|�C���^�̐ݒ�
			m_pSelect = (*itr);

			bFirst = true;
		}
		else if(fDistanceNear > fDistance)
		{
			// �߂������̍X�V
			fDistanceNear = fDistance;

			// �I�𒆂̃I�u�W�F�N�g�̃|�C���^�̐ݒ�
			m_pSelect = (*itr);
		}
	}

	if (!bHit)
	{
		// ��x��������Ȃ��Ȃ�
		m_pSelect = nullptr;
	}

	return bHit;
}

//===================================================
// �}�b�v�I�u�W�F�N�g�̓o�^
//===================================================
HRESULT CMapObjectManager::Register(void)
{
	// ���f���̃}�l�[�W���[�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pModelManager == nullptr) return E_FAIL;

	// ������
	static char pModelName[MAX_WORD] = {};

	ImGui::SetNextItemWidth(200); // ���͗��̕����w��

	if (ImGui::InputText(u8"���f���̓o�^", pModelName, sizeof(pModelName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// �ȗ��p�p�X
		std::string modelPath = "data/MODEL/";
		std::string findPath = pModelName;

		// �܂܂�Ă�����
		if (findPath.find("data/MODEL/") != std::string::npos)
		{
			// �p�X�̐ݒ�
			modelPath = pModelName;
		}
		else
		{
			// �A������
			modelPath += pModelName;
		}

		// �p�X�̃��X�g�𒲂ׂ�
		for (auto pathList = m_aModelPath.begin(); pathList != m_aModelPath.end(); pathList++)
		{
			// ���łɓo�^�ς݂�������
			if (pathList->find(modelPath) != std::string::npos)
			{
				MessageBox(NULL, modelPath.c_str(), "���łɓo�^����Ă��܂�", MB_OK);
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//===================================================
// �f�X�g���N�^
//===================================================
CMapObjectManager::~CMapObjectManager()
{
}
