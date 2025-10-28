//===================================================
//
// �}�l�[�W���[ [manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "main.h"
#include <memory>
#include <vector>

//***************************************************
// �O���錾
//***************************************************
class CCollisionManager;
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CTextureManager;
class CTextureMTManager;
class CLight;
class CModelManager;
class CCamera;
class CDebugLog;
class CMeshField;
class CMapObjectList;
class CSystemBase;

//***************************************************
// �}�l�[�W���[�N���X�̒�`
//***************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer* GetRenderer(void);
	static CInputKeyboard* GetInputKeyboard(void);
	static CInputJoypad* GetInputJoypad(void);
	static CInputMouse* GetInputMouse(void);
	static CSound* GetSound(void);
	void SetFps(const int fps) { m_fps = fps; }
	static int GetFrameCounter(void);
	static bool GetShowDebug(void) { return m_bShowDebug; }
	static CTextureManager* GetTexture(void);
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void);
	static CModelManager* GetModel(void);
	static CDebugLog* GetDebugLog(void) { return m_pDebugLog; }
	static CTextureMTManager* GetTextureMT(void) { return m_pTexutreMTManager; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CMapObjectList* GetMapObjectList(void) { return m_pMapObjectList.get(); }
	static void AddSystem(std::unique_ptr<CSystemBase> pSystem);
private:
	void Load(void);

	static CRenderer* m_pRenderer;					// �����_���[�̃|�C���^
	static CInputKeyboard* m_pInputKeyboard;		// �L�[�{�[�h�̃|�C���^
	static CInputJoypad* m_pInputJoypad;			// �p�b�h�̃|�C���^
	static CInputMouse* m_pInputMouse;				// �}�E�X�̃|�C���^
	static CSound* m_pSound;						// �T�E���h�̃|�C���^
	static CTextureManager* m_pTexture;				// �e�N�X�`���N���X�ւ̃|�C���^	
	static CCamera* m_pCamera;						// �J�����̃|�C���^
	static CLight* m_pLight;						// ���C�g�ւ̃|�C���^
	static CModelManager* m_pModel;					// ���f���N���X�ւ̃|�C���^
	static CDebugLog* m_pDebugLog;					// �f�o�b�O���O�̃N���X�ւ̃|�C���^
	static CMeshField* m_pMeshField;				// ���b�V���t�B�[���h�N���X�ւ̃|�C���^
	static CTextureMTManager* m_pTexutreMTManager;  // �e�N�X�`��MT�̃}�l�[�W���[
	static std::unique_ptr<CMapObjectList> m_pMapObjectList; // �}�b�v�̃I�u�W�F�N�g�̃��X�g
	static int m_nFrameCounter;						// �t���[���J�E���^�[
	static bool m_bShowDebug;						// �f�o�b�O�\�������邩���Ȃ���
	static std::vector<std::unique_ptr<CSystemBase>> m_pSystem;
	int m_fps;										// fps�i�[�p�ϐ�
};
#endif