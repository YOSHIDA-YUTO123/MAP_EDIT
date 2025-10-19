//=============================================================
//
//�@Imgui�̃}�l�[�W���[ [Imguimaneger.cpp]
//�@Author:YUTO YOSHIDA
//
//=============================================================

//*************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************
#include "imguimaneger.h"
#include "imgui_internal.h"
#include "manager.h"
#include "renderer.h"
#include "math.h"
#include "MapObjectManager.h"
#include "TextureMTManager.h"

using namespace ImGui; // ���O���imgui���g�p

//*************************************************************
// �ÓI�����o�ϐ�
//*************************************************************
bool CImGuiManager::m_bDraggingModel = false; // �h���b�O���Ă��邩�ǂ���
std::string CImGuiManager::m_DraggingModelType; // �h���b�O���Ă��郂�f���̎��

//=============================================================
// �R���X�g���N�^
//=============================================================
CImGuiManager::CImGuiManager()
{
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CImGuiManager::~CImGuiManager()
{
}

//=============================================================
// ����������
//=============================================================
void CImGuiManager::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

   // �t�H���g�ݒ�i�f�t�H���g�̃t�H���g�Ƃ��� Consolas ���g�p�j
	ImFont* myFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	io.FontDefault = myFont;
	io.FontGlobalScale = 1.0f;
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 1.0f;      // �p��
	//style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.4f, 0.f, 1.0f); // �D���ȐF

	//style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);     // �����F
	//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.1, 1); // �w�i�F
	//style.Colors[ImGuiCol_Button] = ImVec4(0.4, 0.7, 0.2, 1); // �{�^��
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5, 0.8, 0.3, 1); // �z�o�[
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3, 0.6, 0.2, 1); // �����ꂽ�Ƃ�
	//style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2, 0.2, 0.2, 1); // ���͗��̔w�i
	//style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0, 1.0, 1.0, 1); // �X���C�_�[�܂�
	style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.4f, 0.9f, 1.0f); // TreeNode�w�b�_�[

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
}

//=============================================================
// �I������
//=============================================================
void CImGuiManager::Uninit(void)
{
	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================
// �ʒu�̐ݒ菈��
//=============================================================
void CImGuiManager::SetPosition(ImVec2 pos)
{
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
}

//=============================================================
// �傫���̐ݒ菈��
//=============================================================
void CImGuiManager::SetSize(ImVec2 Size)
{
	ImGui::SetNextWindowSize(Size, ImGuiCond_Once);
}

//=============================================================
// �E�B���h�E�̕`��J�n
//=============================================================
void CImGuiManager::Start(const char* WindowName, TYPE type)
{
	if (WindowName != NULL)
	{
		switch (type)
		{
		case TYPE_DEFOULT:
			ImGui::Begin(WindowName);
			break;
		case TYPE_NOMOVE:
			ImGui::Begin(WindowName, NULL, ImGuiWindowFlags_NoMove);
			break;
		case TYPE_NOMOVESIZE:
			ImGui::Begin(WindowName, NULL, ImGuiWindowFlags_NoResize);
			break;
		case TYPE_NOMOVEANDSIZE:
			ImGui::Begin(WindowName, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			break;
		default:
			break;
		}
	}
}

//=============================================================
// �`��̏I������
//=============================================================
void CImGuiManager::EndRenderer(void)
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================
// �E�B���h�E�̃t���[���̍쐬����
//=============================================================
void CImGuiManager::NewFrame(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//=============================================================
// IMGUI�̃E�B���h�E�𑀍삵�Ă��邩�ǂ���
//=============================================================
bool CImGuiManager::GetActiveWindow(void)
{
	ImGuiIO& io = ImGui::GetIO();

	bool beingMoved = ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left);
	bool beingResized = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();

	// IMGUI�̃E�B���h�E�𑀍삵�Ă���
	if (io.WantCaptureMouse || beingMoved || beingResized)
	{
		return true;
	}

	return false;
}

//=============================================================
// IMGUI�̃e�N�X�`����ICON�̕\��
//=============================================================
void CImGuiManager::ShowTextureIcon(const LPDIRECT3DTEXTURE9 texture, const char* pModelType)
{
	if (ImGui::ImageButton(pModelType, (ImTextureID)texture, ImVec2(64, 64)))
	{
	}

	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && !m_bDraggingModel)
	{
		m_bDraggingModel = true;
		m_DraggingModelType = pModelType;
	}

	if (m_bDraggingModel && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &mousePos); // DirectX���W�n�ɕϊ�

		// ImGui�̃f�o�C�X�ƃr���[�|�[�g�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		D3DVIEWPORT9 vp;
		pDevice->GetViewport(&vp);

		// �v�Z�p�s��
		D3DXMATRIX view, proj;

		// �r���[�A�v���W�F�N�V�����}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &view);
		pDevice->GetTransform(D3DTS_PROJECTION, &proj);

		// �ϊ����W�̎擾
		D3DXVECTOR3 pos = math::ScreenToWorld(
			D3DXVECTOR2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)),
			0.0f,
			vp,
			view,
			proj);

		// �u���b�N�𐶐�����
		CMapObjectManager::GetInstance()->Create(pos, { 0.0f,0.0f,0.0f }, m_DraggingModelType.c_str(),"static");
		
		m_bDraggingModel = false;
		m_DraggingModelType.clear();
	}

	// �h���b�O����������
	if (m_bDraggingModel && m_DraggingModelType == pModelType)
	{
		// �e�N�X�`��MT�̎擾
		CTextureMTManager* pTextureMT = CManager::GetTextureMT();

		// �擾�ł��Ȃ������珈�����Ȃ�
		if (pTextureMT == nullptr) return;

		ImVec2 mouse = ImGui::GetMousePos();
		ImGui::GetForegroundDrawList()->AddImage(
			(ImTextureID)texture,
			ImVec2(mouse.x - 32, mouse.y - 32),
			ImVec2(mouse.x + 32, mouse.y + 32),
			ImVec2(0, 0),
			ImVec2(1, 1),
			IM_COL32(255, 255, 255, 180));
	}

	// �{�^���𗣂�����h���b�O�I��
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		m_bDraggingModel = false;
	}
}