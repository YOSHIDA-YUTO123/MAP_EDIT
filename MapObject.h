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
#include "CollisionObject3D.h"
#include <string>

//***************************************************
// �}�b�v�̃I�u�W�F�N�g�̃N���X�̒�`
//***************************************************
class CMapObject : public CCollisionObject3D
{
public:

	// ���
	typedef enum
	{
		TYPE_STATIC = 0,
		TYPE_COLLISION,
		TYPE_MAX
	}TYPE;

	CMapObject();
	~CMapObject();

	static CMapObject* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName, const char* pType);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetALv(const float fAlv) { m_fAlv = fAlv; }
	void Register(const char* pModelFileName);
	void UpdateMove(void);
	void SetMove(const float fMove) { m_fMove = fMove; }

	const char* GetPath(void) const { return m_aModelPath.c_str(); }
	const char* GetType(void) const { return m_aType.c_str(); }

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

	std::string m_aModelPath;	// ���f���̃p�X
	std::string m_aType;		// ��ނ̏��
	float m_fAlv;				// �A���t�@�l
	float m_fMove;				// �ړ���
	int m_nModelIdx;			// ���f���̃C���f�b�N�X
	bool m_bMouseUp;			// �}�E�X�𗣂������ǂ���
};

#endif
