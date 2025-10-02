//===================================================
//
// 2D�I�u�W�F�N�g(�}���`�e�N�X�`��)�̕`�� [object2DMT.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object2DMT.h"
#include "manager.h"
#include"renderer.h"
#include"textureManager.h"

using namespace Const; // ���O���Const���g�p

//===================================================
// �R���X�g���N�^
//===================================================
CObject2DMT::CObject2DMT() : CObject(7)
{
	// ���ׂĂ�-1������
	memset(&m_nTextureIdx, -1, sizeof(m_nTextureIdx));
	m_rot = VEC3_NULL;
	m_pos = VEC3_NULL;
	m_pVtxBuffer = nullptr;
	m_Size = VEC2_NULL;
	m_col = WHITE;
}

//===================================================
// �f�X�g���N�^
//===================================================
CObject2DMT::~CObject2DMT()
{
}

//===================================================
// ��������
//===================================================
CObject2DMT* CObject2DMT::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size)
{
	// 2D�I�u�W�F�N�g�̐���
	CObject2DMT* pObject = new CObject2DMT;

	pObject->SetPosition(pos);
	pObject->SetRotaition(VEC3_NULL);
	pObject->SetSize(Size);

	if (FAILED(pObject->Init()))
	{// �������Ɏ��s������
		// �I������
		pObject->Uninit();
		pObject = nullptr;

		return nullptr;
	}

	return pObject;
}

//===================================================
// ����������
//===================================================
HRESULT CObject2DMT::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_MULT) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2DMT,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_���̃|�C���^
	VERTEX_2D_MULT* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	// �Ίp���̒��������߂�
	float fLength = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

	// �X�������߂�
	float fAngle = atan2f(m_Size.x, m_Size.y);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].texMT = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texMT = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texMT = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].texMT = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CObject2DMT::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// �������g�̔j��
	CObject::Release();
}

//===================================================
// �X�V����
//===================================================
void CObject2DMT::Update(void)
{
	// ���_���̃|�C���^
	VERTEX_2D_MULT* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	// �Ίp���̒��������߂�
	float fLength = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

	// �X�������߂�
	float fAngle = atan2f(m_Size.x, m_Size.y);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//===================================================
// �`�揈��
//===================================================
void CObject2DMT::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D_MULT));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2DMT);

	// �e�N�X�`���̐�����
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// �e�N�X�`������������
		if (m_nTextureIdx[nCnt] != -1)
		{
			// �e�N�X�`����ݒ�
			pDevice->SetTexture(nCnt, pTexture->GetAdress(m_nTextureIdx[nCnt]));
		}
		else
		{
			// �e�N�X�`�����Ȃ�
			pDevice->SetTexture(nCnt, NULL);
		}
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // �v���~�e�B�u�̎��	

	// ���Ƃɖ߂�
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//===================================================
// �e�N�X�`����ID�̓o�^
//===================================================
void CObject2DMT::SetTextureID(const char* pTexture1, const char* pTexture2)
{
	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// ID�̓o�^
	m_nTextureIdx[0] = pTexture->Register(pTexture1);

	// ID�̓o�^
	m_nTextureIdx[1] = pTexture->Register(pTexture2);
}

//===================================================
// �e�N�X�`�����W�̐ݒ�
//===================================================
void CObject2DMT::SetTexture(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex1, const D3DXVECTOR2 tex2, const D3DXVECTOR2 tex3, const int nCnt)
{
	// ���_���̃|�C���^
	VERTEX_2D_MULT* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = tex0;
		pVtx[1].tex = tex1;
		pVtx[2].tex = tex2;
		pVtx[3].tex = tex3;
	}
	else
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].texMT = tex0;
		pVtx[1].texMT = tex1;
		pVtx[2].texMT = tex2;
		pVtx[3].texMT = tex3;
	}

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}
