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
#include "json.hpp"
#include <fstream>
#include "math.h"
#include "imgui_internal.h"
#include "DebugLog.h"
#include "camera.h"

// json�̎g�p
using json = nlohmann::json;

//***************************************************
// �萔�錾
//***************************************************
constexpr int MAX_WORD = 256; // �ő�̕�����
const char* JSON_FILE = "data/stage000.json"; // �g�p����json�t�@�C��

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
CMapObject* CMapObjectManager::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName)
{
	// �}�b�v�I�u�W�F�N�g�̐���
	CMapObject* pMapObject = CMapObject::Create(pos, rot, pModelFileName);

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
	pImgui->SetSize(ImVec2(400.0f, 500.0f));
	pImgui->Start("aaa", CImGuiManager::TYPE_NOMOVEANDSIZE);

	// �L�[�{�[�h�̎擾
	CInputMouse* pMouse = CManager::GetInputMouse();

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ���f���̃p�X�̎擾
	GetModelPath();

	// ���f���̃p�X�̓o�^
	Register();

	// �t�@�C���p�X�̐ݒ�
	SetFilePath();

	// ���f���̃p�X�̃��X�g�̕\��
	SetModelPathList();

	// �z�u���[�h���ǂ���
	static bool bSetMode = true;

	SetCamerafocus(bSetMode);

	if (ImGui::RadioButton(u8"�I�u�W�F�N�g�𕽖ʂňړ�������", m_bDragMoveXZ))
	{
		// ��Ԃ̐؂�ւ�
		m_bDragMoveXZ = m_bDragMoveXZ ? false : true;
	}

	if (ImGui::BeginTabBar("test0000"))
	{
		if (ImGui::BeginTabItem(u8"�z�u"))
		{
			bSetMode = true;

			if (m_pEditMapObj != nullptr)
			{
				// �\������
				m_pEditMapObj->SetShow(true);
			}

			// �I�����Ȃ�
			m_pSelect = nullptr;

			// �ҏW�p�I�u�W�F�Ƃ̍X�V����
			UpdateEditMapObj();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�z�u�I�u�W�F�N�g�ݒ�"))
		{
			bSetMode = false;
			if (m_pEditMapObj != nullptr)
			{
				// �\�����Ȃ�
				m_pEditMapObj->SetShow(false);
			}

			// ���f���̃C���X�y�N�^�[
			SetInspector();

			// �I�𒆂̃I�u�W�F�N�g�̏���
			UpdateSelectObj();

			ImGui::EndTabItem();
		}

		// �^�u�o�[�̏I������
		ImGui::EndTabBar();
	}

	// �I�𒆂̃��f���̏���
	SetSelectObjAlv();
	
	// IMGUI�̃E�B���h�E�𑀍삵�Ă��Ȃ��Ȃ�
	if (!pImgui->GetActiveWindow())
	{
		// ���N���b�N����������&&���_��������Ă��Ȃ��Ȃ�
		if (pMouse->OnMouseTriggerDown(CInputMouse::LEFT) && !pKeyboard->GetPress(DIK_LALT))
		{
			// �}�E�X�Ƃ̓����蔻��
			CollisionMouse();
		}
	}
	
	if (ImGui::Button(u8"�Z�[�u����", ImVec2(300.0, 0)))
	{
		int nID = MessageBox(NULL, "�z�u���̃Z�[�u", "�Z�[�u���܂��� ?", MB_YESNO);

		if (nID == IDYES)
		{
			// �f�o�b�O���O�̎擾
			CDebugLog *pDebugLog = CManager::GetDebugLog();

			if (pDebugLog != nullptr)
			{
				pDebugLog->Set(u8"�Z�[�u����!!!", 120);
			}
			// �Z�[�u
			Save();
		}
	}

	if (ImGui::Button(u8"���[�h", ImVec2(300.0, 0)))
	{
		int nID = MessageBox(NULL, "�X�e�[�W�̃��[�h", "���[�h���܂��� ?", MB_YESNO);

		if (nID == IDYES)
		{
			// �f�o�b�O���O�̎擾
			CDebugLog* pDebugLog = CManager::GetDebugLog();

			if (pDebugLog != nullptr)
			{
				pDebugLog->Set(u8"���[�h����!!!", 120);
			}

			// ���[�h
			Load();
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
	m_fMove = MOVE_VALUE;
	m_bDragMoveXZ = false;
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

	if (ImGui::BeginCombo(u8"���f���̎��", m_aModelPath[m_nType].c_str()))
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
void CMapObjectManager::SetSelectObjAlv(void)
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
}

//===================================================
// �C���X�y�N�^�[�̐ݒ�
//===================================================
void CMapObjectManager::SetInspector(void)
{
	if (m_pSelect != nullptr)
	{
		// ���̐ݒ�
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

	// �v�f���Ȃ��Ȃ珈�����Ȃ�
	if (m_aModelPath.empty()) return;

	m_pEditMapObj->SetMove(m_fMove);
	m_pEditMapObj->UpdateEdit();

	// �ҏW�p���f���̈ʒu�̎擾
	D3DXVECTOR3 pos = m_pEditMapObj->GetPosition();

	// �ҏW�p���f���̌����̎擾
	D3DXVECTOR3 rot = D3DXToDegree(m_pEditMapObj->GetRotation());

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �ʒu�̏��
	ImGui::DragFloat3(u8"�ʒu", pos, 0.5f);

	// ���̐ݒ�
	if (ImGui::DragFloat3(u8"����", rot, 0.5f, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI)))
	{
		// �͈͓������[�v
		rot.x = Wrap(rot.x, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.y = Wrap(rot.y, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.z = Wrap(rot.z, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
	}

	if (ImGui::Button(u8"�I�u�W�F�N�g�𐶐�",ImVec2(300.0,0)) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		// ���f���̐���
		Create(pos, D3DXToRadian(rot), m_aModelPath[m_nType].c_str());
	}

	// �ʒu�̐ݒ�
	m_pEditMapObj->SetPosition(pos);

	// �����̐ݒ�
	m_pEditMapObj->SetRotation(D3DXToRadian(rot));

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

		m_aModelPath.push_back(modelPath);
	}

	return S_OK;
}

//===================================================
// �I�𒆂̃I�u�W�F�N�g�̍X�V����
//===================================================
void CMapObjectManager::UpdateSelectObj(void)
{
	// �I������Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_pSelect == nullptr) return;

	// �v�f���Ȃ��Ȃ珈�����Ȃ�
	if (m_aModelPath.empty()) return;

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// Imgui�̎擾����
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// ���_��������Ă��Ȃ��Ȃ�
	if (!pKeyboard->GetPress(DIK_LALT) && m_pSelect != nullptr)
	{
		if (pImgui != nullptr && !pImgui->GetActiveWindow())
		{
			// �}�E�X�̃h���b�O����
			m_pSelect->SetMouseDrag();
		}
	}

	// �ړ��̐ݒ�
	m_pSelect->SetMove(m_fMove);
	m_pSelect->UpdateMove();

	if (ImGui::Button(u8"��ނ̓K��", ImVec2(300.0, 0)))
	{
		// ��ނ̕ύX
		m_pSelect->Register(m_aModelPath[m_nType].c_str());
	}

	if (ImGui::Button(u8"�����̃��Z�b�g", ImVec2(300.0, 0)))
	{
		// �����̐ݒ�
		m_pSelect->SetRotation({ 0.0f,0.0f,0.0f });
	}

	if (pKeyboard->GetTrigger(DIK_DELETE) || ImGui::Button(u8"����", ImVec2(300.0, 0)))
	{
		// ���X�g�������
		Erase(m_pSelect);
		m_pSelect = nullptr;
	}
}

//===================================================
// �Z�[�u����
//===================================================
void CMapObjectManager::Save(void)
{
	json config;

	// �v�f�𒲂ׂ�
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// null�������珈�����Ȃ�
		if ((*itr) == nullptr) continue;

		// �ʒu�̎擾
		D3DXVECTOR3 pos = (*itr)->GetPosition();

		// �����̎擾
		D3DXVECTOR3 rot = (*itr)->GetRotation();

		json obj =
		{
			{"file_path",(*itr)->GetPath()},
			{"position",{{"x",pos.x},{"y",pos.y},{"z",pos.z}}},
			{"rotation",{{"x",rot.x},{"y",rot.y},{"z",rot.z}}}
		};

		config["MODEL_INFO"].push_back(obj);
	}

	// �t�@�C�����J��
	std::ofstream file(JSON_FILE);

	// �t�@�C�����J������
	if (file.is_open())
	{
		// �t�@�C���ɏ����o��
		file << config.dump(4);

		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "�G���[", "�t�@�C�����J���܂���ł���", MB_OK);
	}
}

//===================================================
// ���[�h����
//===================================================
void CMapObjectManager::Load(void)
{
	json config;

	// �t�@�C�����J��
	std::ifstream file(JSON_FILE);

	if (file.is_open())
	{
		file >> config;
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "�G���[", "�t�@�C�����J���܂���ł���", MB_OK);
		return;
	}

	// �v�f�����ׂ�
	for (auto& obj : config["MODEL_INFO"])
	{
		std::string filepath = obj["file_path"];
		float posX = obj["position"]["x"];
		float posY = obj["position"]["y"];
		float posZ = obj["position"]["z"];

		float rotX = obj["rotation"]["x"];
		float rotY = obj["rotation"]["y"];
		float rotZ = obj["rotation"]["z"];

		// ���f���̐���
		Create(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(rotX, rotY, rotZ), filepath.c_str());
	}
}

//===================================================
// ���X�g�̘A���̉���
//===================================================
void CMapObjectManager::Erase(CMapObject* pObj)
{
	// �v�f�𒲂ׂ�
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// null�������珈�����Ȃ�
		if ((*itr) == nullptr) continue;

		if ((*itr) == pObj)
		{
			// �I������
			(*itr)->Uninit();

			// �Ԃ𖄂߂�
			itr = m_pMapObjectList.erase(itr);

			pObj = nullptr;
		}

		// �Ō���������甲����
		if (itr == m_pMapObjectList.end()) break;
	}
}

//===================================================
// �t�@�C���p�X�̐ݒ菈��
//===================================================
void CMapObjectManager::SetFilePath(void)
{
	// ���f���̃}�l�[�W���[�̎擾
	CModelManager* pModelManager = CManager::GetModel();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pModelManager == nullptr) return;
	
	for (auto& modellist : pModelManager->GetList())
	{
		// ���łɃ��X�g�ɓo�^����Ă��邩���ׂ�
		auto it = std::find(m_aModelPath.begin(), m_aModelPath.end(), modellist.filepath);

		// ������Ȃ��Ȃ�
		if (it == m_aModelPath.end())
		{
			m_aModelPath.push_back(modellist.filepath);
		}
	}
}

//===================================================
// �J�����̃t�H�[�J�X
//===================================================
void CMapObjectManager::SetCamerafocus(const bool bMode)
{
	if (ImGui::Button(u8"�J������I�����f���Ƀt�H�[�J�X"))
	{
		D3DXVECTOR3 selectPos = { 0.0f,0.0f,0.0f };

		if (m_pSelect != nullptr)
		{
			// �ʒu�̎擾
			selectPos = m_pSelect->GetPosition();
		}

		D3DXVECTOR3 gostPos = m_pEditMapObj->GetPosition();

		D3DXVECTOR3 pos = bMode ? gostPos : selectPos;

		// �J�����̎擾
		CCamera* pCamera = CManager::GetCamera();

		// null�������珈�����Ȃ�
		if (pCamera == nullptr)return;

		pCamera->SetPosV(pos);
		pCamera->SetPosR(pos);
		pCamera->UpdatePositionV();
		pCamera->UpdatePositionR();
	}
}

//===================================================
// �f�X�g���N�^
//===================================================
CMapObjectManager::~CMapObjectManager()
{
}
