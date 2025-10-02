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
#include "debugproc.h"

//***************************************************
// �O���錾
//***************************************************
class CImGuiManager;

//***************************************************
// �����_���[�N���X�̒�`
//***************************************************
class CRenderer
{
public:
	static constexpr int NUM_TEXTUREMT = 2; // �e�N�X�`���̐�

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(const int fps);
	static CImGuiManager* GetImGui(void) { return m_pImGui; }

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU); // �����_�����O�^�[�Q�b�g�̕ύX

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
	LPDIRECT3DTEXTURE9 m_pTextureMT[NUM_TEXTUREMT];		// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
	LPDIRECT3DSURFACE9 m_pRenderMT[NUM_TEXTUREMT];		// �����_�����O�^�[�Q�b�g�p�C���^�[�t�F�[�X
	LPDIRECT3DSURFACE9 m_pZBuffMT;						// �����_�����O�^�[�Q�b�g�pZ�o�b�t�@
	D3DVIEWPORT9 m_viewport;							// �e�N�X�`�������_�����O�p�r���[�|�[�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;				// �t�B�[�h�o�b�O�p�|���S���̒��_�o�b�t�@
	LPDIRECT3D9 m_pD3D;									// Directx�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// Directx�f�o�C�X�ւ̃|�C���^
	UINT m_ResizeWidth, m_ResizeHeight;					// ��ʂ̑傫���ύX���̑傫��
	D3DPRESENT_PARAMETERS m_d3dpp;						// �v���[���e�[�V�����p�����[�^
	float m_fALv;										// A�l
	bool m_bDeviceLost;									// �f�o�C�X�̃��X�g����
	bool m_bEffect;
};

#endif