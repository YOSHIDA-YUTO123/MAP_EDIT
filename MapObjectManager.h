//===================================================
//
// �}�b�v�I�u�W�F�N�g�̃}�l�[�W���[ [MapObjectManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MAPOBJECTMANAGER_H_
#define _MAPOBJECTMANAGER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "main.h"
#include <vector>
#include <memory>
#include <string>

//***************************************************
// �O���錾
//***************************************************
class CMapObject;
class CImGuiManager;
class CEditMapObject;

//***************************************************
// �}�b�v�I�u�W�F�N�g�̃��X�g�N���X
//***************************************************
class CMapObjectManager
{
public:
	~CMapObjectManager();

	/// <summary>
	/// �����̃C���X�^���X�̎擾
	/// </summary>
	/// <returns>�����̃C���X�^���X</returns>
	static CMapObjectManager* GetInstance(void);

	/// <summary>
	/// �}�b�v�I�u�W�F�N�g�̐�������
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="pModelFileName"></param>
	/// <returns>�}�b�v�I�u�W�F�N�g�̃C���X�^���X</returns>
	CMapObject* Create(const D3DXVECTOR3 pos, const char* pModelFileName);

	/// <summary>
	/// �}�b�v�I�u�W�F�N�g�̃��X�g�̎擾
	/// </summary>
	/// <returns>vector�^�̃I�u�W�F�N�g�̃��X�g</returns>
	std::vector<CMapObject*>& GetList(void) { return m_pMapObjectList; }

	HRESULT Init(void);
	void Update(void);
private:
	CMapObjectManager();

	void GetModelPath(void);
	void SetModelPathList(void);
	void SetSelectObj(void);
	void SetInspector(void);
	void UpdateEditMapObj(void);
	bool CollisionMouse(void);
	HRESULT Register(void);

	static std::unique_ptr<CMapObjectManager> m_pInstance; // �����̃C���X�^���X
	static std::vector<CMapObject*> m_pMapObjectList;	   // �}�b�v�I�u�W�F�N�g�̃��X�g
	static std::vector<std::string> m_aModelPath;		   // ���f���̃p�X�̃��X�g
	CEditMapObject* m_pEditMapObj;						   // �ҏW�p�}�b�v�I�u�W�F�N�g
	CMapObject* m_pSelect;								   // �I�𒆂̃}�b�v�I�u�W�F�N�g
	int m_nType;										   // ���f���̎��
};

#endif
