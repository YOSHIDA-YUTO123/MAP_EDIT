//===================================================
//
// ���C�g [light.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include<vector>

//***************************************************
// ���C�g�̃N���X�̒�`
//***************************************************
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetDirectional(const D3DXCOLOR Diffuse, const D3DXVECTOR3 dir, const D3DXVECTOR3 pos); // �f�B���N�V���i�����C�g�̐ݒ菈��
	void SetPoint(const D3DXVECTOR3 pos,const float fRange,const D3DXCOLOR Diffuse,const D3DXCOLOR Ambient);			// �|�C���g���C�g�̐ݒ菈��
	void SetLight(void);

	D3DXVECTOR3 GetDir(void) const { return m_aLightInfo[0].aLight.Direction; }
	void DeleteLight(void); // �f�B���N�V�������C�g�ȊO������
private:

	static constexpr int MAX_LIGHT = 8; // ���C�g�̍ő吔

	// ���C�g�̏��
	struct LightInfo
	{
		D3DLIGHT9 aLight;	// ���C�g�̍\����
		bool bUse;			// �g�p���
	};

	LightInfo m_aLightInfo[MAX_LIGHT]; // ���C�g�̍\����
};
#endif