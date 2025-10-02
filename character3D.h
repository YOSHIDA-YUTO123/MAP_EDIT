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
		TYPE_MAX
	}TYPE;

	CCharacter3D();
	CCharacter3D(const TYPE type);
	virtual ~CCharacter3D();

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	void Draw(const float fAvl);
	void DrawMT(void);
	
	TYPE GetType(void) const { return m_type; }
	CMotion* LoadMotion(const char *pFileName, const int nNumMotion);

	// �Q�b�^�[
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetSize(void) const { return m_Size; }
	D3DXVECTOR3 GetRotaition(void) const { return m_rot; }

	STATE GetState(void) { return m_state; }
	CMotion* GetMotion(void) { return m_pMotion.get(); } // ���[�V�����̎擾
	float GetSpeed(void) { return m_fSpeed; }
	int GetLife(void) const { return m_nLife; }
	D3DXVECTOR3 GetModelPos(const int nIdx);
	D3DXVECTOR3 GetModelRot(const int nIdx);
	D3DXVECTOR3 GetModelSize(const int nIdx);
	D3DXMATRIX GetParent(const int nIdx) const;

	// �Z�b�^�[
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotaition(const D3DXVECTOR3 rot) { m_rot = rot; }

	void SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 Size);
	void SetModelMT(const char* pTextureName);
	void Copy(CCharacter3D* pCharacter);

	bool GetAlive(void);		// �����Ă��邩

	void SetState(const STATE state,const int nTime);
	bool Hit(int nDamage);		// �q�b�g���̏���
	bool HitStop(void);			// �q�b�g�X�g�b�v���Ă邩�ǂ���
	void SetHitStop(const int nTime) { m_nHitStopTime = nTime; } // �q�b�g�X�g�b�v�̐ݒ�
	void UpdateMotion(void);

protected:
	void SetCharacter(void);
private:

	std::unique_ptr<CMotion> m_pMotion;		// ���[�V�����̃N���X�ւ̃|�C���^
	std::vector<CModel*> m_apModel;			// ���f���N���X�̃|�C���^
	STATE m_state;							// ���
	TYPE m_type;							// ���
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_Size;						// �傫��
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	float m_fSpeed;							// ���̑���
	int m_nNumModel;						// ���f���̍ő吔
	int m_nCounterState;					// ��Ԃ̃J�E���^�[
	int m_nLife;							// ����
	int m_nHitStopTime;						// �q�b�g�X�g�b�v�̎���
};
#endif