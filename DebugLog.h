//===================================================
//
// �f�o�b�O���O [DebugLog.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// ��d�C���N���[�h�h�~
//***************************************************
#ifndef _DEBUGLOG_H_
#define _DEBUGLOG_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"
#include <string>

//***************************************************
// �f�o�b�O���O�N���X�̒�`
//***************************************************
class CDebugLog : public CObject
{
public:
	~CDebugLog();

	static CDebugLog* Create(void);
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Set(const char* pMessage, const int nTime);
private:
	CDebugLog();

	// ���
	struct Info
	{
		std::string aMessage; // �f�o�b�O���
		int nTime;			  // ����
	};

	std::vector<Info> m_Info; // �f�o�b�O���
};
#endif