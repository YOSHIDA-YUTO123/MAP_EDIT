//===================================================
//
// �v���R���p�C�� [pch.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _PCH_H_
#define _PCH_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#define _CRT_SECURE_NO_WARNINGS		 // scanf�̌x���Ώ��}�N��
#include<windows.h>
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800) // �r���h���̌x���Ώ��}�N��
#include "dinput.h"
#include"Xinput.h"
#include"xaudio2.h"

//***************************************************
// ���C�u�����̃����N
//***************************************************
#pragma comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�l�[�V����(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	// �V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	// �W���C�p�b�h�����ɕK�v
#pragma comment(lib, "Shlwapi.lib") // 

//***************************************************
// �}�N����`
//***************************************************
#define CLASS_NAME "WindowClass"	// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME "Map_Viewer"	// �E�C���h�E�̖��O(�L���v�V�����ɕ\��)
#define SCREEN_WIDTH (1280)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT (720)			// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)			     // 2D�̒��_���
#define FVF_VERTEX_2DMT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)				 // 2D�}���`�e�N�X�`���̒��_���
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX1)   // ���W,�@��,�J���[,�e�N�X�`��
#define FVF_VERTEX_3DMT (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX2)   // ���W,�@��,�J���[,�e�N�X�`��2

//***************************************************
// �萔�̖��O���
//***************************************************
namespace Const
{
	const D3DXVECTOR3 VEC3_NULL = { 0.0f,0.0f,0.0f };		// VECTOR3�̏�����
	const D3DXVECTOR2 VEC2_NULL = { 0.0f,0.0f };			// VECTOR2�̏�����
	const D3DXVECTOR3 CENTER_POS_2D = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	const D3DXCOLOR WHITE = { 1.0f,1.0f,1.0f,1.0f };		// �F�̏����J���[
	constexpr int FRAME = 60;								// ��t���[��
	constexpr int MAX_WORD = 5024;							// �ő�̕�����
	constexpr float MAX_GRABITY = 1.0f;						// �d�͗�
	constexpr int NUM_PRIORITY = 8;							// �I�u�W�F�N�g�̗D�揇�ʂ̐�
}

//**********************
// ���_���[2D]�̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;				// ���_���W
	float rhw;						// ���W�ԊҌW��(1.0f�Ŏw��)
	D3DCOLOR col;					// ���_�J���[
	D3DXVECTOR2 tex;				// �e�N�X�`�����W
}VERTEX_2D;

//**********************
// ���_���[3D]�̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// ���_���W
	float rhw;					// ���W�ԊҌW��(1.0f�Ŏw��)
	D3DCOLOR col;				// ���_�J���[
	D3DXVECTOR2 tex;			// �e�N�X�`�����W
	D3DXVECTOR2 texMT;			// �e�N�X�`�����WMT
}VERTEX_2D_MULT;

//**********************
// ���_���[3D]�̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// ���_���W
	D3DXVECTOR3 nor;			// �@���x�N�g��
	D3DCOLOR col;				// ���_�J���[
	D3DXVECTOR2 tex;			// �e�N�X�`�����W
}VERTEX_3D;

//**********************
// ���_���[3D]�̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// ���_���W
	D3DXVECTOR3 nor;			// �@���x�N�g��
	D3DCOLOR col;				// ���_�J���[
	D3DXVECTOR2 tex;			// �e�N�X�`�����W
	D3DXVECTOR2 texMT;			// �e�N�X�`�����WMT
}VERTEX_3D_MULT;

#endif
