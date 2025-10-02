//===================================================
//
// �ҏW���̃I�u�W�F�N�g�̕\���p�N���X [EditMapObject.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _CEDITMAPOBJECT_H_
#define _CEDITMAPOBJECT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "objectX.h"
#include <string>

//***************************************************
// �ҏW���̓����ȃI�u�W�F�N�g�̃N���X�̒�`
//***************************************************
class CEditMapObject : public CObjectX
{
public:
	CEditMapObject();
	~CEditMapObject();

	static CEditMapObject* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetShow(const bool bEnable) { m_bShow = bEnable; }
	bool CollisionMouse(float* pDistance);
private:
	std::string m_aModelName; // ���f���̖��O
	bool m_bShow;			  // �\�����邩�ǂ���
	bool m_bMouseUp;		  // �}�E�X�𗣂������ǂ���
};

#endif
