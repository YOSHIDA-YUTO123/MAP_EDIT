//===================================================
//
// キャラクターマネージャー [CharacterManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "CharacterManager.h"
#include "character3D.h"
#include "motion.h"
#include "player.h"

using namespace std; // 名前空間stdの使用

//***************************************************
// 静的メンバ変数宣言
//***************************************************
unique_ptr<CCharacterManager> CCharacterManager::m_pInstance = nullptr; // 自分のインスタンス

//===================================================
// コンストラクタ
//===================================================
CCharacterManager::CCharacterManager()
{
	for (auto& list : m_pCharacter)
	{
		list = nullptr;
	}
}

//===================================================
// デストラクタ
//===================================================
CCharacterManager::~CCharacterManager()
{
}

//===================================================
// 生成処理
//===================================================
void CCharacterManager::Create(void)
{
	if (m_pInstance == nullptr)
	{
		// 自分の生成
		m_pInstance.reset(new CCharacterManager);
		m_pInstance->Init();
	}
}

//===================================================
// キャラクターの設定
//===================================================
void CCharacterManager::SetCharacter(CCharacter3D* pCaracther)
{
	// 種類の取得
	auto Type = pCaracther->GetType();

	if (m_pCharacter[Type] == nullptr)
	{
		// 非表示
		pCaracther->SetOffShow();
		m_pCharacter[Type] = pCaracther;
	}
}

//===================================================
// キャラクターの生成
//===================================================
CCharacter3D* CCharacterManager::CreateCharacter(CCharacter3D::TYPE type)
{
	CCharacter3D* templateChar = m_pCharacter[type];

	if (m_pCharacter[type] != nullptr)
	{
		return templateChar->Clone();
	}

	return nullptr;
}

//===================================================
// 初期化処理
//===================================================
void CCharacterManager::Init(void)
{
	auto templateChar = new CPlayer;

	templateChar->LoadMotion("motion.txt", 5);

	SetCharacter(templateChar);

	CCharacter3D* p = CreateCharacter(CCharacter3D::TYPE_PLAYER);
}


////===================================================
//// モーションの情報の設定
////===================================================
//void CCharacterManager::SetMotion(CMotion* pMotion, const CCharacter3D::TYPE type)
//{
//	// モーションの情報の設定
//	pMotion->GetInfo(&m_aConfig[type].Motion);
//	m_aConfig[type].type = type;
//}
//
////===================================================
//// モデルの設定
////===================================================
//void CCharacterManager::SetModel(CModel* pModel, const CCharacter3D::TYPE type, const int nNumModel,const int nIdx)
//{
//	CModel modelWk = {};
//
//	// モデルの情報のコピー
//	pModel->Copy(&modelWk);
//
//	// サイズの確保
//	m_aConfig[type].aModelList.resize(nNumModel);
//
//	// 親モデルのID
//	int nParentIdx = pModel->GetParentID();
//
//	if (nParentIdx != -1)
//	{
//		// 親モデルの設定
//		modelWk.SetParent(&m_aConfig[type].aModelList[nParentIdx], nParentIdx);
//	}
//	else
//	{
//		// 親モデルの設定
//		modelWk.SetParent(nullptr, nParentIdx);
//	}
//
//	// モデルの名前の取得
//	const char* pModelName = pModel->GetModelName();
//
//	// モデルの名前の取得
//	modelWk.SetModelName(pModelName);
//
//	// 情報の設定
//	m_aConfig[type].aModelList[nIdx] = modelWk;
//
//	m_aConfig[type].type = type;
//}
//
////===================================================
//// モーションのリストの設定
////===================================================
//void CCharacterManager::GetMotion(CMotion* pMotion, const CCharacter3D::TYPE type)
//{
//	// 情報の取得
//	m_aConfig[type].Motion.GetInfo(pMotion);
//}
//
////===================================================
//// モデルの取得
////===================================================
//void CCharacterManager::GetModel(CModel** pModel, const CCharacter3D::TYPE type, const int nIdx,int *pParentIndx)
//{
//	// モデルの名前の取得
//	const char* pModelName = m_aConfig[type].aModelList[nIdx].GetModelName();
//
//	// モデルの生成
//	(*pModel) = CModel::Create(pModelName);
//
//	// モデルの情報のコピー
//	m_aConfig[type].aModelList[nIdx].Copy((*pModel));
//
//	// 親のインデックスの取得
//	int nParentIdx = m_aConfig[type].aModelList[nIdx].GetParentID();
//
//	*pParentIndx = nParentIdx;
//}
