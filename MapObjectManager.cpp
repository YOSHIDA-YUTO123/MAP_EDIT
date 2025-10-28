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
#include "textureManager.h"
#include "IconModel.h"
#include "object3D.h"
#include "TextureMTManager.h"
#include "transform.h"
#include"MapObjectList.h"

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
CMapObject* CMapObjectManager::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName, const char* pType)
{
	// �}�b�v�I�u�W�F�N�g�̐���
	CMapObject* pMapObject = CMapObject::Create(pos, rot, pModelFileName, pType);

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

	// �A�C�R���̐���
	CIconModel::Create();

	LoadFileName();

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

	// �J�����̃t�H�[�J�X����
	SetCamerafocus();

	CopyAndPaste(pKeyboard);

	if (ImGui::Checkbox(u8"�I�u�W�F�N�g�𕽖ʂňړ�������", &m_bDragMoveXZ))
	{
		//// ��Ԃ̐؂�ւ�
		//m_bDragMoveXZ = m_bDragMoveXZ ? false : true;
	}
	else if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		m_bDragMoveXZ = true;
	}
	else if(pKeyboard->GetRelease(DIK_LSHIFT))
	{
		m_bDragMoveXZ = false;
	}

	if (ImGui::BeginTabBar("test000"))
	{
		if (ImGui::BeginTabItem(u8"�z�u�I�u�W�F�N�g�ݒ�"))
		{
			bSetMode = false;

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
	
	SetLoadFile();

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
}

//===================================================
// �R���X�g���N�^
//===================================================
CMapObjectManager::CMapObjectManager()
{
	m_pCopyObj = nullptr;
	m_fMove = MOVE_VALUE;
	m_bDragMoveXZ = false;
	m_pSelect = nullptr;
	m_nType = 0;
}

//===================================================
// ���f���̃p�X�̎擾
//===================================================
void CMapObjectManager::GetModelPath(void)
{
	// �}�b�v�̃I�u�W�F�N�g�̃��X�g�̎擾
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pMapObjectList == nullptr) return;

	// ���f���̑��������ׂ�
	for (auto &list : pMapObjectList->GetList())
	{
		// �t�@�C���p�X�̎擾
		std::string modelpath = list.filepath;

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

	if (ImGui::BeginCombo("##", m_aModelPath[m_nType].c_str()))
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

	// �}�b�v�̃I�u�W�F�N�g�̃��X�g�̎擾
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pMapObjectList == nullptr) return E_FAIL;

	// ������
	static char pModelName[MAX_WORD] = {};

	ImGui::SetNextItemWidth(200); // ���͗��̕����w��

	if (ImGui::InputText(u8"���f���̓o�^", pModelName, sizeof(pModelName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// �ȗ��p�p�X
		std::string modelPath = "data/MODEL/MapObjectFile/";
		std::string findPath = pModelName;

		// �܂܂�Ă�����
		if (findPath.find("data/MODEL/MapObjectFile/") != std::string::npos)
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
		for (auto& pathList : m_aModelPath)
		{
			// ���łɓo�^�ς݂�������
			if (pathList.find(modelPath) != std::string::npos)
			{
				MessageBox(NULL, modelPath.c_str(), "���łɓo�^����Ă��܂�", MB_OK);
				return E_FAIL;
			}
		}

		int nIdx = pModelManager->Register(modelPath.c_str());

		if (nIdx == -1) return E_FAIL;

		m_aModelPath.push_back(modelPath);

		pMapObjectList->Register();
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
		// ��ԏ��̎擾
		CTransform* pTransform = m_pSelect->GetTransform();

		// �擾�o������
		if (pTransform != nullptr)
		{
			// ���̎擾
			CTransform::Info transformInfo = pTransform->GetInfo();

			transformInfo.rot = Const::VEC3_NULL;

			// �����̐ݒ�
			pTransform->SetInfo(transformInfo);
		}
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

		// ��ԏ��̎擾
		CTransform* pTransform = (*itr)->GetTransform();

		// �擾�o������
		if (pTransform == nullptr) continue;
		
		// ���̎擾
		CTransform::Info transformInfo = pTransform->GetInfo();
	
		// �ʒu�̎擾
		D3DXVECTOR3 pos = transformInfo.pos;

		// �����̎擾
		D3DXVECTOR3 rot = transformInfo.rot;

		json obj =
		{
			{"file_path",(*itr)->GetPath()},
			{"position",{{"x",pos.x},{"y",pos.y},{"z",pos.z}}},
			{"rotation",{{"x",rot.x},{"y",rot.y},{"z",rot.z}}},
			{"type",(*itr)->GetType()},
		};

		config["MODEL_INFO"].push_back(obj);
	}

	// �t�@�C�����J��
	std::ofstream file(m_aLoadFileName);

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
		MessageBox(NULL, "�t�@�C�����J���܂���ł���", m_aLoadFileName.c_str(), MB_OK);
	}
}

//===================================================
// ���[�h����
//===================================================
void CMapObjectManager::Load(void)
{
	json config;

	// �t�@�C�����J��
	std::ifstream file(m_aLoadFileName);

	if (file.is_open())
	{
		file >> config;
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "�t�@�C�����J���܂���ł���", m_aLoadFileName.c_str(), MB_OK);
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

		std::string type = obj["type"];

		// ���f���̐���
		Create(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(rotX, rotY, rotZ), filepath.c_str(), type.c_str());
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
	// �}�b�v�I�u�W�F�N�g�̃}�l�[�W���[�N���X�̎擾
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pMapObjectList == nullptr) return;
	
	// �o�^����
	pMapObjectList->Register();

	for (auto& modellist : pMapObjectList->GetList())
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
void CMapObjectManager::SetCamerafocus(void)
{
	if (ImGui::Button(u8"�J������I�����f���Ƀt�H�[�J�X"))
	{
		D3DXVECTOR3 selectPos = Const::VEC3_NULL;

		// ��ԏ��̎擾
		CTransform* pTransform = m_pSelect->GetTransform();

		if (m_pSelect != nullptr && pTransform != nullptr)
		{
			// ���̎擾
			CTransform::Info transformInfo = pTransform->GetInfo();

			// �ʒu�̎擾
			selectPos = transformInfo.pos;
		}

		// �J�����̎擾
		CCamera* pCamera = CManager::GetCamera();

		// null�������珈�����Ȃ�
		if (pCamera == nullptr)return;

		pCamera->SetPosV(selectPos);
		pCamera->SetPosR(selectPos);
		pCamera->UpdatePositionV();
		pCamera->UpdatePositionR();
	}
}

//===================================================
// �e�N�X�`���̃A�C�R���̐ݒ�
//===================================================
void CMapObjectManager::SetTextureIcon(CImGuiManager* pImgui)
{
	// ���f���p�X�̃��X�g�������Ȃ�
	if (m_aModelPath.empty()) return;

	// �e�N�X�`���̃}�l�[�W���[�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTexture == nullptr) return;

	// �}�b�v�̃I�u�W�F�N�g�̃��X�g�̎擾
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pMapObjectList == nullptr) return;

	// �e�N�X�`��MT�̎擾
	CTextureMTManager* pTextureMT = CManager::GetTextureMT();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTextureMT == nullptr) return;

	for (auto& modelList : pMapObjectList->GetList())
	{
		// �p�X�̓o�^
		pTextureMT->GetAddress(modelList.filepath);
	}

	int nCnt = 0;

	for (auto& modelList : pTextureMT->GetList())
	{
		// ���f���̖��O�̎擾
		const char* pModelName = modelList.aName.c_str();

		// �e�N�X�`���̃A�C�R���̕\��
		pImgui->ShowTextureIcon(pTextureMT->GetAddress(pModelName), pModelName);

		// 3�\�����Ă��Ȃ��Ȃ�
		if (nCnt != 3)
		{
			ImGui::SameLine();
			nCnt++;
		}
		else
		{
			nCnt = 0;
		}
	}

	// ���s
	ImGui::NewLine();
}

//===================================================
// �R�s�[�y�[�X�g����
//===================================================
void CMapObjectManager::CopyAndPaste(CInputKeyboard *pKeyboard)
{
	// �I������Ă��Ȃ������珈�����Ȃ�
	if (m_pCopyObj != nullptr && pKeyboard->GetPress(DIK_LCONTROL) && pKeyboard->GetTrigger(DIK_V))
	{
		// ��ԏ��̎擾
		CTransform* pTransform = m_pCopyObj->GetTransform();

		if (m_pSelect != nullptr && pTransform != nullptr)
		{
			// ���̎擾
			CTransform::Info transformInfo = pTransform->GetInfo();

			// �ʒu�̎擾
			D3DXVECTOR3 pos = transformInfo.pos;
			D3DXVECTOR3 rot = transformInfo.rot;

			// �p�X�̎擾
			const char* pModelPath = m_pCopyObj->GetPath();

			// ����
			Create(pos, rot, pModelPath, m_pCopyObj->GetType());
		}
	}

	// ctrl + c
	if (pKeyboard->GetPress(DIK_LCONTROL) && pKeyboard->GetTrigger(DIK_C))
	{
		m_pCopyObj = m_pSelect;
	}
}

//===================================================
// �t�@�C�����̃��[�h
//===================================================
void CMapObjectManager::LoadFileName(void)
{
	std::fstream file("data/system.ini");
	std::string line;

	std::string stageFile;

	if (file.is_open())
	{
		// �t�@�C������s�Âǂݎ��
		while (std::getline(file, line))
		{
			if (line.find("STAGE_JSONFILE") != std::string::npos)
			{
				// = �̈ʒu�����߂�
				size_t pos = line.find('=');

				stageFile = line.substr(pos + 1);

				stageFile.erase(0, stageFile.find_first_not_of(" \t\"")); // �O�̋󔒂̏���
				stageFile.erase(stageFile.find_last_not_of(" \t\"") + 1); // ���̋󔒂̏���
			}
		}

		file.clear();
		file.close();
	}

	m_aLoadFileName = stageFile;
}

//===================================================
// �ǂݍ��ރt�@�C���̐ݒ�
//===================================================
void CMapObjectManager::SetLoadFile(void)
{
	// ������
	static char pModelName[MAX_WORD] = {};

	ImGui::Text(u8"���݂̃t�@�C���� [ %s ]", m_aLoadFileName.c_str());

	ImGui::SetNextItemWidth(200); // ���͗��̕����w��

	if (ImGui::InputText(u8"�t�@�C���ݒ�", pModelName, sizeof(pModelName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_aLoadFileName = pModelName;
	}
}

//===================================================
// �f�X�g���N�^
//===================================================
CMapObjectManager::~CMapObjectManager()
{
}
