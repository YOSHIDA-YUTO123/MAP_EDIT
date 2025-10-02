//================================================
//
// �|�[�Y [pause.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "pause.h"
#include"manager.h"
#include"renderer.h"
#include"textureManager.h"
#include"input.h"
#include"math.h"
#include "sound.h"

using namespace Const; // ���O���Const���g�p
using namespace std; // ���O���std���g�p
using namespace math; // ���O���math���g�p

constexpr float UI_MAG = 1.2f;	// �g�嗦
constexpr float COUNTER = 1.0f / 30.0f; // �J�E���^�[

//************************************************
// �ÓI�����o�ϐ��錾
//************************************************
bool CPauseManager::m_bPause = false; // �|�[�Y��Ԃ��ǂ���
unique_ptr<CPauseManager> CPauseManager::m_pInstance = nullptr; // �����̃C���X�^���X

//================================================
// �R���X�g���N�^
//================================================
CPause::CPause(const TYPE type)
{
	m_Type = type;
	m_BaseSize = VEC2_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CPause::~CPause()
{
}

//================================================
// ��������
//================================================
CPause* CPause::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const TYPE type)
{
	// �|�[�Y�N���X�ւ̃|�C���^
	CPause* pPause = nullptr;

	// ��ނ̑J��
	switch (type)
	{
	case TYPE_CONTINUE:
		pPause = new CPauseContinue;
		break;
	case TYPE_RETRY:
		pPause = new CPauseRetry;
		break;
	case TYPE_QUIT:
		pPause = new CPauseQuit;
		break;
	default:
		return nullptr;
	}
	
	// ����������
	pPause->Init();

	// �ʒu�̐ݒ菈��
	pPause->SetPosition(pos);
	pPause->SetSize(Size.x,Size.y);
	pPause->SetVtx(WHITE);
	pPause->m_BaseSize = Size;

	return pPause;
}

//================================================
// ����������
//================================================
HRESULT CPause::Init(void)
{
	// ����������
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_PAUSE);

	return S_OK;
}

//================================================
// �I������
//================================================
void CPause::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//================================================
// �`�揈��
//================================================
void CPause::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//================================================
// �R���X�g���N�^(Continue)
//================================================
CPauseContinue::CPauseContinue() : CPause(TYPE_CONTINUE)
{
}

//================================================
// �f�X�g���N�^(Continue)
//================================================
CPauseContinue::~CPauseContinue()
{
}

//================================================
// ����������(Continue)
//================================================
HRESULT CPauseContinue::Init(void)
{
	// ����������
	if (FAILED(CPause::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̐ݒ�
	SetTextureID("data/TEXTURE/pause/pause_continue.png");

	return S_OK;
}

//================================================
// �I������(Continue)
//================================================
void CPauseContinue::Uninit(void)
{
	// �I������
	CPause::Uninit();
}

//================================================
// �X�V����(Continue)
//================================================
void CPauseContinue::Update(void)
{
	// �|�[�Y�}�l�[�W���[�̎擾
	CPauseManager* pPauseManager = CPauseManager::GetInstance();

	// �|�[�Y��Ԃ̎擾
	bool bPause = CPauseManager::GetPause();

	// �|�[�Y�̃}�l�[�W���[��null��������܂��̓|�[�Y������Ȃ��Ȃ珈�����Ȃ�
	if (pPauseManager == nullptr || bPause == false) return;

	// �I�𒆂̎��
	TYPE select = pPauseManager->GetSelectMenu();

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �p�b�h�̎擾
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// �傫���̎擾
	D3DXVECTOR2 Size = CObject2D::GetSize();

	// ��̑傫���̎擾
	D3DXVECTOR2 BaseSize = CPause::GetBaseSize();

	// �I�𒆂̎�ނƎ����̎�ނ���v���Ă�����
	if (select == CPause::GetType())
	{
		// �ړI�̑傫��
		D3DXVECTOR2 DestSize = BaseSize * UI_MAG;

		// �ړI�̑傫���ɋ߂Â���
		Size += (DestSize - Size) * 0.1f;

		// �傫���̐ݒ�
		CObject2D::SetSize(Size.x, Size.y);

		// �F
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));

		// ENTER�L�[�������ꂽ�� || A�{�^���������ꂽ��
		if (pKeyboard->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A))
		{
			// �|�[�Y���I�t
			pPauseManager->EnablePause(false);
		}
	}
	else
	{
		// �ړI�̑傫���ɋ߂Â���
		Size += (BaseSize - Size) * 0.5f;

		// �傫���̐ݒ�
		CObject2D::SetSize(Size.x, Size.y);

		// �F�̐ݒ�
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//================================================
// �`�揈��(Continue)
//================================================
void CPauseContinue::Draw(void)
{
	// �`�揈��
	CPause::Draw();
}


//================================================
// �R���X�g���N�^(���g���C)
//================================================
CPauseRetry::CPauseRetry() : CPause(TYPE_RETRY)
{

}

//================================================
// �f�X�g���N�^(���g���C)
//================================================
CPauseRetry::~CPauseRetry()
{
}

//================================================
// ����������(���g���C)
//================================================
HRESULT CPauseRetry::Init(void)
{
	// ����������
	if (FAILED(CPause::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̐ݒ�
	SetTextureID("data/TEXTURE/pause/pause_retry.png");

	return S_OK;
}

//================================================
// �I������(���g���C)
//================================================
void CPauseRetry::Uninit(void)
{
	// �I������
	CPause::Uninit();
}

//================================================
// �X�V����(���g���C)
//================================================
void CPauseRetry::Update(void)
{
	// �|�[�Y�}�l�[�W���[�̎擾
	CPauseManager* pPauseManager = CPauseManager::GetInstance();

	// �|�[�Y��Ԃ̎擾
	bool bPause = CPauseManager::GetPause();

	// �|�[�Y�̃}�l�[�W���[��null��������܂��̓|�[�Y������Ȃ��Ȃ珈�����Ȃ�
	if (pPauseManager == nullptr || bPause == false) return;

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �p�b�h�̎擾
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// �I�𒆂̎��
	TYPE select = pPauseManager->GetSelectMenu();

	// �傫���̎擾
	D3DXVECTOR2 Size = CObject2D::GetSize();

	// ��̑傫���̎擾
	D3DXVECTOR2 BaseSize = CPause::GetBaseSize();

	// �I�𒆂̎�ނƎ����̎�ނ���v���Ă�����
	if (select == CPause::GetType())
	{
		// �ړI�̑傫��
		D3DXVECTOR2 DestSize = BaseSize * UI_MAG;

		// �ړI�̑傫���ɋ߂Â���
		Size += (DestSize - Size) * 0.1f;

		// �傫���̐ݒ�
		CObject2D::SetSize(Size.x, Size.y);

		// �F
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));

		// ENTER�L�[�������ꂽ��
		if (pKeyboard->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A))
		{
		}
	}
	else
	{
		// �ړI�̑傫���ɋ߂Â���
		Size += (BaseSize - Size) * 0.5f;

		// �傫���̐ݒ�
		CObject2D::SetSize(Size.x, Size.y);

		// �F�̐ݒ�
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//================================================
// �`�揈��(���g���C)
//================================================
void CPauseRetry::Draw(void)
{
	// �`�揈��
	CPause::Draw();
}

//================================================
// �R���X�g���N�^(��߂�)
//================================================
CPauseQuit::CPauseQuit() : CPause(TYPE_QUIT)
{

}

//================================================
// �f�X�g���N�^(��߂�)
//================================================
CPauseQuit::~CPauseQuit()
{
}

//================================================
// ����������(��߂�)
//================================================
HRESULT CPauseQuit::Init(void)
{
	// ����������
	if (FAILED(CPause::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̐ݒ�
	SetTextureID("data/TEXTURE/pause/pause_quit.png");

	return S_OK;
}

//================================================
// �I������(��߂�)
//================================================
void CPauseQuit::Uninit(void)
{
	// �I������
	CPause::Uninit();
}

//================================================
// �X�V����(��߂�)
//================================================
void CPauseQuit::Update(void)
{
	// �|�[�Y�}�l�[�W���[�̎擾
	CPauseManager* pPauseManager = CPauseManager::GetInstance();

	// �|�[�Y��Ԃ̎擾
	bool bPause = CPauseManager::GetPause();

	// �|�[�Y�̃}�l�[�W���[��null��������܂��̓|�[�Y������Ȃ��Ȃ珈�����Ȃ�
	if (pPauseManager == nullptr || bPause == false) return;

	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �p�b�h�̎擾
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// �I�𒆂̎��
	TYPE select = pPauseManager->GetSelectMenu();

	// �傫���̎擾
	D3DXVECTOR2 Size = CObject2D::GetSize();

	// ��̑傫���̎擾
	D3DXVECTOR2 BaseSize = CPause::GetBaseSize();

	// �I�𒆂̎�ނƎ����̎�ނ���v���Ă�����
	if (select == CPause::GetType())
	{
		// �ړI�̑傫��
		D3DXVECTOR2 DestSize = BaseSize * UI_MAG;

		// �ړI�̑傫���ɋ߂Â���
		Size += (DestSize - Size) * 0.1f;

		// �傫���̐ݒ�
		CObject2D::SetSize(Size.x, Size.y);

		// �F
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));

		// ENTER�L�[�������ꂽ��
		if (pKeyboard->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A))
		{
			// ���̎擾
			CSound* pSound = CManager::GetSound();

			if (pSound != nullptr)
			{
				// ���̍Đ�
				pSound->Play(CSound::SOUND_LABEL_ENTER);
			}
		}
	}
	else
	{
		// �ړI�̑傫���ɋ߂Â���
		Size += (BaseSize - Size) * 0.5f;

		// �傫���̐ݒ�
		CObject2D::SetSize(Size.x, Size.y);

		// �F�̐ݒ�
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//================================================
// �`�揈��(��߂�)
//================================================
void CPauseQuit::Draw(void)
{
	// �`�揈��
	CPause::Draw();
}

//================================================
// �R���X�g���N�^(�}�l�[�W���[)
//================================================
CPauseManager::CPauseManager()
{
	m_SelectMenu = CPause::TYPE_CONTINUE;
}

//================================================
// �f�X�g���N�^(�}�l�[�W���[)
//================================================
CPauseManager::~CPauseManager()
{
	m_bPause = false;
}

//================================================
// ��������
//================================================
void CPauseManager::Create(void)
{
	// ��������������Ă��Ȃ�������
	if (m_pInstance == nullptr)
	{
		// �����̐���
		m_pInstance.reset(new CPauseManager);

		// �|�[�Y��
		CPauseNow::Create(D3DXVECTOR3(80.0f,660.0f, 0.0f), D3DXVECTOR2(60.0f, 50.0f));

		// �|�[�Y�̑I�����j���[��
		for (int nCnt = CPause::TYPE_CONTINUE; nCnt <= CPause::TYPE_QUIT; nCnt++)
		{
			// �L���X�g����
			CPause::TYPE type = static_cast<CPause::TYPE>(nCnt);

			// �|�[�Y�̐�������
			CPause::Create(D3DXVECTOR3(640.0f, 360.0f + (nCnt * 100.0f), 0.0f), D3DXVECTOR2(100.0f, 50.0f), type);
		}
	}
}

//================================================
// �I������
//================================================
void CPauseManager::Uninit(void)
{
	// �j��
	m_pInstance.reset();
	m_pInstance = nullptr;
}

//================================================
// �|�[�Y�̐؂�ւ�����(�}�l�[�W���[)
//================================================
void CPauseManager::EnablePause(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �p�b�h�̎擾
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	if (pKeyboard != nullptr)
	{
		// p�L�[�������ꂽ
		if (pKeyboard->GetTrigger(DIK_P))
		{
			// �|�[�Y�̐؂�ւ�����
			m_bPause = m_bPause ? false : true;
		}
	}

	if (pJoyPad != nullptr)
	{
		// START�{�^���������ꂽ��
		if (pJoyPad->GetTrigger(pJoyPad->JOYKEY_START))
		{
			// �|�[�Y�̐؂�ւ�����
			m_bPause = m_bPause ? false : true;
		}
	}
}

//================================================
// ���j���[�̑I������
//================================================
void CPauseManager::SelectMenu(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �p�b�h�̎擾
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// ���̎擾
	CSound* pSound = CManager::GetSound();

	if (pKeyboard != nullptr && pJoyPad != nullptr)
	{
		// �オ�����ꂽ��
		if (pKeyboard->GetTrigger(DIK_UP) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_UP))
		{
			if (pSound != nullptr)
			{
				// ���̍Đ�
				pSound->Play(CSound::SOUND_LABEL_MENU);
			}

			// ���̍��ڂ�
			m_SelectMenu = static_cast<CPause::TYPE>(m_SelectMenu - 1);
		}
		// ���������ꂽ��
		else if (pKeyboard->GetTrigger(DIK_DOWN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_DOWN))
		{
			if (pSound != nullptr)
			{
				// ���̍Đ�
				pSound->Play(CSound::SOUND_LABEL_MENU);
			}

			// �O�̍��ڂ�
			m_SelectMenu = static_cast<CPause::TYPE>(m_SelectMenu + 1);
		}
	}

	// �͈͓������[�v����
	m_SelectMenu = Wrap(m_SelectMenu, CPause::TYPE_CONTINUE, CPause::TYPE_QUIT);
}

//================================================
// �R���X�g���N�^(�|�[�Y��)
//================================================
CPauseNow::CPauseNow()
{
	m_BaseSize = VEC2_NULL;
	m_fCounter = NULL;
}

//================================================
// �f�X�g���N�^(�|�[�Y��)
//================================================
CPauseNow::~CPauseNow()
{
}

//================================================
// ��������
//================================================
CPauseNow* CPauseNow::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size)
{
	// �|�[�Y���̐���
	CPauseNow* pPause = new CPauseNow;

	pPause->Init();
	pPause->SetPosition(pos);
	pPause->SetSize(Size.x, Size.y);
	pPause->SetVtx(WHITE);
	pPause->m_BaseSize = Size;

	return pPause;
}

//================================================
// ����������(�|�[�Y��)
//================================================
HRESULT CPauseNow::Init(void)
{
	// ����������
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_PAUSE);

	// �e�N�X�`����ID�̐ݒ�
	CObject2D::SetTextureID("data/TEXTURE/pause/pausenow.png");

	return S_OK;
}

//================================================
// �I������(�|�[�Y��)
//================================================
void CPauseNow::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//================================================
// �X�V����(�|�[�Y��)
//================================================
void CPauseNow::Update(void)
{
	m_fCounter += COUNTER;

	D3DXVECTOR2 Size = VEC2_NULL;

	// �傫�������߂�
	Size.x = fabsf(sinf(m_fCounter) * (m_BaseSize.x * 0.2f)) + m_BaseSize.x;
	Size.y = fabsf(sinf(m_fCounter) * (m_BaseSize.y * 0.2f)) + m_BaseSize.x;

	// �傫�����v�Z
	CObject2D::SetSize(Size.x, Size.y);
}

//================================================
// �`�揈��(�|�[�Y��)
//================================================
void CPauseNow::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}
