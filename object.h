//===================================================
//
// �I�u�W�F�N�g�̕`�揈�� [object.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include<vector>

class CObstacle;
class CObstacleOwner;

//***************************************************
// �I�u�W�F�N�g�N���X�̒�`
//***************************************************
class CObject
{
public:

	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_OBSTACLE,
		TYPE_PAUSE,
		TYPE_MAX
	}TYPE;

	CObject(int nPriority = 3);
	virtual ~CObject();

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static int GetNumObject(const int nPriority) { return m_nNumAll[nPriority]; }
	static void DestroyAll(void);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	int GetPriority(void) const { return m_nPriority; }
	void SetType(const TYPE type) { m_type = type; }
	TYPE GetType(void) const { return m_type; }
	bool GetDeath(void) const { return m_bDeath; }	// ���S�t���O�̎擾

protected:
	void Release(void);								// ���S�t���O�𗧂Ă�
private:
	void Destroy(CObject* pObject);
	static int m_nNumAll[Const::NUM_PRIORITY];		// �I�u�W�F�N�g�̑���
	static CObject* m_pTop[Const::NUM_PRIORITY];	// �擪�I�u�W�F�N�g�ւ̃|�C���^
	static CObject* m_pCur[Const::NUM_PRIORITY];	// �Ō���I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pPrev;								// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNext;								// ���̃I�u�W�F�N�g�ւ̃|�C���^
	TYPE m_type;									// �I�u�W�F�N�g�̎��
	int m_nPriority;								// �D�揇��
	bool m_bDeath;									// ���S�t���O
};

#endif