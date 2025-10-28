//==============================================
//
// プレイヤーの処理をするクラス [player.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "player.h"
#include "input.h"
#include "manager.h"
#include "character3D.h"
#include "transform.h"
#include "CollisionObject3D.h"
#include "camera.h"
#include "math.h"
#include "motion.h"
#include "Collider.h"
#include "meshfield.h"

//==============================================
// コンストラクタ
//==============================================
CPlayer::CPlayer() :
    CObject(4),
    m_pCharacter(nullptr),
    m_move(),
    m_pTransformRef(nullptr),
	m_fMove(NULL),
	m_fRotDest(NULL)
{
}

//==============================================
// デストラクタ
//==============================================
CPlayer::~CPlayer()
{
}

//==============================================
// 生成処理
//==============================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pMotionTxt, const int nNumMotion)
{
    CPlayer* pInstance = new CPlayer;

    // 初期化処理
    if (FAILED(pInstance->Init()))
    {
        pInstance->Uninit();
        pInstance = nullptr;
        return nullptr;;
    }

    // モーションのロード処理
    pInstance->m_pCharacter->LoadMotion(pMotionTxt, nNumMotion);

    // 当たり判定の取得
    CCollisionObject3D *pCollObj = pInstance->m_pCharacter->GetCollisionObject();

    // 空間情報の取得
    CTransform *pTransform = pCollObj->GetTransform();

    if (pTransform != nullptr)
    {
        // 情報の取得
        CTransform::Info info = pTransform->GetInfo();

        info.pos = pos;
        info.rot = rot;

        pTransform->SetInfo(info);
    }

    return pInstance;
}

//==============================================
// 初期化処理
//==============================================
HRESULT CPlayer::Init(void)
{
    // キャラクターの生成
    m_pCharacter = CCharacter3D::Create();

    if (m_pCharacter != nullptr)
    {
        if (FAILED(m_pCharacter->Init()))
        {
            return E_FAIL;
        }
        m_pCharacter->SetType(CCharacter3D::TYPE_PLAYER);
    }

	// 当たり判定オブジェクトの取得
	CCollisionObject3D* pCollisionObject = m_pCharacter->GetCollisionObject();

	// 空間情報の取得
	CTransform* pTransform = pCollisionObject->GetTransform();

	// 空間情報の設定
	m_pTransformRef = pTransform;

	m_pCharacter->SetCharacter(0, 1.0f, D3DXVECTOR3(20.0f, 100.0f, 20.0f));

	// コライダーの追加
	pCollisionObject->AddCollider(CColliderAABB::Create(), "player");
	
	m_fMove = 1.0f;

    return S_OK;
}

//==============================================
// 終了処理
//==============================================
void CPlayer::Uninit(void)
{
    m_pCharacter = nullptr;

    // 終了処理
    CObject::Release();
}

//==============================================
// 更新処理
//==============================================
void CPlayer::Update(void)
{
    // キャラクターが生成されていないなら
    if (m_pCharacter == nullptr) return;
    
    // 情報の取得
    CTransform::Info transform = m_pTransformRef->GetInfo();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの向きの取得
	float fCameraRotY = pCamera->GetRotaition().y;

	bool bMove = false;

	if (pKeyboard->GetPress(DIK_A))
	{
		//プレイヤーの移動(上)
		if (pKeyboard->GetPress(DIK_W) == true)
		{
			m_move.x += sinf(fCameraRotY - D3DX_PI * 0.25f) * m_fMove;
			m_move.z += cosf(fCameraRotY - D3DX_PI * 0.25f) * m_fMove;

			m_fRotDest = fCameraRotY + D3DX_PI * 0.75f;

			bMove = true;
		}
		//プレイヤーの移動(下)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(fCameraRotY - D3DX_PI * 0.75f) * m_fMove;
			m_move.z += cosf(fCameraRotY - D3DX_PI * 0.75f) * m_fMove;

			m_fRotDest = fCameraRotY + D3DX_PI * 0.25f;

			bMove = true;
		}
		// プレイヤーの移動(左)
		else
		{
			m_move.z += sinf(fCameraRotY) * m_fMove;
			m_move.x -= cosf(fCameraRotY) * m_fMove;

			m_fRotDest = fCameraRotY + D3DX_PI * 0.5f;

			bMove = true;
		}
	}
	//プレイヤーの移動(右)
	else if (pKeyboard->GetPress(DIK_D))
	{
		//プレイヤーの移動(上)
		if (pKeyboard->GetPress(DIK_W))
		{
			m_move.x += sinf(fCameraRotY + D3DX_PI * 0.25f) * m_fMove;
			m_move.z += cosf(fCameraRotY + D3DX_PI * 0.25f) * m_fMove;

			m_fRotDest = fCameraRotY - D3DX_PI * 0.75f;

			bMove = true;
		}
		//プレイヤーの移動(下)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(fCameraRotY + D3DX_PI * 0.75f) * m_fMove;
			m_move.z += cosf(fCameraRotY + D3DX_PI * 0.75f) * m_fMove;

			m_fRotDest = fCameraRotY - D3DX_PI * 0.25f;

			bMove = true;
		}
		// プレイヤーの移動(右)
		else
		{
			m_move.z -= sinf(fCameraRotY) * m_fMove;
			m_move.x += cosf(fCameraRotY) * m_fMove;

			m_fRotDest = fCameraRotY - D3DX_PI * 0.5f;

			bMove = true;
		}
	}
	//プレイヤーの移動(上)
	else if (pKeyboard->GetPress(DIK_W) == true)
	{
		m_move.x += sinf(fCameraRotY) * m_fMove;
		m_move.z += cosf(fCameraRotY) * m_fMove;

		m_fRotDest = fCameraRotY + D3DX_PI;

		bMove = true;
	}
	//プレイヤーの移動(下)
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		m_move.x -= sinf(fCameraRotY) * m_fMove;
		m_move.z -= cosf(fCameraRotY) * m_fMove;

		m_fRotDest = fCameraRotY;

		bMove = true;
	}

	// モーションの取得
	CMotion* pMotion = m_pCharacter->GetMotion();

	if (bMove)
	{
		pMotion->SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if(pMotion->GetType() == MOTIONTYPE_MOVE)
	{
		pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 10);
	}

    // 移動量の減衰
    m_move.x += (0.0f - m_move.x) * 0.25f;
    m_move.z += (0.0f - m_move.z) * 0.25f;

    // 移動量の更新
    transform.pos += m_move;

	// メッシュフィールドの生成
	CMeshField* pMeshField = CManager::GetMeshField();

	float fHeight = 0.0f;

	if (pMeshField->Collision(transform.pos, &fHeight))
	{
		transform.pos.y = fHeight;
	}

	m_move.y += -Const::MAX_GRABITY;

	// 角度の正規化
	NormalizeDiffRot(m_fRotDest - transform.rot.y, &transform.rot.y);

	// 目的の向きに補間
	transform.rot.y += (m_fRotDest - transform.rot.y) * 0.1f;

    // モーションの更新処理
    m_pCharacter->UpdateMotion();

    m_pTransformRef->SetInfo(transform);
}

//==============================================
// 描画処理
//==============================================
void CPlayer::Draw(void)
{
    // キャラクターが生成されていないなら
    if (m_pCharacter == nullptr) return;

    // 描画処理
    m_pCharacter->Draw();
}