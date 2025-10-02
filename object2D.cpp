//===================================================
//
// �I�u�W�F�N�g2D�̕`�揈�� [object2D.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object2D.h"
#include"renderer.h"
#include "manager.h"
#include"textureManager.h"

using namespace Const;	// ���O���Const���g�p����

//===================================================
// �R���X�g���N�^
//===================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_Size = VEC2_NULL;

	m_pVtxBuffer = NULL;
	m_fAngle = 0.0f;
	m_Length = 0.0f;

	m_nTextureIdx = -1;
}
//===================================================
// �f�X�g���N�^
//===================================================
CObject2D::~CObject2D()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//===================================================
// ����������
//===================================================
HRESULT CObject2D::Init(void)
{
	m_fAngle = 0.0f;
	m_Length = 0.0f;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//===================================================
// �I������
//===================================================
void CObject2D::Uninit(void)
{
	// �������g�̔j��
	Release();
}
//===================================================
// �X�V����
//===================================================
void CObject2D::Update(void)
{	
	CObject2D::UpdateVertex();
}
//===================================================
// �`�揈��
//===================================================
void CObject2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`��������Ȃ�
	if (m_nTextureIdx != -1)
	{
		// �e�N�X�`����ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));
	}
	else
	{
		// �e�N�X�`�����Ȃ�
		pDevice->SetTexture(0, NULL);
	}
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // �v���~�e�B�u�̎��	
}

//===================================================
// ���_�̃I�t�Z�b�g�̐ݒ菈��
//===================================================
void CObject2D::SetVtx(const D3DXCOLOR col)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}
//===================================================
// �I�u�W�F�N�g2D�̐�������
//===================================================
CObject2D* CObject2D::Create(const float fWidth, const float fHeight, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject2D* pObject2D = nullptr;

	// 2D�I�u�W�F�N�g�̐���
	pObject2D = new CObject2D;

	if (pObject2D == nullptr) return nullptr;

	pObject2D->Init();
	pObject2D->m_rot = rot;
	pObject2D->m_pos = pos;
	pObject2D->m_Size = { fWidth,fHeight };
	pObject2D->SetSize(fWidth, fHeight);
	pObject2D->SetVtx(WHITE);
	pObject2D->SetTextureID();

	return pObject2D;
}
//===================================================
// �I�u�W�F�N�g�̑傫���̐ݒ菈��
//===================================================
void CObject2D::SetSize(const float fWidth, const float fHeight)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	// �傫���̐ݒ菈��
	m_Size = { fWidth, fHeight };

	m_Length = sqrtf((fWidth * fWidth) + (fHeight * fHeight));
	m_fAngle = atan2f(fWidth, fHeight);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �傫���̐ݒ菈��
//===================================================
void CObject2D::SetSize(const float leftWidth, const float rightWdth, const float topHeight, const float buttom)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu
	D3DXVECTOR3 pos = m_pos;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - leftWidth, pos.y - topHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + rightWdth, pos.y - topHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - leftWidth, pos.y + buttom, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + rightWdth, pos.y + buttom, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �ʒu�̍X�V
//===================================================
void CObject2D::UpdateVertex(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	m_Length = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �F�̐ݒ菈��
//===================================================
void CObject2D::SetColor(const D3DXCOLOR col)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �e�N�X�`���̐ݒ菈��
//===================================================
void CObject2D::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`��������������
	if (pTextureName == NULL)
	{
		m_nTextureIdx = -1;
		return;
	}

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	m_nTextureIdx = pTexture->Register(pTextureName);
}
