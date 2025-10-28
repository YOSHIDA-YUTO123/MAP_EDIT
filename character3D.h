//===================================================
//
// �L�����N�^�[�̍\�� [character.h]
// Author:YUTO YOSHIDA
// 
// << 3D�L�����N�^�[�̌����ځA���[�V�����������N���X >>
// 
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _CHARACTER3D_H_
#define _CHARACTER3D_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"object.h"
#include<memory>
#include<vector>

//***************************************************
// �O���錾
//***************************************************
class CShadowS;
class CRotation;
class CMotion;
class CModel;
class CCollisionObject3D;

//***************************************************
// �L�����N�^�[3D�N���X�̒�`
//***************************************************
class CCharacter3D : public CObject
{
public:

	// ��Ԃ̎��
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_MOVE,
		STATE_ACTION,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	// �L�����N�^�[�̎��
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_MAX
	}TYPE;

	CCharacter3D();
	CCharacter3D(const TYPE type);
	virtual ~CCharacter3D();

	static CCharacter3D* Create(void) { return new CCharacter3D; }

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	/// <summary>
	/// �N���[���̍쐬����
	/// </summary>
	/// <returns>�L�����N�^�[�̃|�C���^</returns>
	void Draw(const float fAvl);
	void DrawMT(void);
	
	void SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 Size);
	void SetModelMT(const char* pTextureName);

	void SetState(const STATE state,const int nTime);
	bool Hit(int nDamage);		// �q�b�g���̏���
	void UpdateMotion(void);

	CCollisionObject3D* GetCollisionObject(void) { return m_pCollObject; }

	TYPE GetType(void) const { return m_type; }
	CMotion* LoadMotion(const char* pFileName, const int nNumMotion);

	STATE GetState(void) { return m_state; }
	CMotion* GetMotion(void) { return m_pMotion.get(); } // ���[�V�����̎擾
	float GetSpeed(void) { return m_fSpeed; }
	int GetLife(void) const { return m_nLife; }
	void SetType(const TYPE type) { m_type = type; }

	D3DXVECTOR3 GetModelPos(const int nIdx);
	D3DXVECTOR3 GetModelRot(const int nIdx);
	D3DXVECTOR3 GetModelSize(const int nIdx);

private:
	CCollisionObject3D* m_pCollObject;		// �����蔻�肷��I�u�W�F�N�g
	std::unique_ptr<CMotion> m_pMotion;		// ���[�V�����̃N���X�ւ̃|�C���^
	std::vector<CModel*> m_apModel;			// ���f���N���X�̃|�C���^
	STATE m_state;							// ���
	TYPE m_type;							// ���
	float m_fSpeed;							// ���̑���
	int m_nNumModel;						// ���f���̍ő吔
	int m_nCounterState;					// ��Ԃ̃J�E���^�[
	int m_nLife;							// ����
};
#endif