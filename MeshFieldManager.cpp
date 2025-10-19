//===================================================
//
// ���b�V���t�B�[���h�̃}�l�[�W���[�N���X�̒�` [MeshFieldManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
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
// �ÓI�����o�ϐ��錾
//***************************************************
std::unique_ptr<CMeshFieldManager> CMeshFieldManager::m_pInstance = nullptr; // �����̃C���X�^���X

//===================================================
// �f�X�g���N�^
//===================================================
CMeshFieldManager::~CMeshFieldManager()
{
    m_pBrushSphere.reset();
}

//===================================================
// �C���X�^���X�̎擾
//===================================================
CMeshFieldManager* CMeshFieldManager::GetInstance(void)
{
    // ��������Ă��Ȃ�������
    if (m_pInstance == nullptr)
    {
        m_pInstance.reset(new CMeshFieldManager);
        m_pInstance->Init();
    }

    return m_pInstance.get();
}

//===================================================
// ����������
//===================================================
HRESULT CMeshFieldManager::Init(void)
{
    m_fBrushRadius = BRUSH_RADIUS;

    // �V�����_�[�̐���
    m_pMeshCylinder = CMeshCylinder::Create(Const::VEC3_NULL, 8, 1, m_fBrushRadius, 1000.0f, Const::VEC3_NULL);

    // ��ԏ��̎擾
    m_pTransform.reset(CTransform::Create());

    // ����
    m_pBrushSphere = CColliderSphere::Create(Const::VEC3_NULL, m_fBrushRadius,m_pTransform.get());

    return S_OK;
}

//===================================================
// �X�V����
//===================================================
void CMeshFieldManager::Update(void)
{
    // �t�B�[���h�̎擾
    CMeshField* pMeshField = CManager::GetMeshField();

    // �擾�ł��Ȃ������珈�����Ȃ�
    if (pMeshField == nullptr) return;

    if (m_pMeshCylinder != nullptr)
    {
        m_pMeshCylinder->Show(true);
    }

    // �������̎擾
    int nSegH = pMeshField->GetSegH();
    int nSegV = pMeshField->GetSegV();

    // �傫���̎擾
    D3DXVECTOR2 Size = pMeshField->GetSize();

    if (ImGui::DragInt(u8"������", &nSegH, 0.5f, 1, CMeshField::MAX_SEGMENT_H - 1))
    {
        // ���_�̐ݒ�
        pMeshField->SetSegment(nSegH, nSegV);
    }
    if (ImGui::DragInt(u8"�����c", &nSegV, 0.5f, 1, CMeshField::MAX_SEGMENT_V - 1))
    {
        // ���_�̐ݒ�
        pMeshField->SetSegment(nSegH, nSegV);
    }
    if (ImGui::DragFloat2(u8"�傫���̐ݒ�", Size, 0.5f, 1.0f, 9999999.0f))
    {
        pMeshField->SetSize(Size);
    }

    if (ImGui::Button(u8"�Z�[�u", ImVec2(300, 0)))
    {
        pMeshField->Save();
    }

    if (ImGui::Button(u8"���[�h", ImVec2(300, 0)))
    {
        pMeshField->Load();
    }

    ImGui::Checkbox(u8"�u���V���I��", &m_bShow);    

    if (!m_bShow)
    {
        if (m_pMeshCylinder != nullptr)
        {
            m_pMeshCylinder->Show(false);
        }

        return;
    }

    // �u���V�̐ݒ�
    SetBrush();

    // ���_�̐ݒ�
    SetVtx(pMeshField);
}

//===================================================
// �����邩�ǂ���
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
// �R���X�g���N�^
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
// �u���V�̐ݒ�
//===================================================
void CMeshFieldManager::SetBrush(void)
{
    // null�������珈�����Ȃ�
    if (m_pMeshCylinder == nullptr) return;

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &mousePos); // DirectX���W�n�ɕϊ�

    // ImGui�̃f�o�C�X�ƃr���[�|�[�g�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DVIEWPORT9 vp;
    pDevice->GetViewport(&vp);

    // �v�Z�p�s��
    D3DXMATRIX view, proj;

    // �r���[�A�v���W�F�N�V�����}�g���b�N�X�̎擾
    pDevice->GetTransform(D3DTS_VIEW, &view);
    pDevice->GetTransform(D3DTS_PROJECTION, &proj);

    // �ϊ����W�̎擾
    D3DXVECTOR3 pos = math::ScreenToWorld(
        D3DXVECTOR2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)),
        0.0f,
        vp,
        view,
        proj);

    // �ʒu�̐ݒ�
    m_pMeshCylinder->SetPosition(pos);
    m_BrushPos = pos; 
}

//===================================================
// �����̐ݒ菈��
//===================================================
void CMeshFieldManager::SetVtx(CMeshField* pMeshField)
{
    // null�������珈�����Ȃ�
    if (m_pBrushSphere == nullptr) return;

    // ���̎擾
    CTransform::Info info = m_pTransform->GetInfo();

    // �R���C�_�[�̈ʒu�̍X�V
    info.pos = m_BrushPos;
    info.fRadius = m_fBrushRadius;

    m_pTransform->SetInfo(info);

    // �L�[�{�[�h�̎擾
    CInputMouse* pMouse = CManager::GetInputMouse();

    // �L�[�{�[�h�̎擾
    CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

    // ImGui�̎擾
    CImGuiManager* pImGuiManager = CManager::GetRenderer()->GetImGui();

    if (ImGui::BeginTabBar("test000"))
    {
        if (ImGui::BeginTabItem(u8"�����ݒ�"))
        {
            // window��G���Ă��Ȃ��Ȃ�
            if (pImGuiManager != nullptr && !pImGuiManager->GetActiveWindow())
            {
                if (pMouse->OnMousePress(pMouse->LEFT) && !pKeyboard->GetPress(DIK_LALT))
                {
                    // �����蔻��
                    pMeshField->SetVtxHeight(m_pBrushSphere.get(), m_fAddHeightValue);
                }
                else if (pMouse->OnMousePress(pMouse->RIGHT) && !pKeyboard->GetPress(DIK_LALT))
                {
                    // �����蔻��
                    pMeshField->SetVtxHeight(m_pBrushSphere.get(), -m_fAddHeightValue);
                }
            }

            ImGui::DragFloat(u8"�u���V���x", &m_fAddHeightValue, 0.5f, 1.0f, 50.0f);

            // ���a�̐ݒ�
            if (ImGui::DragFloat(u8"�u���V�̔��a", &m_fBrushRadius, 0.5f, 5.0f, 500.0f))
            {
                m_pMeshCylinder->SetRadius(m_fBrushRadius);
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(u8"�F�ݒ�"))
        {
            // window��G���Ă��Ȃ��Ȃ�
            if (pImGuiManager != nullptr && !pImGuiManager->GetActiveWindow())
            {
                if (pMouse->OnMousePress(pMouse->LEFT) && !pKeyboard->GetPress(DIK_LALT))
                {
                    // �����蔻��
                    pMeshField->SetVtxColor(m_pBrushSphere.get(), m_col);
                }
            }

            // �F�ύX
            ImGui::ColorEdit4(u8"�J���[�ύX", m_col);

            ImGui::EndTabItem();
        }

        // �^�u�o�[�̏I������
        ImGui::EndTabBar();
    }
}
