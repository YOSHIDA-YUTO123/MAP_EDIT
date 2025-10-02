//===================================================
//
// �}�b�v�ɔz�u����I�u�W�F�N�g�̃N���X [MapObject.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"
#include <string>

//***************************************************
// �}�b�v�̃I�u�W�F�N�g�̃N���X�̒�`
//***************************************************
class CMapObject : public CObject
{
public:
	CMapObject();
	~CMapObject();

	static CMapObject* Create(const D3DXVECTOR3 pos, const char* pModelFileName);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetALv(const float fAlv) { m_fAlv = fAlv; }
	void Register(const char* pModelFileName);

	/// <summary>
	/// �}�E�X�ƃ��f���̓����蔻��
	/// </summary>
	/// <param name="pDistance"></param>
	/// <returns>���茋��</returns>
	bool CollisionMouse(float *pDistance);

	/// <summary>
	/// ���̐ݒ�(Imgui���g�p����)
	/// </summary>
	void SetInfo(void);

	void SetMouseDrag(void);
private:
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	float m_fAlv;				// �A���t�@�l
	int m_nModelIdx;			// ���f���̃C���f�b�N�X
	bool m_bMouseUp;			// �}�E�X�𗣂������ǂ���
};

#endif
