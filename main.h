//===================================================
//
// ���C�� [main.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// ��d�C���N���[�h�h�~
//***************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************

//***************************************************
// ���C���N���X�̒�`
//***************************************************
class CMain
{
public:
	CMain();
	~CMain();
	void ToggleFullscreen(HWND hWnd); // �t���X�N���[��

	/// <summary>
	/// �E�B���h�E��XFile���h���b�v�����Ƃ��̏���
	/// </summary>
	/// <param name="hWnd"></param>
	/// <param name="wParam"></param>
	void DropXFile(HWND hWnd, WPARAM wParam);

	/// <summary>
	/// �h���b�v���ꂽ�t�@�C���̃p�X��\\��/�ɕϊ����鏈��
	/// </summary>
	/// <param name="path"></param>
	void NormalizePathSlash(TCHAR* path);
private:
	RECT m_windowRect;				  // �E�B���h�E�̃T�C�Y
	bool m_bFullScreen;				  // �t���X�N���[�����ǂ���
};
#endif
