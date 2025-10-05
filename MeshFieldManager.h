//===================================================
//
// メッシュフィールドのマネージャークラスの定義 [MeshFieldManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESHFIELDMANAGER_H_
#define _MESHFIELDMANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// メッシュフィールドのマネージャークラスの定義
//***************************************************
class CMeshFieldManager : public CObject
{
public:
	~CMeshFieldManager();

	static CMeshFieldManager* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	CMeshFieldManager();
};

#endif
