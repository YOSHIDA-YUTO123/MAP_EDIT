//================================================
//
// �|�[�Y [pause.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "object2D.h"
#include <memory>

//************************************************
// �|�[�Y�N���X�̒�`
//************************************************
class CPause : public CObject2D
{
public:

	// �|�[�Y�̃��j���[
	enum TYPE
	{
		TYPE_CONTINUE = 0, // �R���e�j���[
		TYPE_RETRY,        // ���g���C
		TYPE_QUIT,		   // ��߂�
		TYPE_MAX
	};

	virtual ~CPause();

	static CPause* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size,const TYPE type);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override = 0;
	virtual void Draw(void) override;

protected:
	CPause(const TYPE type);

	D3DXVECTOR2 GetBaseSize(void) const { return m_BaseSize; }
	TYPE GetType(void) const { return m_Type; }

	void SetBaseSize(const D3DXVECTOR2 Size) { m_BaseSize = Size; }
private:
	TYPE m_Type;					// �|�[�Y�̃��j���[
	D3DXVECTOR2 m_BaseSize;			// ��̑傫��
};

//************************************************
// �|�[�Y(�R���e�j���[)�N���X�̒�`
//************************************************
class CPauseContinue : public CPause
{
public:
	CPauseContinue();
	~CPauseContinue();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};

//************************************************
// �|�[�Y(���g���C)�N���X�̒�`
//************************************************
class CPauseRetry : public CPause
{
public:
	CPauseRetry();
	~CPauseRetry();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};

//************************************************
// �|�[�Y(��߂�)�N���X�̒�`
//************************************************
class CPauseQuit : public CPause
{
public:
	CPauseQuit();
	~CPauseQuit();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};

//************************************************
// �|�[�Y�̃}�l�[�W���[�N���X�̒�`
//************************************************
class CPauseManager
{
public:
	~CPauseManager();
	static CPauseManager* GetInstance(void) { return m_pInstance.get(); }
	static void Create(void);

	void Uninit(void);
	void EnablePause(void);									   // �����Ȃ�
	void EnablePause(const bool bPause) { m_bPause = bPause; } // ��������
	void SelectMenu(void);
	static bool GetPause(void) { return m_bPause; }
	CPause::TYPE GetSelectMenu(void) const { return m_SelectMenu; }
private:
	CPauseManager();
	CPause::TYPE m_SelectMenu;						   // �|�[�Y�̃��j���[�̑I��
	static std::unique_ptr<CPauseManager> m_pInstance; // �����̃C���X�^���X
	static bool m_bPause; // �|�[�Y��Ԃ��ǂ���
};

//************************************************
// �|�[�Y(�|�[�Y��)�̃N���X�̒�`
//************************************************
class CPauseNow : public CObject2D
{
public:
	CPauseNow();
	~CPauseNow();

	static CPauseNow* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size);
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	D3DXVECTOR2 m_BaseSize;	// ���̑傫��
	float m_fCounter;		// �J�E���^�[
};
#endif