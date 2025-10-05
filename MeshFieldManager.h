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

//***************************************************
// ���b�V���t�B�[���h�̃}�l�[�W���[�N���X�̒�`
//***************************************************
class CMeshFieldManager : public CObject
{
public:
	~CMeshFieldManager();

	static CMeshFieldManager* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	CMeshFieldManager();
};

#endif
