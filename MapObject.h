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

	static CMapObject* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetALv(const float fAlv) { m_fAlv = fAlv; }
	void Register(const char* pModelFileName);
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void UpdateMove(void);
	void SetMove(const float fMove) { m_fMove = fMove; }

	const char* GetPath(void) const { return m_aModelPath.c_str(); }
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }

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
	static constexpr float MOVE_VALUE = 5.0f; // �ړ���

	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	std::string m_aModelPath;	// ���f���̃p�X
	float m_fAlv;				// �A���t�@�l
	float m_fMove;				// �ړ���
	int m_nModelIdx;			// ���f���̃C���f�b�N�X
	bool m_bMouseUp;			// �}�E�X�𗣂������ǂ���
};

#endif
