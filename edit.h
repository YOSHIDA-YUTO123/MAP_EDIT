//===================================================
//
// �ҏW���[�h����Imgui�̏������܂Ƃ߂�N���X [edit.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"

//***************************************************
// �ҏW���[�h�N���X�̒�`
//***************************************************
class CEdit : public CObject
{
public:
	CEdit();
	~CEdit();

	static CEdit* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};
#endif