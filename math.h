//===================================================
//
// �v�Z [math.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MATH_H_
#define _MATH_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include <random>

// �͈͓��Ɏ��߂�
template <class T> T Clamp(const T value, const T min, const T max)
{
	if (value < min) return min;

	if (value > max) return max;

	return value;
};

// �͈͓��ɂ����[�v����
template <class T> T Wrap(const T value, const T min, const T max)
{
	if (value <= min) return max;

	if (value >= max) return min;

	return value;
};

// �p�x�𐳋K������
template <class T> T NormalizeRot(T* value)
{
	if ((*value) < -D3DX_PI)
	{
		(*value) += D3DX_PI * 2.0f;
	}
	else if ((*value) >D3DX_PI)
	{
		(*value) += -D3DX_PI * 2.0f;
	}

	return (*value);
};

// �ړI�̊p�x�܂ł̋��������ߐ��K������
template <class T> T NormalizeDiffRot(const T Diff, T* value)
{
	if (Diff <= -D3DX_PI)
	{
		(*value) += -D3DX_PI * 2.0f;
	}
	else if (Diff >= D3DX_PI)
	{
		(*value) += D3DX_PI * 2.0f;
	}

	return *value;
};

// �ړI�̒l�܂ł̕��
template <class T> T LerpDest(const T dest,const T value,const float coef)
{
	T out = value + (dest - value) * coef;

	return out;
};

// �ړI�̒l�܂ł̕��
template <class T> T LerpDiff(const T offset, const T Diff, const float fRate)
{
	T out = offset + (Diff * fRate);

	return out;
};

// 
template <class T> bool IsInRange(const T value,const T min,const T max)
{
	if (value >= min && value <= max)
	{
		return true;
	}

	return false;
};

// �����_��
template <class T> T Random(const T min, const T max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<T> dist(min, max);
	
	return dist(gen);
}

//***************************************************
// �v�Z�pnamespace�̒�`
//***************************************************
namespace math
{
	/**
	* @brief ���������߂�֐�
	* @param ��̈ʒu�̍���
	* @return �v�Z��̋���
	*/
	float GetDistance(const D3DXVECTOR3 diff);
	/**
	* @brief �P�ʃx�N�g�����擾����֐�
	* @param �ړI�̈ʒu
	* @param �ʒu
	* @return ���K�����ꂽ�x�N�g��
	*/
	D3DXVECTOR3 GetVector(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos);
	float GetTargetAngle(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos); // �ڕW�܂ł̊p�x�̎擾
	D3DXVECTOR3 GetPositionFromMatrix(const D3DXMATRIX mtxWorld);		 // MATRIX�̈ʒu�̏���D3DXVECTOR3�^�ɕϊ�����
	D3DXQUATERNION CreateQuaternion(D3DXQUATERNION* pOut, D3DXVECTOR3 axis, float fAngle);
	D3DXMATRIX SetMatrixRotationQuaternion(D3DXMATRIX* pOut, D3DXQUATERNION quaternion);
	D3DXVECTOR3 MatrixToEulerXYZ(const D3DXMATRIX mtxWorld);			// ��]�s����I�C���[�p�ɕϊ�

	/// <summary>
	/// �}�E�X�̃��C�̎擾
	/// </summary>
	/// <param name="pRayOrigin"></param>
	/// <param name="OutDir"></param>
	void GetMouseRay(D3DXVECTOR3* pRayOrigin, D3DXVECTOR3* OutDir);

	/// <summary>
	/// �X�N���[�����W�����[���h���W�ɕϊ�
	/// </summary>
	/// <param name="screen"></param>
	/// <param name="fWorldY"></param>
	/// <param name="vp"></param>
	/// <param name="view"></param>
	/// <param name="proj"></param>
	/// <returns>�ϊ����W</returns>
	D3DXVECTOR3 ScreenToWorld(const D3DXVECTOR2 screen, const float fWorldY, const D3DVIEWPORT9 vp, const D3DXMATRIX view, const D3DXMATRIX proj);
};
#endif