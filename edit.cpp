//===================================================
//
// �ҏW���[�h����Imgui�̏������܂Ƃ߂�N���X [edit.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "edit.h"
#include "MapObjectManager.h"
#include "imguimaneger.h"
#include "manager.h"
#include "renderer.h"
#include "MeshFieldManager.h"

//===================================================
// �R���X�g���N�^
//===================================================
CEdit::CEdit()
{
}

//===================================================
// �f�X�g���N�^
//===================================================
CEdit::~CEdit()
{
}

//===================================================
// ��������
//===================================================
CEdit* CEdit::Create(void)
{
	CEdit* pEdit = new CEdit;

	// ����������
	if (FAILED(pEdit->Init()))
	{
		pEdit->Uninit();
		pEdit = nullptr;
		return nullptr;
	}

	return pEdit;
}

//===================================================
// ����������
//===================================================
HRESULT CEdit::Init(void)
{

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CEdit::Uninit(void)
{
	// �������g�̔j��
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CEdit::Update(void)
{
	// �C���X�^���X�̎擾
	CMapObjectManager* pMapObjManager = CMapObjectManager::GetInstance();

	CMeshFieldManager* pMeshFieldManager = CMeshFieldManager::GetInstance();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pMapObjManager == nullptr) return;
	if (pMeshFieldManager == nullptr) return;

	// Imgui�̎擾
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pImgui == nullptr) return;

	// �E�B���h�E�̐ݒ�
	pImgui->SetPosition(ImVec2(0.0f, 0.0f));
	pImgui->SetSize(ImVec2(400.0f, 500.0f));
	pImgui->Start(u8"�C���X�y�N�^�[", CImGuiManager::TYPE_NOMOVEANDSIZE);

	if (ImGui::BeginTabBar("test000"))
	{
		if (ImGui::BeginTabItem(u8"���f���z�u"))
		{
			// �X�V����
			pMapObjManager->Update();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"�t�B�[���h�ҏW"))
		{
			// �X�V����
			pMeshFieldManager->Update();

			ImGui::EndTabItem();
		}
		else
		{
			// �X�V����
			pMeshFieldManager->SetShow(false);
		}

		// �^�u�o�[�̏I������
		ImGui::EndTabBar();
	}

	// �I������
	pImgui->End();

	// �E�B���h�E�̐ݒ�
	pImgui->SetPosition(ImVec2(0.0f, 500.0f));

	ImVec2 window = ImGui::GetWindowSize();
	float fHeight = SCREEN_HEIGHT - 500.0f;

	pImgui->SetSize(ImVec2(400.0f, fHeight));
	pImgui->Start("Asset", CImGuiManager::TYPE_NOMOVEANDSIZE);

	// �A�C�R���̐ݒ菈��
	pMapObjManager->SetTextureIcon(pImgui);

	pImgui->End();

}

//===================================================
// �`�揈��
//===================================================
void CEdit::Draw(void)
{
}
