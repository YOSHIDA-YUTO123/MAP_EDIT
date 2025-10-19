//===================================================
//
// メッシュフィールドのマネージャークラスの定義 [MeshFieldManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "MeshFieldManager.h"
#include "meshfield.h"
#include "manager.h"
#include "imgui.h"
#include "cylinder.h"
#include "math.h"
#include "renderer.h"
#include "Collider.h"
#include "input.h"
#include "imguimaneger.h"
#include "transform.h"

//***************************************************
// 静的メンバ変数宣言
//***************************************************
std::unique_ptr<CMeshFieldManager> CMeshFieldManager::m_pInstance = nullptr; // 自分のインスタンス

//===================================================
// デストラクタ
//===================================================
CMeshFieldManager::~CMeshFieldManager()
{
    m_pBrushSphere.reset();
}

//===================================================
// インスタンスの取得
//===================================================
CMeshFieldManager* CMeshFieldManager::GetInstance(void)
{
    // 生成されていなかったら
    if (m_pInstance == nullptr)
    {
        m_pInstance.reset(new CMeshFieldManager);
        m_pInstance->Init();
    }

    return m_pInstance.get();
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMeshFieldManager::Init(void)
{
    m_fBrushRadius = BRUSH_RADIUS;

    // シリンダーの生成
    m_pMeshCylinder = CMeshCylinder::Create(Const::VEC3_NULL, 8, 1, m_fBrushRadius, 1000.0f, Const::VEC3_NULL);

    // 空間情報の取得
    m_pTransform.reset(CTransform::Create());

    // 生成
    m_pBrushSphere = CColliderSphere::Create(Const::VEC3_NULL, m_fBrushRadius,m_pTransform.get());

    return S_OK;
}

//===================================================
// 更新処理
//===================================================
void CMeshFieldManager::Update(void)
{
    // フィールドの取得
    CMeshField* pMeshField = CManager::GetMeshField();

    // 取得できなかったら処理しない
    if (pMeshField == nullptr) return;

    if (m_pMeshCylinder != nullptr)
    {
        m_pMeshCylinder->Show(true);
    }

    // 分割数の取得
    int nSegH = pMeshField->GetSegH();
    int nSegV = pMeshField->GetSegV();

    // 大きさの取得
    D3DXVECTOR2 Size = pMeshField->GetSize();

    if (ImGui::DragInt(u8"分割横", &nSegH, 0.5f, 1, CMeshField::MAX_SEGMENT_H - 1))
    {
        // 頂点の設定
        pMeshField->SetSegment(nSegH, nSegV);
    }
    if (ImGui::DragInt(u8"分割縦", &nSegV, 0.5f, 1, CMeshField::MAX_SEGMENT_V - 1))
    {
        // 頂点の設定
        pMeshField->SetSegment(nSegH, nSegV);
    }
    if (ImGui::DragFloat2(u8"大きさの設定", Size, 0.5f, 1.0f, 9999999.0f))
    {
        pMeshField->SetSize(Size);
    }

    if (ImGui::Button(u8"セーブ", ImVec2(300, 0)))
    {
        pMeshField->Save();
    }

    if (ImGui::Button(u8"ロード", ImVec2(300, 0)))
    {
        pMeshField->Load();
    }

    ImGui::Checkbox(u8"ブラシをオン", &m_bShow);    

    if (!m_bShow)
    {
        if (m_pMeshCylinder != nullptr)
        {
            m_pMeshCylinder->Show(false);
        }

        return;
    }

    // ブラシの設定
    SetBrush();

    // 頂点の設定
    SetVtx(pMeshField);
}

//===================================================
// 見えるかどうか
//===================================================
void CMeshFieldManager::SetShow(const bool bEnable)
{
    if (m_pMeshCylinder != nullptr)
    {
        m_pMeshCylinder->Show(bEnable);
        m_bShow = bEnable;
    }
}

//===================================================
// コンストラクタ
//===================================================
CMeshFieldManager::CMeshFieldManager()
{
    m_col = Const::WHITE;
    m_fBrushRadius = NULL;
    m_fAddHeightValue = ADD_HEIGHT;
    m_BrushPos = Const::VEC3_NULL;
    m_bShow = false;
    m_pMeshCylinder = nullptr;
}

//===================================================
// ブラシの設定
//===================================================
void CMeshFieldManager::SetBrush(void)
{
    // nullだったら処理しない
    if (m_pMeshCylinder == nullptr) return;

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &mousePos); // DirectX座標系に変換

    // ImGuiのデバイスとビューポートの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DVIEWPORT9 vp;
    pDevice->GetViewport(&vp);

    // 計算用行列
    D3DXMATRIX view, proj;

    // ビュー、プロジェクションマトリックスの取得
    pDevice->GetTransform(D3DTS_VIEW, &view);
    pDevice->GetTransform(D3DTS_PROJECTION, &proj);

    // 変換座標の取得
    D3DXVECTOR3 pos = math::ScreenToWorld(
        D3DXVECTOR2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)),
        0.0f,
        vp,
        view,
        proj);

    // 位置の設定
    m_pMeshCylinder->SetPosition(pos);
    m_BrushPos = pos; 
}

//===================================================
// 高さの設定処理
//===================================================
void CMeshFieldManager::SetVtx(CMeshField* pMeshField)
{
    // nullだったら処理しない
    if (m_pBrushSphere == nullptr) return;

    // 情報の取得
    CTransform::Info info = m_pTransform->GetInfo();

    // コライダーの位置の更新
    info.pos = m_BrushPos;
    info.fRadius = m_fBrushRadius;

    m_pTransform->SetInfo(info);

    // キーボードの取得
    CInputMouse* pMouse = CManager::GetInputMouse();

    // キーボードの取得
    CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

    // ImGuiの取得
    CImGuiManager* pImGuiManager = CManager::GetRenderer()->GetImGui();

    if (ImGui::BeginTabBar("test000"))
    {
        if (ImGui::BeginTabItem(u8"高さ設定"))
        {
            // windowを触っていないなら
            if (pImGuiManager != nullptr && !pImGuiManager->GetActiveWindow())
            {
                if (pMouse->OnMousePress(pMouse->LEFT) && !pKeyboard->GetPress(DIK_LALT))
                {
                    // 当たり判定
                    pMeshField->SetVtxHeight(m_pBrushSphere.get(), m_fAddHeightValue);
                }
                else if (pMouse->OnMousePress(pMouse->RIGHT) && !pKeyboard->GetPress(DIK_LALT))
                {
                    // 当たり判定
                    pMeshField->SetVtxHeight(m_pBrushSphere.get(), -m_fAddHeightValue);
                }
            }

            ImGui::DragFloat(u8"ブラシ強度", &m_fAddHeightValue, 0.5f, 1.0f, 50.0f);

            // 半径の設定
            if (ImGui::DragFloat(u8"ブラシの半径", &m_fBrushRadius, 0.5f, 5.0f, 500.0f))
            {
                m_pMeshCylinder->SetRadius(m_fBrushRadius);
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(u8"色設定"))
        {
            // windowを触っていないなら
            if (pImGuiManager != nullptr && !pImGuiManager->GetActiveWindow())
            {
                if (pMouse->OnMousePress(pMouse->LEFT) && !pKeyboard->GetPress(DIK_LALT))
                {
                    // 当たり判定
                    pMeshField->SetVtxColor(m_pBrushSphere.get(), m_col);
                }
            }

            // 色変更
            ImGui::ColorEdit4(u8"カラー変更", m_col);

            ImGui::EndTabItem();
        }

        // タブバーの終了処理
        ImGui::EndTabBar();
    }
}
