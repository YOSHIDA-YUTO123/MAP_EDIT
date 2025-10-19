//===================================================
//
// ���C�g [light.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "light.h"
#include"manager.h"
#include"renderer.h"

//===================================================
// �R���X�g���N�^
//===================================================
CLight::CLight()
{
	ZeroMemory(m_aLightInfo, sizeof(m_aLightInfo));
}

//===================================================
// �f�X�g���N�^
//===================================================
CLight::~CLight()
{
	
}

//===================================================
// ����������
//===================================================
HRESULT CLight::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ���C�g�𖳌���
		pDevice->LightEnable(nCnt, FALSE);
	}

	// �l�̃N���A
	ZeroMemory(m_aLightInfo, sizeof(m_aLightInfo));

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CLight::Uninit(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ���C�g�𖳌���
		pDevice->LightEnable(nCnt, FALSE);
	}

	// �l�̃N���A
	ZeroMemory(m_aLightInfo, sizeof(m_aLightInfo));
}

//===================================================
// �X�V����
//===================================================
void CLight::Update(void)
{

}

//===================================================
// ���C�g�̐ݒ菈��
//===================================================
void CLight::SetDirectional(const D3DXCOLOR Diffuse,const D3DXVECTOR3 dir,const D3DXVECTOR3 pos)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g�̏��
	LightInfo info = {};

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// �g���Ă��Ȃ��Ȃ�
		if (m_aLightInfo[nCnt].bUse == false)
		{
			info.aLight.Type = D3DLIGHT_DIRECTIONAL;
			info.aLight.Diffuse = Diffuse;

			// ���C�g�̕����x�N�g��
			D3DXVECTOR3 Direction = dir;

			// ���K������
			D3DXVec3Normalize(&Direction, &Direction);

			// �����x�N�g���̑��
			info.aLight.Direction = Direction;

			// �ʒu�̐ݒ�
			info.aLight.Position = pos;

			info.bUse = true;

			// ���C�g�̐ݒ�
			pDevice->SetLight(nCnt, &info.aLight);

			// ���C�g��L����
			pDevice->LightEnable(nCnt, TRUE);

			// ����ݒ�
			m_aLightInfo[nCnt] = info;

			break;
		}
	}
}

//===================================================
// �|�C���g���C�g�̐ݒ菈��
//===================================================
void CLight::SetPoint(const D3DXVECTOR3 pos, const float fRange, const D3DXCOLOR Diffuse, const D3DXCOLOR Ambient)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g�̏��
	LightInfo info = {};

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// �g���Ă��Ȃ��Ȃ�
		if (m_aLightInfo[nCnt].bUse == false)
		{
			info.aLight.Type = D3DLIGHT_POINT;
			info.aLight.Diffuse = Diffuse;

			// ���C�g�̈ʒu
			info.aLight.Position = pos;

			info.aLight.Diffuse = Diffuse;

			info.aLight.Ambient = Ambient;

			info.aLight.Specular = Diffuse;
			info.aLight.Attenuation0 = 0.0f;
			info.aLight.Attenuation1 = 0.004f;
			info.aLight.Attenuation2 = 0.00f;
			info.aLight.Range = fRange;

			info.bUse = true;

			// ���C�g�̐ݒ�
			pDevice->SetLight(nCnt, &info.aLight);

			// ���C�g��L����
			pDevice->LightEnable(nCnt, TRUE);

			// ���C�g�̏��̐ݒ�
			m_aLightInfo[nCnt] = info;

			break;
		}
	}


}

//===================================================
// ���C�g�̐ݒ菈��
//===================================================
void CLight::SetLight(void)
{
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// ���C�g�̐ݒ�
		pDevice->SetLight(nCnt, &m_aLightInfo[nCnt].aLight);

		// ���C�g��L����
		pDevice->LightEnable(nCnt, TRUE);
	}
}

//===================================================
// �f�B���N�V�������C�g�ȊO�̏���
//===================================================
void CLight::DeleteLight(void)
{
	// ���C�g�̍ő吔�����ׂ�
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ���C�g���g���Ă��邩�f�B���N�V�������C�g�ȊO��������
		if (m_aLightInfo[nCnt].bUse == true && m_aLightInfo[nCnt].aLight.Type != D3DLIGHT_DIRECTIONAL)
		{
			// �f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

			// ���C�g��L����
			pDevice->LightEnable(nCnt, FALSE);
		}
	}
}
