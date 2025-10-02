//===================================================
//
// �|���S���̕`�揈�� [renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "imguimaneger.h"
#include "light.h"

//***************************************************
// ���O���
//***************************************************
using namespace Const;	// ���O���Const���g�p����

//***************************************************
// �ÓI�����o�ϐ��̐錾
//***************************************************
CDebugProc* CRenderer::m_pDebug = nullptr;		// �f�o�b�O�I�u�W�F�N�g�ւ̃|�C���^
CImGuiManager* CRenderer::m_pImGui = nullptr;	// Imgui�}�l�[�W���[�ւ̃|�C���^

//===================================================
// �R���X�g���N�^
//===================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;			// Directx�̃f�o�C�X�̏�����
	m_pD3DDevice = nullptr;		// Directx�̃f�o�C�X�̏�����

	for (int nCnt = 0; nCnt < NUM_TEXTUREMT; nCnt++)
	{
		m_pRenderMT[nCnt] = nullptr;		// �����_�����O�^�[�Q�b�g�p�C���^�[�t�F�[�X
		m_pTextureMT[nCnt] = nullptr;		// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
	}
	m_pVtxBuffMT = nullptr;
	m_fALv = NULL;
	m_pZBuffMT = nullptr;		// �����_�����O�^�[�Q�b�g�pZ�o�b�t�@
	ZeroMemory(&m_viewport, sizeof(m_viewport)); // �r���[�|�[�g
	m_bEffect = false;
	m_bDeviceLost = false;
	m_ResizeHeight = 0;
	m_ResizeWidth = 0;
}

//===================================================
// �f�X�g���N�^
//===================================================
CRenderer::~CRenderer()
{

}

//===================================================
// ����������
//===================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;		 // �f�B�X�v���C���[�h

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); // �p�����[�^�̃[���N���A

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;							// �Q�[����ʃT�C�Y(��)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// �Q�[����ʃT�C�Y(����)
	m_d3dpp.BackBufferFormat = d3ddm.Format;							// �o�b�N�o�b�t�@�̌`��
	m_d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �o�b�N�o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@�Ƃ���24bit,�X�e���V���o�b�t�@8bit2
	m_d3dpp.Windowed = bWindow;										// �E�C���h�E���[�h
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��

	// Direct3D�f�o�C�X�̐����`�揈���ƒ��_�������n�[�h�E�F�A�ōs��
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// ����������
	if (m_pDebug == nullptr)
	{
		m_pDebug = new CDebugProc;
		m_pDebug->Init();
	}

	//LPDIRECT3DSURFACE9 pRenderDef, pZBufferDef;

	//for (int nCnt = 0; nCnt < NUM_TEXTUREMT; nCnt++)
	//{
	//	// �����_���[�^�[�Q�b�g�p�e�N�X�`���̐���
	//	m_pD3DDevice->CreateTexture(
	//		SCREEN_WIDTH,
	//		SCREEN_HEIGHT,
	//		1,
	//		D3DUSAGE_RENDERTARGET,
	//		D3DFMT_X8R8G8B8,
	//		D3DPOOL_DEFAULT,
	//		&m_pTextureMT[nCnt],
	//		NULL);

	//	// �e�N�X�`���̃����_�����O�^�[�Q�b�g�p�C���^�[�t�F�[�X�̐���
	//	m_pTextureMT[nCnt]->GetSurfaceLevel(0, &m_pRenderMT[nCnt]);
	//}

	//// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
	//m_pD3DDevice->CreateDepthStencilSurface(
	//	SCREEN_WIDTH,
	//	SCREEN_HEIGHT,
	//	D3DFMT_D24S8,
	//	D3DMULTISAMPLE_NONE,
	//	0,
	//	TRUE,
	//	&m_pZBuffMT,
	//	NULL);

	//// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	//m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	//// ���݂�Z�o�b�t�@�̎擾(�ۑ�)
	//m_pD3DDevice->GetDepthStencilSurface(&pZBufferDef);

	//for (int nCnt = 0; nCnt < NUM_TEXTUREMT; nCnt++)
	//{
	//	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	//	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[nCnt]);
	//}

	//// Z�o�b�t�@�𐶐�����Z�o�b�t�@�̐ݒ�
	//m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	//// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̃N���A
	//m_pD3DDevice->Clear(0,
	//	NULL,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	//// �����_�[�^�[�Q�b�g�����Ƃɖ߂�
	//m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	//// Z�o�b�t�@�����Ƃɖ߂�
	//m_pD3DDevice->SetDepthStencilSurface(pZBufferDef);

	//// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐���
	//m_viewport.X = 0;
	//m_viewport.Y = 0;
	//m_viewport.Width = SCREEN_WIDTH;
	//m_viewport.Height = SCREEN_HEIGHT;
	//m_viewport.MinZ = 0.0f;
	//m_viewport.MaxZ = 1.0f;

	////���_�o�b�t�@�̐����E���_���̐ݒ�
	//if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEXTUREMT,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&m_pVtxBuffMT,
	//	NULL)))
	//{
	//	return E_FAIL;
	//}

	//// ���_���̃|�C���^
	//VERTEX_2D* pVtx;

	//// ���_�o�b�t�@�̃��b�N
	//m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCnt = 0; nCnt < NUM_TEXTUREMT; nCnt++)
	//{
	//	D3DXVECTOR2 pos = D3DXVECTOR2(640.0f, 360.0f);

	//	float width = SCREEN_WIDTH * 0.5f;
	//	float height = SCREEN_HEIGHT * 0.5f;

	//	// ���_���W�̐ݒ�
	//	pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

	//	// rhw�̐ݒ�
	//	pVtx[0].rhw = 1.0f;
	//	pVtx[1].rhw = 1.0f;
	//	pVtx[2].rhw = 1.0f;
	//	pVtx[3].rhw = 1.0f;

	//	float fA = (nCnt == 0) ? 1.0f : 0.8f;

	//	// ���_�J���[�̐ݒ�
	//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);

	//	// �e�N�X�`�����W�̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	pVtx += 4;
	//}
	//// ���_�o�b�t�@�̃A�����b�N
	//m_pVtxBuffMT->Unlock();

	m_pImGui = new CImGuiManager;
	m_pImGui->Init(hWnd, m_pD3DDevice);

	return S_OK;
}
//===================================================
// �I������
//===================================================
void CRenderer::Uninit(void)
{
	if (m_pDebug != nullptr)
	{
		// �I������
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	// ���ׂẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	for (int nCnt = 0; nCnt < NUM_TEXTUREMT; nCnt++)
	{
		// �����_�����O�^�[�Q�b�g�p�C���^�[�t�F�[�X�̔j��
		if (m_pRenderMT[nCnt]!= nullptr)
		{
			m_pRenderMT[nCnt]->Release();
			m_pRenderMT[nCnt] = nullptr;
		}

		// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̔j��
		if (m_pTextureMT[nCnt] != nullptr)
		{
			m_pTextureMT[nCnt]->Release();
			m_pTextureMT[nCnt] = nullptr;
		}
	}

	// �����_�����O�^�[�Q�b�g�pZ�o�b�t�@�̔j��
	if (m_pZBuffMT != nullptr)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = nullptr;
	}

	// �t�B�[�h�o�b�N�G�t�F�N�g�p���_�o�b�t�@�̔j��
	if (m_pVtxBuffMT != nullptr)
	{
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = nullptr;
	}

	// Drectx�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Drectx�f�o�C�X�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// ImGui�̔j��
	if (m_pImGui != nullptr)
	{
		m_pImGui->Uninit();
		delete m_pImGui;
		m_pImGui = nullptr;
	}
}
//===================================================
// �X�V����
//===================================================
void CRenderer::Update(void)
{
#ifdef _DEBUG

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_H))
	{
		m_bEffect = m_bEffect ? false : true;
		m_fALv = m_bEffect ? 0.8f : 0.0f;
	}
#endif // _DEBUG

	// ���ׂẴI�u�W�F�N�g�̍X�V����
	CObject::UpdateAll();
}
//===================================================
// �`�揈��
//===================================================
void CRenderer::Draw(const int fps)
{
	// ��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ		

		// ���ׂẴI�u�W�F�N�g�̕`�揈��
		CObject::DrawAll();

		// ImGui�̕`��I��
		m_pImGui->EndRenderer();		

#ifdef _DEBUG
		CDebugProc::Print("FPS = %d\n", fps);

		CDebugProc::Draw();
#endif // _DEBUG

		CDebugProc::Reset();

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===================================================
// �����_�����O�^�[�Q�b�g�̕ύX
//===================================================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	D3DXMATRIX mtxView, mtxProjection; // �r���[�}�g���b�N�X�A�v���W�F�N�V�����}�g���b�N�X
	float fAspect;

	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[0]);

	// ��������Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐ݒ�
	m_pD3DDevice->SetViewport(&m_viewport);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&mtxView,
		&posV,
		&posR,
		&vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	fAspect = (float)m_viewport.Width / (float)m_viewport.Height;

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		fAspect,
		1.0f,
		100000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//===================================================
// �f�o�C�X�̎擾����
//===================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	// �f�o�C�X�̃|�C���^��Ԃ�
	return m_pD3DDevice;
}

//===================================================
// ���C���[�t���[���I��
//===================================================
void CRenderer::onWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

//===================================================
// ���C���[�t���[���I�t
//===================================================
void CRenderer::offWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//===================================================
// �u���[�̃I��
//===================================================
void CRenderer::onEffect(const float fLevel)
{
	m_bEffect = true;
	m_fALv = fLevel;
}

//===================================================
// �u���[�̃I�t
//===================================================
void CRenderer::offEffect(void)
{
	m_bEffect = false;
	m_fALv = 0.0f;
}

//===================================================
// �f�o�C�X�̃��Z�b�g
//===================================================
void CRenderer::ResetDevice(void)
{
	// ���C�g�̎擾
	CLight* pLight = CManager::GetLight();

	if (m_pDebug != nullptr)
	{
		// �f�o�b�O�t�H���g�̔j��
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = m_pD3DDevice->Reset(&m_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�`���[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �J�����̎擾
	pLight->Init();
	pLight->SetDirectional(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));

	if (m_pDebug == nullptr)
	{
		m_pDebug = new CDebugProc;
		m_pDebug->Init();
	}
}

//===================================================
// �f�o�C�X�̑����̎擾
//===================================================
HRESULT CRenderer::GetDeviceLost(void)
{
	// Handle lost D3D9 device
	if (m_bDeviceLost)
	{
		HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
		if (hr == D3DERR_DEVICELOST)
		{
			return D3DERR_DEVICELOST;
		}
		if (hr == D3DERR_DEVICENOTRESET)
		{
			ResetDevice();
			m_bDeviceLost = false;
			return D3DERR_DEVICELOST;
		}
	}

	// �o�b�N�o�b�t�@��ύX��̑傫���ɒ���
	if (m_ResizeWidth != 0 && m_ResizeHeight != 0)
	{
		m_d3dpp.BackBufferWidth = m_ResizeWidth;
		m_d3dpp.BackBufferHeight = m_ResizeHeight;
		m_ResizeWidth = m_ResizeHeight = 0;
		ResetDevice();
	}

	return S_OK;
}

//===================================================
// �E�B���h�E�̐V�����T�C�Y�̐ݒ�
//===================================================
void CRenderer::SetReSize(const UINT Width, const UINT Height)
{
	m_ResizeWidth = Width;
	m_ResizeHeight = Height;
}
