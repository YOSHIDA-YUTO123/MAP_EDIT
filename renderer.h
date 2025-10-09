//===================================================
//
// �|���S���̕`�揈�� [renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "main.h"

//***************************************************
// �O���錾
//***************************************************
class CImGuiManager;
class CDebugProc;

//***************************************************
// �����_���[�N���X�̒�`
//***************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(const int fps);
	static CImGuiManager* GetImGui(void) { return m_pImGui; }

	LPDIRECT3DDEVICE9 GetDevice(void);

	void onWireFrame();
	void offWireFrame();
	void onEffect(const float fLevel); // �u���[�̃I��
	void offEffect(void);
	void ResetDevice(void);
	HRESULT GetDeviceLost(void);
	void SetReSize(const UINT Width, const UINT Height);

private:
	static CImGuiManager* m_pImGui;						// IMGUI�N���X�ւ̃|�C���^
	static CDebugProc* m_pDebug;						// �f�o�b�O�t�H���g�ւ̃|�C���^
	LPDIRECT3D9 m_pD3D;									// Directx�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// Directx�f�o�C�X�ւ̃|�C���^
	UINT m_ResizeWidth, m_ResizeHeight;					// ��ʂ̑傫���ύX���̑傫��
	D3DPRESENT_PARAMETERS m_d3dpp;						// �v���[���e�[�V�����p�����[�^
	float m_fALv;										// A�l
	bool m_bDeviceLost;									// �f�o�C�X�̃��X�g����
	bool m_bEffect;
};

#endif