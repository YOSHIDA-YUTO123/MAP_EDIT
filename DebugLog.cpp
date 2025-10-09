//===================================================
//
// �f�o�b�O���O [DebugLog.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "DebugLog.h"
#include "imguimaneger.h"
#include "manager.h"
#include "renderer.h"
#include "imgui.h"

//===================================================
// �R���X�g���N�^
//===================================================
CDebugLog::CDebugLog()
{
}

//===================================================
// �f�X�g���N�^
//===================================================
CDebugLog::~CDebugLog()
{

}

//===================================================
// ��������
//===================================================
CDebugLog* CDebugLog::Create(void)
{
	return new CDebugLog;
}

//===================================================
// ����������
//===================================================
HRESULT CDebugLog::Init(void)
{

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CDebugLog::Uninit(void)
{
	// �������g�̔j��
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CDebugLog::Update(void)
{
	// Imgui�̎擾
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// null�������珈�����Ȃ�
	if (pImgui == nullptr) return;

	pImgui->SetSize(ImVec2(200.0f, 200.0f));
	pImgui->SetPosition(ImVec2(1080.0f, 0.0f));

	pImgui->Start("Log", CImGuiManager::TYPE_NOMOVESIZE);

	// �v�f�𒲂ׂ�
	for (auto it = m_Info.begin(); it != m_Info.end(); it++)
	{
		// �f�o�b�O���O�̐ݒ�
		ImGui::Text(it->aMessage.c_str());

		it->nTime--;

		// �g�p���Ԃ��I��������
		if (it->nTime <= 0)
		{
			it = m_Info.erase(it);
		}

		if (it == m_Info.end()) break;
	}

	pImgui->End();
}

//===================================================
// �`�揈��
//===================================================
void CDebugLog::Draw(void)
{
}

//===================================================
// �ݒ菈��
//===================================================
void CDebugLog::Set(const char* pMessage, const int nTime)
{
	CDebugLog::Info info = {};

	info.aMessage = pMessage;
	info.nTime = nTime;

	// ���X�g�ɐݒ�
	m_Info.push_back(info);
}
