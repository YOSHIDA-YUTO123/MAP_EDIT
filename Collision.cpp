//================================================
//
// 当たり判定 [collision.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "Collision.h"
#include"math.h"
#include"Collider.h"
#include "transform.h"

using namespace Const; // 名前空間Constを使用
using namespace math; // 名前空間を使用
using namespace std; // 名前空間stdを処理使用

constexpr float HALF_VALUE = 0.5f; // 半分

//================================================
// コンストラクタ
//================================================
CCollision::CCollision(TYPE type) : m_type(type)
{
	m_pos = VEC3_NULL;
}

//================================================
// デストラクタ
//================================================
CCollision::~CCollision()
{
}

//================================================
// 当たり判定の生成処理
//================================================
CCollision* CCollision::Create(const D3DXVECTOR3 pos, const TYPE type)
{
	// 当たり判定のポインタ
	CCollision* pCollision = nullptr;

	//// 種類の遷移
	//switch (type)
	//{
	//case TYPE::TYPE_AABB:
	//	pCollision = new CCollisionAABB;
	//	break;
	//case TYPE::TYPE_SPHERE:
	//	pCollision = new CCollisionSphere;
	//	break;
	//case TYPE::TYPE_FOV:
	//	pCollision = new CCollisionFOV;
	//	break;
	//default:
	//	break;
	//}

	// 位置の設定
	pCollision->m_pos = pos;

	// 種類を代入
	pCollision->m_type = type;

	// 判定を返す
	return pCollision;
}

//================================================
// コンストラクタ
//================================================
CCollisionAABB::CCollisionAABB() : CCollision(TYPE::TYPE_AABB)
{
}

//================================================
// デストラクタ
//================================================
CCollisionAABB::~CCollisionAABB()
{
}

//================================================
// AABB対AABBの当たり判定
//================================================
bool CCollisionAABB::Collision(CCollider* pMyBox, CCollider* pTargetBox, D3DXVECTOR3* pushPos,int* pFace)
{
	// 自分の空間情報へのポインタの取得
	CTransform* pMyTransform = pMyBox->GetTransform();

	// 相手の空間情報へのポインタの取得
	CTransform* pOtherTransform = pTargetBox->GetTransform();

	// 自分の空間情報の取得
	CTransform::Info myInfo = {};
	CTransform::Info otherInfo = {};

	if (pMyTransform != nullptr)
	{
		// 情報の取得
		myInfo = pMyTransform->GetInfo();
	}

	if (pOtherTransform != nullptr)
	{
		// 情報の取得
		otherInfo = pOtherTransform->GetInfo();
	}

	// 自分の位置と大きさの取得
	D3DXVECTOR3 pos = myInfo.pos;

	// 相手の位置と大きさの取得
	D3DXVECTOR3 TargetPos = otherInfo.pos;

	// ターゲットの大きさ
	D3DXVECTOR3 TargetSize = otherInfo.Size;
	D3DXVECTOR3 TargetPosOld = otherInfo.posOld;

	// 自分の前回の位置
	D3DXVECTOR3 posOldMin = myInfo.posOld - (myInfo.Size * HALF_VALUE);
	D3DXVECTOR3 posOldMax = myInfo.posOld + (myInfo.Size * HALF_VALUE);

	// 自分の位置
	D3DXVECTOR3 posMin = pos - (myInfo.Size * HALF_VALUE);
	D3DXVECTOR3 posMax = pos + (myInfo.Size * HALF_VALUE);

	// ターゲットの前回の位置(tはターゲットのt)
	D3DXVECTOR3 tPosOldMin = TargetPosOld - (TargetSize * HALF_VALUE);
	D3DXVECTOR3 tPosOldMax = TargetPosOld + (TargetSize * HALF_VALUE);

	// ターゲットの位置(tはターゲットのt)
	D3DXVECTOR3 tPosMin = TargetPos - (TargetSize * HALF_VALUE);
	D3DXVECTOR3 tPosMax = TargetPos + (TargetSize * HALF_VALUE);

	// Yの範囲に入っているか
	const bool bCheckRangeY = posOldMin.y <= tPosOldMax.y && posOldMax.y >= tPosOldMin.y;

	// 範囲外だったら
	if (!bCheckRangeY)
	{
		return false;
	}

	// Zの範囲内に入っている
	if (tPosMin.z < posMax.z && tPosMax.z > posMin.z)
	{
		// 左から右にめり込んだ
		if (tPosOldMin.x >= posOldMax.x &&
			tPosMin.x < posMax.x)
		{
			// 当たった面を設定
			if (pFace != nullptr)
			{
				// 左にめり込んだ
				*pFace = FACE_LEFT;
			}

			// 押し出しを設定
			if (pushPos != nullptr)
			{
				// めり込んだ分戻す
				pushPos->x = tPosMin.x - myInfo.Size.x * HALF_VALUE - 0.1f;
			}
			return true;
		}
		// 右から左にめり込んだ
		else if (tPosOldMax.x <= posOldMin.x &&
			tPosMax.x > posMin.x)
		{
			// 当たった面を設定
			if (pFace != nullptr)
			{
				// 右にめり込んだ
				*pFace = FACE_RIGHT;
			}

			// 押し出しを設定
			if (pushPos != nullptr)
			{
				// めり込んだ分戻す
				pushPos->x = tPosMax.x + myInfo.Size.x * HALF_VALUE + 0.1f;
			}
			return true;
		}
	}

	// xの範囲に入っている
	if (tPosMin.x < posMax.x && tPosMax.x > posMin.x)
	{
		// 後方からめり込んだ(左手座標系)
		if (tPosOldMax.z <= posOldMin.z &&
			tPosMax.z > posMin.z)
		{
			// 当たった面を設定
			if (pFace != nullptr)
			{
				// 後方からめり込んだ
				*pFace = FACE_BACK;
			}

			// 押し出しを設定
			if (pushPos != nullptr)
			{
				// めり込んだ分戻す
				pushPos->z = tPosMax.z + myInfo.Size.z * HALF_VALUE + 0.1f;
			}
			return true;
		}
		else if (tPosOldMin.z >= posOldMax.z &&
			tPosMin.z < posMax.z)
		{
			// 当たった面を設定
			if (pFace != nullptr)
			{
				// 前方からめり込んだ
				*pFace = FACE_FRONT;
			}

			if (pushPos != nullptr)
			{
				// めり込んだ分戻す
				pushPos->z = tPosMin.z - myInfo.Size.z * HALF_VALUE - 0.1f;
			}
			return true;
		}
	}

	//if (pos.x - m_Size.x * HALF_VALUE <= otherPos.x + otherSize.x * HALF_VALUE
	//	&& pos.x + m_Size.x * HALF_VALUE >= otherPos.x - otherSize.x * HALF_VALUE)
	//{
	//	if (pos.z - m_Size.z * HALF_VALUE <= otherPos.z + otherSize.z * HALF_VALUE
	//		&& pos.z + m_Size.z * HALF_VALUE >= otherPos.z - otherSize.z * HALF_VALUE)
	//	{
	//		// 上から下
	//		if (m_posOld.y >= otherPos.y + otherSize.y
	//			&& pos.y < otherPos.y + otherSize.y)
	//		{
	//			bHit = true;

	//			//bLanding = true;
	//			//pos.y = otherPos.y + otherSize.y * g_Block[nCntBlock].Scal.y - Size.y;
	//			//pMove->y = 0.0f;
	//		}
	//		// 下から上
	//		else if (m_posOld.y + m_Size.y * HALF_VALUE <= otherPos.y - otherSize.y * HALF_VALUE
	//			&& pos.y + m_Size.y * HALF_VALUE > otherPos.y - otherSize.y * HALF_VALUE)
	//		{
	//			bHit = true;

	//			//pos.y = m_posOld.y;
	//			//pMove->y = 0.0f;
	//		}

	//	}
	//}

	return false;
}

//================================================
// コンストラクタ
//================================================
CCollisionSphere::CCollisionSphere() : CCollision(TYPE::TYPE_SPHERE)
{
}

//================================================
// デストラクタ
//================================================
CCollisionSphere::~CCollisionSphere()
{
}

//================================================
// 当たり判定(円vs円)
//================================================
bool CCollisionSphere::Collision(CCollider* myCollider, CCollider* otherCollider)
{
	// 自分の空間情報へのポインタの取得
	CTransform* pMyTransform = myCollider->GetTransform();

	// 相手の空間情報へのポインタの取得
	CTransform* pOtherTransform = otherCollider->GetTransform();

	// 自分の空間情報の取得
	CTransform::Info myInfo = {};
	CTransform::Info otherInfo = {};

	if (pMyTransform != nullptr)
	{
		// 情報の取得
		myInfo = pMyTransform->GetInfo();
	}

	if (pOtherTransform != nullptr)
	{
		// 情報の取得
		otherInfo = pOtherTransform->GetInfo();
	}

	// 位置の取得
	D3DXVECTOR3 pos = myInfo.pos;

	// 相手の位置
	D3DXVECTOR3 otherPos = otherInfo.pos;

	// 相手の半径
	float fOtherRadius = otherInfo.fRadius;

	// 自分の半径
	float fMyRadius = myInfo.fRadius;

	// 差分を求める
	D3DXVECTOR3 diff = otherPos - pos;

	// 距離を求める
	float fDistance = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	// 半径足す
	float fRadius = fMyRadius + fOtherRadius;

	// 半径を2乗する
	fRadius = fRadius * fRadius;

	// 距離が半径以下だったら当たっている
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//================================================
// コンストラクタ
//================================================
CCollisionFOV::CCollisionFOV() : CCollision(TYPE::TYPE_FOV)
{
}

//================================================
// デストラクタ
//================================================
CCollisionFOV::~CCollisionFOV()
{
}

//================================================
// 視界の判定
//================================================
bool CCollisionFOV::Collision(const D3DXVECTOR3 otherpos, CCollider* pFOV)
{
	// 自分の空間情報へのポインタの取得
	CTransform* pTransform = pFOV->GetTransform();

	// 自分の空間情報の取得
	CTransform::Info Info = {};

	if (pTransform != nullptr)
	{
		// 情報の取得
		Info = pTransform->GetInfo();
	}

	// 位置の取得
	D3DXVECTOR3 objectPos = Info.pos;

	// 前方までのベクトル
	D3DXVECTOR3 vecFront = GetVector(otherpos, objectPos);

	// データの取得
	const CColliderFOV::Data *pData = static_cast<const CColliderFOV::Data*>(pFOV->GetData());

	// データを取得できなかったら処理しない
	if (pData == nullptr) return false;

	D3DXVECTOR3 LeftPos; // 左の位置

	// 左側の視界の端の位置を求める
	LeftPos.x = objectPos.x + sinf(pData->fNowAngle + pData->fAngleLeft) * pData->fLength;
	LeftPos.y = 0.0f;
	LeftPos.z = objectPos.z + cosf(pData->fNowAngle + pData->fAngleLeft) * pData->fLength;

	// 左側の視界のベクトルの作成
	D3DXVECTOR3 VecLeft = GetVector(LeftPos, objectPos);

	D3DXVECTOR3 RightPos; // 右の位置

	// 右側の視界の端の位置を求める
	RightPos.x = objectPos.x + sinf(pData->fNowAngle + pData->fAngleRight) * pData->fLength;
	RightPos.y = 0.0f;
	RightPos.z = objectPos.z + cosf(pData->fNowAngle + pData->fAngleRight) * pData->fLength;

	// 右側の視界のベクトルの作成
	D3DXVECTOR3 VecRight = GetVector(RightPos, objectPos);

	D3DXVECTOR3 Cross0,Cross1; // 判定用外積ベクトル

	// 外積を出す
	D3DXVec3Cross(&Cross0, &VecLeft, &vecFront);
	D3DXVec3Cross(&Cross1, &vecFront, &VecRight);

	// 2本とも上向きだったら
	if (Cross0.y >= 0.0f && Cross1.y >= 0.0f)
	{
		return true;
	}

	return false;
}

//================================================
// カプセルコライダーの当たり判定
//================================================
bool CCollisionCapsule::Collision(CCollider* myCapsule, CCollider* otherCapsule,D3DXVECTOR3 *NearPos1, D3DXVECTOR3* NearPos2)
{
	// カプセルコライダーのデータ構造体を使用
	using Data = CColliderCapsule::Data;

	// 自分のデータの取得
	const Data* pMyData = static_cast<const Data*>(myCapsule->GetData());

	// 相手のデータの取得
	const Data* pOtherData = static_cast<const Data*>(otherCapsule->GetData());

	// データの取得が出来なかったら処理しない
	if (pMyData == nullptr || pOtherData == nullptr) return false;

	float param1, param2; // パラメーター1,2

	D3DXVECTOR3 closestPos1, closestPos2; // 最近接点1,2

	// 線分1と線分2の距離の取得
	float fDistance = ClosestPtSegmentSegment(
		pMyData->StartPos,
		pMyData->EndPos,
		pOtherData->StartPos,
		pOtherData->EndPos,
		&param1,
		&param2,
		&closestPos1,
		&closestPos2);

	// 距離を求める
	fDistance = sqrtf(fDistance);

	if (NearPos1 != nullptr)
	{
		// 最近接点1を渡す
		*NearPos1 = closestPos1;
	}
	if (NearPos2 != nullptr)
	{
		// 最近接点2を渡す
		*NearPos2 = closestPos2;
	}

	// 空間情報の取得
	CTransform* pMyTransform = myCapsule->GetTransform();
	CTransform* pOtherTransform = myCapsule->GetTransform();

	// データの取得が出来なかったら処理しない
	if (pMyTransform == nullptr || pOtherTransform == nullptr) return false;

	// 自分の空間情報の取得
	CTransform::Info myInfo = pMyTransform->GetInfo();
	CTransform::Info otherInfo = pMyTransform->GetInfo();

	// 二つの円の半径を渡す
	float fRadius = myInfo.fRadius + otherInfo.fRadius;

	// 距離が半径以下だったら当たっている
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//================================================
// 円との当たり判定
//================================================
bool CCollisionCapsule::CollisionSphere(CCollider* pCapsule, CCollider* pSphere)
{
	// カプセルの空間情報へのポインタの取得
	CTransform* pCapsuleTransform = pCapsule->GetTransform();

	// 円の空間情報へのポインタの取得
	CTransform* pSphereTransform = pSphere->GetTransform();

	// 自分の空間情報の取得
	CTransform::Info capsuleInfo = {};
	CTransform::Info sphereInfo = {};

	if (pCapsuleTransform != nullptr)
	{
		// 情報の取得
		capsuleInfo = pCapsuleTransform->GetInfo();
	}

	if (pSphereTransform != nullptr)
	{
		// 情報の取得
		sphereInfo = pSphereTransform->GetInfo();
	}

	// カプセルコライダーのデータ構造体を使用
	using CapsuleData = CColliderCapsule::Data;

	// 自分のデータの取得
	const CapsuleData *pCapsuleData = static_cast<const CapsuleData*>(pCapsule->GetData());

	// 取得できなかったら処理しない
	if (pCapsuleData == nullptr) return false;

	// 円の位置の取得
	D3DXVECTOR3 spherePos = sphereInfo.pos;

	// 円の半径の取得
	float fSphereRadius = sphereInfo.fRadius;

	float param1, param2; // パラメーター1,2

	D3DXVECTOR3 closestPos1, closestPos2; // 最近接点1,2

	// 線分と円の距離を求める
	float fDistance = ClosestPtSegmentSegment(
		pCapsuleData->StartPos,
		pCapsuleData->EndPos,
		spherePos,
		spherePos,
		&param1,
		&param2,
		&closestPos1,
		&closestPos2);

	// 距離を求める
	fDistance = sqrtf(fDistance);

	// 空間情報の取得
	CTransform* pCapTransform = pCapsule->GetTransform();

	// データの取得が出来なかったら処理しない
	if (pCapTransform == nullptr) return false;

	// 自分の空間情報の取得
	CTransform::Info myInfo = pCapTransform->GetInfo();

	// 二つの円の半径を渡す
	float fRadius = myInfo.fRadius + fSphereRadius;

	// 距離が半径以下だったら当たっている
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//================================================
// コンストラクタ
//================================================
CCollisionCapsule::CCollisionCapsule() : CCollision(TYPE::TYPE_CAPSULE)
{

}

//================================================
// 線分の線分の距離
//================================================
float CCollisionCapsule::ClosestPtSegmentSegment(D3DXVECTOR3 Start1, D3DXVECTOR3 End1, D3DXVECTOR3 Start2, D3DXVECTOR3 End2, float* pParam1, float* pParam2, D3DXVECTOR3* c1osestPos1, D3DXVECTOR3* c1osestPos2)
{
	D3DXVECTOR3 dir1 = Start1 - End1; // 線分S1のベクトル
	D3DXVECTOR3 dir2 = Start2 - End2; // 線分S2のベクトル
	D3DXVECTOR3 diffStart = Start1 - Start2;

	float fLengthSq1 = D3DXVec3Dot(&dir1, &dir1); // 線分S1の方向ベクトル
	float fLengthSq2 = D3DXVec3Dot(&dir2, &dir2); // 線分S2の方向ベクトル
	
	float dotVec2DiffStart = D3DXVec3Dot(&dir2, &diffStart);

	const float EPSILON = 1e-6f; // チェック用

	// 片方あるいは両方の線分が点に対して縮退しているかどうかチェック
	if (fLengthSq1 <= EPSILON && fLengthSq2 <= EPSILON)
	{
		// 両方の線分が点に縮退
		*pParam1 = *pParam2 = 0.0f;
		*c1osestPos1 = Start1;
		*c1osestPos2 = Start2;

		D3DXVECTOR3 diff = *c1osestPos1 - *c1osestPos2;
		
		return D3DXVec3Dot(&diff, &diff);
	}

	if (fLengthSq1 <= EPSILON)
	{
		// 最初の線分が点に縮退
		*pParam1 = 0.0f;
		*pParam2 = dotVec2DiffStart / fLengthSq2; // s = o >= t = (b*s + f) / e = f / e
		*pParam2 = Clamp(*pParam2, 0.0f, 1.0f);
	}
	else
	{
		float dotDir1Diff = D3DXVec3Dot(&dir1, &diffStart);

		if (fLengthSq2 <= EPSILON)
		{
			// 2番目の線分が点に縮退
			*pParam2 = 0.0f;
			*pParam1 = Clamp(-dotDir1Diff / fLengthSq1, 0.0f, 1.0f); // t = 0 >= s = (b * t - c) / a = -c / a
		}
		else
		{
			float dotDir1Dir2 = D3DXVec3Dot(&dir1, &dir2);
			float denom = fLengthSq1 * fLengthSq2 - dotDir1Dir2 * dotDir1Dir2; // 常に非負

			// 線分が平行ではない場合、L1上のL2に対する最近接点を計算、そして線分1に対してクランプ。そうでない場合は任意のsを選択
			if (denom != 0.0f)
			{
				*pParam1 = Clamp((dotDir1Dir2 * dotVec2DiffStart - dotDir1Diff * fLengthSq2) / denom, 0.0f, 1.0f);
			}
			else
			{
				*pParam1 = 0.0f;
			}
			// L2上のS1(s)に対する最近接点を以下を用いて計算
			// t = dot((P1 + D1 * s) - P2,P2) / dot(D2,D2) = (b * s + f) / e
			*pParam2 = (dotDir1Dir2 * *pParam1 + dotVec2DiffStart) / fLengthSq2;
			// tが[0,1]の中にあれば終了。そうなればｔをクランプ、ｓをｔの新しい値に対して以下を用いて計算
			// s = Dot((P2 + D2 * t) - P1,D1) / Dot(D1,D1) = (t * b - c) / a
			// そしてsを[0,1]に対してクランプ
			if (*pParam2 < 0.0f)
			{
				*pParam2 = 0.0f;
				*pParam1 = Clamp(-dotDir1Diff / fLengthSq1, 0.0f, 1.0f);
			}
			else if (*pParam2 > 1.0f)
			{
				*pParam2 = 1.0f;
				*pParam1 = Clamp((dotDir1Dir2 - dotDir1Diff) / fLengthSq1, 0.0f, 1.0f);
			}
		}
	}

	*c1osestPos1 = Start1 + dir1 * *pParam1;
	*c1osestPos2 = Start2 + dir2 * *pParam2;

	D3DXVECTOR3 diff = *c1osestPos1 - *c1osestPos2;

	// 距離の2乗
	float fDistance = D3DXVec3Dot(&diff, &diff);

	return fDistance;
}

//================================================
// デストラクタ
//================================================
CCollisionCapsule::~CCollisionCapsule()
{
}
