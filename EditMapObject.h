//===================================================
//
// 編集中のオブジェクトの表示用クラス [EditMapObject.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CEDITMAPOBJECT_H_
#define _CEDITMAPOBJECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "objectX.h"
#include <string>

//***************************************************
// 編集中の透明なオブジェクトのクラスの定義
//***************************************************
class CEditMapObject : public CObjectX
{
public:
	CEditMapObject();
	~CEditMapObject();

	static CEditMapObject* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetShow(const bool bEnable) { m_bShow = bEnable; }
	bool CollisionMouse(float* pDistance);
private:
	std::string m_aModelName; // モデルの名前
	bool m_bShow;			  // 表示するかどうか
	bool m_bMouseUp;		  // マウスを離したかどうか
};

#endif
