//===================================================
//
// �J���� [camera.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"

//***************************************************
// �J�����N���X�̒�`
//***************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetCamera(void);
	void SetCamera(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot);
	void MouseWheel(void);
	void MouseView(void);
	void PadView(void);

	// �Q�b�^�[
	D3DXVECTOR3 GetRotaition(void) const { return m_rot; }
	D3DXVECTOR3 GetPosV(void) const { return m_posV; }
	D3DXVECTOR3 GetPosR(void) const { return m_posR; }
	D3DXVECTOR3 GetPosOld(void) const { return m_posVOld; }
	void LerpPos(const D3DXVECTOR3 posRDest, const D3DXVECTOR3 posVDest, const float fCoef);	// �ʒu��ړI�̈ʒu�ɋ߂Â���

	// �Z�b�^�[
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosV(const D3DXVECTOR3 posV) { m_posV = posV; }

	// �Q�b�^�[
	D3DXVECTOR3 GetDestPosV(void) const { return m_posVDest; }
	D3DXVECTOR3 GetDestPosR(void) const { return m_posRDest; }
	float GetDistance(void) const { return m_fDistance; }
	void UpdatePositionV(void);			// ���_�̍X�V����
	void UpdatePositionR(void);			// �����_�̍X�V����

	// �Z�b�^�[
	void SetDestPosV(const D3DXVECTOR3 destpos) { m_posVDest = destpos; }
	void SetPosR(const D3DXVECTOR3 posR) { m_posR = posR; }
	void SetDestPosR(const D3DXVECTOR3 destpos) { m_posRDest = destpos; }
	void SetDistance(const float fDistance) { m_fDistance = fDistance; }
private:
	D3DXMATRIX m_mtxView;		// �r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 m_posV;			// ���_
	D3DXVECTOR3 m_posVOld;		// �O��̈ʒu
	D3DXVECTOR3 m_posR;			// �����_
	D3DXVECTOR3 m_vecU;			// ������x�N�g��
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_posRDest;		// �ړI�̒����_
	D3DXVECTOR3 m_posVDest;		// �ړI�̎��_
	float m_fDistance;			// ����
	float m_fFov;				// �J�����̎���p
	bool m_bCollision;			//
};

#endif