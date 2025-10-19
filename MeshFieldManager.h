//===================================================
//
// ���b�V���t�B�[���h�̃}�l�[�W���[�N���X�̒�` [MeshFieldManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MESHFIELDMANAGER_H_
#define _MESHFIELDMANAGER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"
#include <memory>

//***************************************************
// �O���錾
//***************************************************
class CMeshCylinder;
class CColliderSphere;
class CMeshField;
class CTransform;

//***************************************************
// ���b�V���t�B�[���h�̃}�l�[�W���[�N���X�̒�`
//***************************************************
class CMeshFieldManager
{
public:
	~CMeshFieldManager();

	static CMeshFieldManager* GetInstance(void);

	HRESULT Init(void);
	void Update(void);
	void SetShow(const bool bEnable);
private:
	static constexpr float ADD_HEIGHT = 5.0f;		// �����̉��Z�l
	static constexpr float BRUSH_RADIUS = 100.0f;	// �u���V�̔��a

	CMeshFieldManager();
	void SetBrush(void);
	void SetVtx(CMeshField *pMeshField);

	static std::unique_ptr<CMeshFieldManager> m_pInstance; // �����̃C���X�^���X
	std::unique_ptr<CColliderSphere> m_pBrushSphere;	   // �u���V�̉~�̔���
	std::unique_ptr<CTransform> m_pTransform;			   // ��ԏ��̃|�C���^
	CMeshCylinder* m_pMeshCylinder;						   // �V�����_�[�̎擾
	D3DXCOLOR m_col;									   // �F
	D3DXVECTOR3 m_BrushPos;								   // �u���V�̈ʒu
	float m_fAddHeightValue;							   // �����̉��Z�l
	float m_fBrushRadius;								   // �u���V�̔��a
	bool m_bShow;										   // �\�����邩�ǂ���
};

#endif
