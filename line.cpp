//==============================================
//
// ���̕`�������N���X [line.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "line.h"
#include "manager.h"
#include "renderer.h"

//**********************************************
// �ÓI�����o�ϐ�
//**********************************************
CLineRenderer* CLineRenderer::m_pInstance = nullptr; // �����̃C���X�^���X

//==============================================
// �R���X�g���N�^
//==============================================
CLineRenderer::CLineRenderer() :
	CObject(7),
	m_pVtxBuffer(nullptr)
{

}

//==============================================
// �f�X�g���N�^
//==============================================
CLineRenderer::~CLineRenderer()
{
	m_pInstance = nullptr;
}

//==============================================
// �C���X�^���X�̎擾
//==============================================
CLineRenderer* CLineRenderer::GetInstance(void)
{
	if (m_pInstance == nullptr)
	{
		// ����
		m_pInstance = new CLineRenderer;
		m_pInstance->Init();
	}

	return m_pInstance;
}

//==============================================
// ����������
//==============================================
HRESULT CLineRenderer::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// ���炩���߃��������m��
	m_aLine.reserve(NUM_VERTEX);

	return S_OK;
}

//==============================================
// �I������
//==============================================
void CLineRenderer::Uninit(void)
{
	m_aLine.clear();

	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// �������g�̔j��
	CObject::Release();
}

//==============================================
// �`�揈��
//==============================================
void CLineRenderer::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ���C�e�B���OOFF
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// Z�o�b�t�@��OFF

	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);
	pDevice->SetTransform(D3DTS_WORLD, &Identity);

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	int nCnt = 0;

	for (auto& line : m_aLine)
	{
		pVtx[nCnt].pos = line.pos;
		pVtx[nCnt].col = line.col;
		nCnt++;
	}
	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	// �e�N�X�`����ݒ�
	pDevice->SetTexture(0, NULL);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �v�f����
	for (nCnt = 0; nCnt < static_cast<int>(m_aLine.size()); nCnt++)
	{
		pDevice->SetTexture(0, nullptr);
		pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 2 * nCnt);
	}

	// �v�f�̃N���A
	m_aLine.clear();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ���C�e�B���OON
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// Z�o�b�t�@��ON
}

//==============================================
// ���̒ǉ�
//==============================================
void CLineRenderer::AddLine(const D3DXVECTOR3 start, const D3DXVECTOR3 end, const D3DXCOLOR col)
{
	Vertex vertex;

	vertex.pos = start;
	vertex.col = col;

	// �v�f�̒ǉ�
	m_aLine.push_back(vertex);

	vertex.pos = end;
	vertex.col = col;

	// �v�f�̒ǉ�
	m_aLine.push_back(vertex);

}
