//================================================
//
// ���b�V���t�B�[���h [meshfield.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "meshfield.h"
#include "manager.h"
#include "math.h"
#include "debugproc.h"
#include "LoadManager.h"
#include "impact.h"
#include "Collider.h"
#include "sound.h"
#include "math.h"
#include "renderer.h"
#include "textureManager.h" 
#include "Collision.h"
#include <string>

using namespace Const;		// ���O���Const���g�p����
using namespace std;		// ���O���std���g�p����
using namespace math;		// ���O���math���g�p����

// �����̖��O��Ԃ��g�p
namespace
{
	constexpr int NUM_POLYGON = 2;			// ���肷��|���S���̐�
	constexpr int NUM_SIRCLE = 5;			// ���b�V���T�[�N�����o����
	constexpr float MAX_COLOR = 1.0f;		// �F�̍ő�l
	constexpr float COLOR_EASE = 0.004f;	// �F�̃C�[�W���O�l
	constexpr float AUDIO_DISTANCE = 2000.0f; // ���̕������鋗��
}

//================================================
// �R���X�g���N�^
//================================================
CMeshField::CMeshField(int nPriority) : CObject(nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pIdxBuffer = nullptr;
	m_pVtxBuffer = nullptr;
	m_nNumIdx = NULL;
	m_nNumPolygon = NULL;
	m_nNumVtx = NULL;
	m_nTextureIdx = -1;
	m_nSegV = 1;
	m_nSegH = 1;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_Size = VEC2_NULL;
	m_Nor = VEC3_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CMeshField::~CMeshField()
{
}

//================================================
// ��������
//================================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 pos, const int nSegH, const int nSegV, const D3DXVECTOR2 Size, const D3DXVECTOR3 rot)
{
	// ���b�V���t�B�[���h�𐶐�
	CMeshField* pMeshField = new CMeshField;
	
	// ���_���̐ݒ�
	pMeshField->m_nNumVtx = (nSegH + 1) * (nSegV + 1);

	// �|���S�����̐ݒ�
	pMeshField->m_nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

	// �C���f�b�N�X���̐ݒ�
	pMeshField->m_nNumIdx = pMeshField->m_nNumPolygon + 2;

	pMeshField->m_nSegH = nSegH;
	pMeshField->m_nSegV = nSegV;

	pMeshField->m_pos = pos;
	pMeshField->m_Size = Size;
	pMeshField->m_rot = rot;
	
	// ����������
	if (FAILED(pMeshField->Init()))
	{
		pMeshField->Uninit();

		// null�ɂ���
		pMeshField = nullptr;

		return nullptr;
	}

	return pMeshField;
}

//================================================
// ����������
//================================================
HRESULT CMeshField::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// �e�N�X�`����ID�̐ݒ�
	SetTextureID("field.jpg");

	// ���_�̃J�E���g
	int nCntVtx = 0;

	// �e�N�X�`���̍��W�̊���
	float fTexPosX = 1.0f / m_nSegH;
	float fTexPosY = 1.0f / m_nSegV;

	// �v�Z�p�̈ʒu
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= m_nSegV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_nSegH; nCntX++)
		{
			// �ʒu�̐ݒ�
			posWk.x = ((m_Size.x / m_nSegH) * nCntX) - (m_Size.x * 0.5f);
			posWk.y = m_pos.y;
			posWk.z = m_Size.y - ((m_Size.y / m_nSegV) * nCntZ) - (m_Size.y * 0.5f);

			pVtx[nCntVtx].pos = posWk;

			//�@���x�N�g���̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = WHITE;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = m_nSegH + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < m_nSegV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_nSegH; IndxCount2++, IndxNum++, Num++)
		{
			// �C���f�b�N�X�o�b�t�@�̐ݒ�
			pIdx[IdxCnt] = static_cast<WORD>(IndxNum);
			pIdx[IdxCnt + 1] = static_cast<WORD>(Num);
			IdxCnt += 2;
		}

		// NOTE:�Ō�̍s����Ȃ�������
		if (IndxCount1 < m_nSegV - 1)
		{
			pIdx[IdxCnt] = static_cast<WORD>(Num - 1);
			pIdx[IdxCnt + 1] = static_cast<WORD>(IndxNum);
			IdxCnt += 2;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuffer->Unlock();

	return S_OK;
}

//================================================
// �I������
//================================================
void CMeshField::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// �������g�̔j��
	CObject::Release();
}

//================================================
// �X�V����
//================================================
void CMeshField::Update(void)
{
	// �@���̍Đݒ�
	UpdateNor();
}

//================================================
// �`�揈��
//================================================
void CMeshField::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���N���X�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffer);

	//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`��������������
	if (m_nTextureIdx == -1)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
	else
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));

	}

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
}

//================================================
// ���b�V���t�B�[���h�̓����蔻��
//================================================
bool CMeshField::Collision(const D3DXVECTOR3 pos,float *pOutHeight)
{
	// ���n����
	bool bLanding = false;

	int nSegH = m_nSegH;
	int nSegV = m_nSegV;

	// 1�}�X�̃T�C�Y
	float GridSizeX = m_Size.x / (float)nSegH;
	float GridSizeZ = m_Size.y / (float)nSegV;

	float X = pos.x + (m_Size.x * 0.5f);
	float Z = (m_Size.y * 0.5f) - pos.z;

	// ���Ԗڂ̃|���S����
	int polyX = (int)(X / GridSizeX);
	int polyZ = (int)(Z / GridSizeZ);

	// ���݂̃|���S���̃C���f�b�N�X�ԍ�
	int polyIndex = ((polyZ * (nSegH - 1) + polyX) * 2) + (polyZ * 6);

	// �|���S�����̐ݒ�
	int nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

	// �C���f�b�N�X���̐ݒ�
	int nNumIndex = nNumPolygon + 2;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < NUM_POLYGON; nCnt++)
	{
		// ���_�̃C���f�b�N�X
		int nCntVertex = (polyIndex + nCnt);

		// �}�C�i�X��������
		if (nCntVertex < 0 || nCntVertex > nNumIndex) continue;

		// �C���f�b�N�X���擾
		int nIdx0 = pIdx[nCntVertex];
		int nIdx1 = pIdx[nCntVertex + 1];
		int nIdx2 = pIdx[nCntVertex + 2];

		//// �I�[�o�[���Ă�����
		//if (nIdx0 >= nCntVertex || nIdx1 >= nCntVertex || nIdx2 >= nCntVertex) continue;

		// ���_���擾
		D3DXVECTOR3 vtx0 = pVtx[nIdx0].pos;
		D3DXVECTOR3 vtx1 = pVtx[nIdx1].pos;
		D3DXVECTOR3 vtx2 = pVtx[nIdx2].pos;

		D3DXVECTOR3 edge0 = vtx1 - vtx0; // �Ӄx�N�g��0
		D3DXVECTOR3 edge1 = vtx2 - vtx1; // �Ӄx�N�g��1
		D3DXVECTOR3 edge2 = vtx0 - vtx2; // �Ӄx�N�g��2

		D3DXVECTOR3 Normal = {};

		if (nCnt % 2 == 0)
		{
			// �����Ԗڂ̎O�p�`
			D3DXVec3Cross(&Normal, &edge0, &edge1);
		}
		else
		{
			// ��Ԗڂ̎O�p�`�i�������t�ɂȂ��Ă���j
			D3DXVec3Cross(&Normal, &edge1, &edge0);
		}

		D3DXVec3Normalize(&Normal, &Normal);
		m_Nor = Normal;

		D3DXVECTOR3 PlayerVec0 = pos - vtx0;
		D3DXVECTOR3 PlayerVec1 = pos - vtx1;
		D3DXVECTOR3 PlayerVec2 = pos - vtx2;

		D3DXVECTOR3 Cross0 = {};
		D3DXVECTOR3 Cross1 = {};
		D3DXVECTOR3 Cross2 = {};

		if (nCnt % 2 == 0)
		{
			// �O�p�`�̒��_����O��
			D3DXVec3Cross(&Cross0, &edge0, &PlayerVec0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &edge1, &PlayerVec1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &edge2, &PlayerVec2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}
		else
		{
			// �O�p�`�̒��_����O��
			D3DXVec3Cross(&Cross0, &PlayerVec0, &edge0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &PlayerVec1, &edge1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &PlayerVec2, &edge2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}

		if (Cross0.y >= 0.0f && Cross1.y >= 0.0f && Cross2.y >= 0.0f)
		{
			//float xz = ((pPos->x - vtx0.x) * Normal.x + (pPos->z - pPos->z) * Normal.z);

			//float fHeight = vtx0.y - (xz / Normal.y);

			// ���ʂ̕�������D���v�Z
			float D = -(Normal.x * vtx0.x + Normal.y * vtx0.y + Normal.z * vtx0.z);

			// �v���C���[�̈ʒu�Ɋ�Â��āA�v���C���[��Y�ʒu���v�Z
			float PosY = (Normal.x * pos.x + Normal.z * pos.z + D) / -Normal.y;

			D3DXVECTOR3 vec = vtx0 - pos;
			D3DXVec3Normalize(&vec, &vec);

			// �v���C���[���|���S���̗������ǂ����̔���
			float dot = D3DXVec3Dot(&Normal, &vec); // �@���ƃv���C���[�����Ƃ̓���

			if (pOutHeight != nullptr)
			{
				*pOutHeight = m_pos.y + PosY;
			}

			if (dot >= 0.0f)
			{
				bLanding = true;
				break;
			}
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuffer->Unlock();

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();

	return bLanding;//�����Ԃ�
}

//================================================
// �@���̍Đݒ菈��
//================================================
void CMeshField::UpdateNor(void)
{
	int nCnt = 0;
	int nSegH = m_nSegH;
	int nSegV = m_nSegV;

	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�������ׂ�
	for (int nCntZ = 0; nCntZ <= nSegV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= nSegH; nCntX++)
		{
			// �v�Z�p���_0,1,2,3,4
			D3DXVECTOR3 vtx0, vtx1, vtx2, vtx3, vtx4;

			// �v�Z�p���_��������
			vtx0 = vtx1 = vtx2 = vtx3 = vtx4 = VEC3_NULL;

			// �v�Z�p�x�N�g��0,1,2,3
			D3DXVECTOR3 vec0, vec1, vec2, vec3;

			// �v�Z�p�x�N�g���̏�����
			vec0 = vec1 = vec2 = vec3 = VEC3_NULL;

			// �v�Z�p�C���f�b�N�X0,1,2,3�̐錾
			int nIdx0 = 0;
			int nIdx1 = 0;
			int nIdx2 = 0;
			int nIdx3 = 0;

			// �v�Z�p�@��0,1,2,3
			D3DXVECTOR3 Nor0, Nor1, Nor2, Nor3;

			// �v�Z�p�@���̏�����
			Nor0 = Nor1 = Nor2 = Nor3 = VEC3_NULL;

			// �v�Z��̖@���i�[�p�ϐ�
			D3DXVECTOR3 Normal = VEC3_NULL;

			// ���̏ゾ������
			if (nCntX == 0)
			{
				// ���ゾ������
				if (nCntZ == 0)
				{
					nIdx0 = 0;
					nIdx1 = 1;
					nIdx2 = nSegH + 1;

					vtx0 = pVtx[nIdx0].pos;
					vtx1 = pVtx[nIdx1].pos;
					vtx2 = pVtx[nIdx2].pos;

					vec0 = vtx1 - vtx0;
					vec1 = vtx2 - vtx0;

					D3DXVec3Cross(&Normal, &vec0, &vec1);
				}
				// ������������
				else if (nCntZ == nSegV)
				{
					nIdx0 = (nSegH + 1) * nSegV;
					nIdx1 = (nSegH + 1) * (nSegV - 1);
					nIdx2 = ((nSegH + 1) * nSegV) + 1;

					vtx0 = pVtx[nIdx0].pos;
					vtx1 = pVtx[nIdx1].pos;
					vtx2 = pVtx[nIdx2].pos;

					vec0 = vtx1 - vtx0;
					vec1 = vtx2 - vtx0;

					D3DXVec3Cross(&Normal, &vec0, &vec1);
				}
				// ���̕�(�p�ȊO)��������
				else if(nCnt == (nSegH + 1) * nCntZ)
				{
					nIdx0 = nCnt - (nSegH + 1);
					nIdx1 = nCnt + 1;
					nIdx2 = nCnt + (nSegH + 1);

					vtx0 = pVtx[nIdx0].pos;
					vtx1 = pVtx[nIdx1].pos;
					vtx2 = pVtx[nIdx2].pos;
					vtx3 = pVtx[nCnt].pos;

					vec0 = vtx0 - vtx3;
					vec1 = vtx1 - vtx3;
					vec2 = vtx2 - vtx3;

					D3DXVec3Cross(&Nor0, &vec0, &vec1);
					D3DXVec3Cross(&Nor1, &vec1, &vec2);

					Normal = (Nor0 + Nor1) * 0.5f;
				}
			}
			// ��̕ӂ�������
			else if (nCntZ == 0 && nCnt < nSegH)
			{
				nIdx0 = (nSegH + 1) + nCntX;
				nIdx1 = nCnt - 1;
				nIdx2 = nCnt + 1;

				vtx0 = pVtx[nCnt].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;
				vtx3 = pVtx[nIdx0].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;
				vec2 = vtx3 - vtx0;

				D3DXVec3Cross(&Nor0, &vec2, &vec0);
				D3DXVec3Cross(&Nor1, &vec1, &vec2);

				Normal = (Nor0 + Nor1) * 0.5f;
			}
			// �E�ゾ������
			else if (nCntX == nSegH && nCntZ == 0)
			{
				nIdx0 = nSegH;
				nIdx1 = nSegH - 1;
				nIdx2 = nSegH + 1 + nCntX;

				vtx0 = pVtx[nIdx0].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&Normal, &vec1, &vec0);
			}
			// ���̕ӂ�������
			else if (nCntZ == nSegV && nCnt < ((nSegH + 1) * (nSegV + 1)) - 1)
			{
				nIdx0 = nCnt - 1;
				nIdx1 = nCnt - (nSegH + 1);
				nIdx2 = nCnt + 1;

				vtx0 = pVtx[nCnt].pos;
				vtx1 = pVtx[nIdx0].pos;
				vtx2 = pVtx[nIdx1].pos;
				vtx3 = pVtx[nIdx2].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;
				vec2 = vtx3 - vtx0;

				D3DXVec3Cross(&Nor0, &vec0, &vec1);
				D3DXVec3Cross(&Nor1, &vec1, &vec2);

				Normal = (Nor0 + Nor1) * 0.5f;
			}
			// �E����������
			else if (nCnt == ((nSegH + 1) * (nSegV + 1)) - 1)
			{
				nIdx0 = ((nSegH + 1) * (nSegV + 1)) - 1;
				nIdx1 = nIdx0 - (nSegH + 1);
				nIdx2 = nIdx0 - 1;

				vtx0 = pVtx[nIdx0].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&Normal, &vec1, &vec0);
			}
			// �E�̕�(�p�ȊO)��������
			else if (nCntX == nSegH && nCnt == (nCntX * (nCntZ + 1)) + nCntZ)
			{
				nIdx0 = nCnt - (nSegH + 1);
				nIdx1 = nCnt - 1;
				nIdx2 = nCnt + (nSegH + 1);

				vtx0 = pVtx[nIdx0].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;
				vtx3 = pVtx[nCnt].pos;

				vec0 = vtx0 - vtx3;
				vec1 = vtx1 - vtx3;
				vec2 = vtx2 - vtx3;

				D3DXVec3Cross(&Nor0, &vec1, &vec0);
				D3DXVec3Cross(&Nor1, &vec2, &vec1);

				Normal = (Nor0 + Nor1) * 0.5f;
			}
			// ����ȊO(�[�����ł��p�ł��Ȃ�)
			else
			{
				nIdx0 = nCnt - 1;
				nIdx1 = nCnt - (nSegH + 1);
				nIdx2 = nCnt + 1;
				nIdx3 = nCnt + (nSegH + 1);

				vtx0 = pVtx[nCnt].pos;
				vtx1 = pVtx[nIdx0].pos;
				vtx2 = pVtx[nIdx1].pos;
				vtx3 = pVtx[nIdx2].pos;
				vtx4 = pVtx[nIdx3].pos;

				vec0 = vtx1 - vtx0; // ��
				vec1 = vtx2 - vtx0; // ��

				vec2 = vtx3 - vtx0; // �E
				vec3 = vtx4 - vtx0; // ��

				D3DXVec3Cross(&Nor0, &vec0, &vec1);
				D3DXVec3Cross(&Nor1, &vec3, &vec0);
				D3DXVec3Cross(&Nor2, &vec1, &vec2);
				D3DXVec3Cross(&Nor3, &vec2, &vec3);

				Normal = (Nor0 + Nor1 + Nor2 + Nor3) * 0.25f;
			}

			// ���K��
			D3DXVec3Normalize(&Normal, &Normal);

			// �@���̍Đݒ�
			pVtx[nCnt].nor = Normal;

			nCnt++;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuffer->Unlock();
}

//================================================
// ���_���W�̎擾
//================================================
D3DXVECTOR3 CMeshField::GetVtxPos(const int nIdx)
{
	VERTEX_3D* pVtx = NULL;

	D3DXVECTOR3 OutPos;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	OutPos = pVtx[nIdx].pos;

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	return OutPos;
}

//================================================
// �e�N�X�`����ID�̐ݒ菈��
//================================================
void CMeshField::SetTextureID(const char* pTextureName)
{
	// �e�N�X�`���}�l�[�W���[�̎擾
	CTextureManager* pTexture = CManager::GetTexture();

	// �t�@�C���p�X
	string filePath = "data/TEXTURE/field/";

	// ������̘A��
	filePath += pTextureName;

	if (pTexture != nullptr)
	{
		// �e�N�X�`����ID�̎擾
		m_nTextureIdx = pTexture->Register(filePath.c_str());
	}
}