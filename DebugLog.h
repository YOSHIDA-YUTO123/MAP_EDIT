//===================================================
//
// デバッグログ [DebugLog.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// 二重インクルード防止
//***************************************************
#ifndef _DEBUGLOG_H_
#define _DEBUGLOG_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include <string>

//***************************************************
// デバッグログクラスの定義
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

	// 情報
	struct Info
	{
		std::string aMessage; // デバッグ情報
		int nTime;			  // 時間
	};

	std::vector<Info> m_Info; // デバッグ情報
};
#endif