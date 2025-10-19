//===================================================
//
// �v���C���[ [player.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "character3D.h"

//===================================================
// �v���C���[�N���X�̒�`
//===================================================
class CPlayer : public CCharacter3D
{
public:
	CPlayer();
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	CCharacter3D* Clone(void) const override;

private:
};
#endif
