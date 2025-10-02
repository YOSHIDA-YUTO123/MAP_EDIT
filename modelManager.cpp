//==============================================
//
// ���f���}�l�[�W���[ [modelManager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"
#include <stdio.h>
#include "LoadManager.h"
#include <cassert>
#include "textureManager.h"

using namespace Const;		// ���O���Const���g�p����
using namespace std;		// ���O���std���g�p����

//**********************************************
// �ÓI�����o�ϐ��錾
//**********************************************
int CModelManager::m_nNumAll = 0;	// ���f���̑���

//==============================================
// �R���X�g���N�^
//==============================================
CModelManager::CModelManager()
{
	
}

//==============================================
// �f�X�g���N�^
//==============================================
CModelManager::~CModelManager()
{
}

//==============================================
// �C���f�b�N�X�̓o�^
//==============================================
int CModelManager::Register(const char* pFilename)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nIdx = -1;

	// ���f���̐�
	int nNumModel = static_cast<int>(m_apModelInfo.size());

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		if (pFilename == NULL || m_apModelInfo[nCnt].filepath == NULL)
		{
			continue;
		}

		if (strcmp(m_apModelInfo[nCnt].filepath, pFilename) == 0)
		{
			return nCnt;
		}
	}

	if (nIdx == -1)
	{
		// ���f���̏��
		ModelInfo info = {};

		////X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(pFilename,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&info.pBuffMat,
			NULL,
			&info.dwNumMat,
			&info.pMesh)))
		{
			// ���b�Z�[�W�{�b�N�X�̕\��
			MessageBox(NULL, pFilename, "���f�����ǂݍ��߂܂���ł���", MB_OK | MB_ICONWARNING);

			return -1;
		}

		// ��������R�s�[����
		strncpy(info.filepath, pFilename, sizeof(info.filepath));

		// �傫���̐ݒ�
		SetSize(&info);

		// �}�e���A���̐ݒ�
		SetMaterial(&info);

		// �v�f�̐ݒ�
		m_apModelInfo.push_back(info);

		// �C���f�b�N�X�̔ԍ���Ԃ�
		nIdx = m_nNumAll;

		m_nNumAll++;
	}

	return nIdx;
}

//==============================================
// ���f���̏��̎擾
//==============================================
CModelManager::ModelInfo CModelManager::GetModelInfo(const int nIdx)
{
	// ���f���̐�
	int nNumModel = static_cast<int>(m_apModelInfo.size());

	if (nIdx < 0 || nIdx >= nNumModel)
	{
		assert(false && "�C���f�b�N�X�I�[�o�[ModelInfo");
		return ModelInfo();
	}

	return m_apModelInfo[nIdx];
}

//==============================================
// �傫���̎擾
//==============================================
void CModelManager::SetSize(ModelInfo *pModelInfo)
{
	// ���_���W�̎擾
	int nNumVtx;	// ���_��
	DWORD sizeFVF;  // ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff; // ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = pModelInfo->pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�擾
	sizeFVF = D3DXGetFVFVertexSize(pModelInfo->pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	pModelInfo->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// �ő�̒��_�A�ŏ��̒��_
	D3DXVECTOR3 vtxMin = VEC3_NULL, vtxMax = VEC3_NULL;

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
		if (vtx.x < vtxMin.x)
		{
			vtxMin.x = vtx.x;
		}
		if (vtx.y < vtxMin.y)
		{
			vtxMin.y = vtx.y;
		}
		if (vtx.z < vtxMin.z)
		{
			vtxMin.z = vtx.z;
		}
		if (vtx.x > vtxMax.x)
		{
			vtxMax.x = vtx.x;
		}
		if (vtx.y > vtxMax.y)
		{
			vtxMax.y = vtx.y;
		}
		if (vtx.z > vtxMax.z)
		{
			vtxMax.z = vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	// �T�C�Y�ɑ��
	pModelInfo->Size.x = vtxMax.x - vtxMin.x;
	pModelInfo->Size.y = vtxMax.y - vtxMin.y;
	pModelInfo->Size.z = vtxMax.z - vtxMin.z;

	// ���_�o�b�t�@�̃A�����b�N
	pModelInfo->pMesh->UnlockVertexBuffer();
}

//==============================================
// �}�e���A���̐ݒ菈��
//==============================================
void CModelManager::SetMaterial(ModelInfo* pModelInfo)
{
	// �}�e���A���̎擾
	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pModelInfo->pBuffMat->GetBufferPointer();

	// �e�N�X�`���}�l�[�W���[�̎擾
	CTextureManager* pTextureManager = CManager::GetTexture();

	for (int nCnt = 0; nCnt < (int)pModelInfo->dwNumMat; nCnt++)
	{
		int nTextureIdx = -1;

		// �t�@�C�������g�p���ăe�N�X�`����ǂݍ���
		nTextureIdx = pTextureManager->Register(pMat[nCnt].pTextureFilename);

		pModelInfo->nTextureIdx.push_back(nTextureIdx);
	}
}

//==============================================
// ���ׂĂ�xFile�̃��[�h����
//==============================================
HRESULT CModelManager::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �t�@�C�����J��
	fstream file("data/TXT/Modellist.txt");
	string line, input;

	// null����Ȃ�������
	if (file.is_open())
	{
		// ���[�h�}�l�[�W���[�̐���
		unique_ptr<CLoadManager> pLoad = make_unique<CLoadManager>();

		// �t�@�C������s���ǂݎ��
		while (getline(file, line))
		{
			size_t equal_pos = line.find("="); // =�̈ʒu

			// [=] ���������߂�
			input = line.substr(equal_pos + 1);

			// �����񂪈�v���Ă�����
			if (line.find("FILENAME") != string::npos)
			{
				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pLoad->SetInputvalue(input);

				// ���f���̖��O�i�[�p�ϐ�
				string modelName;

				// ���l��������
				value_Input >> modelName;

				// ���f���̖��O����
				const char* MODEL_NAME = modelName.c_str();

				// ���f���̓ǂݍ���
				Register(MODEL_NAME);
			}
		}

		// �t�@�C�������
		file.close();
		file.clear();
	}
	else
	{
		// ���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���", "modelManager.txt", MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

//==============================================
// ���ׂĂ�xFile�̔j������
//==============================================
void CModelManager::UnLoad(void)
{
	// ���ׂẴ��f���̃N���A
	for (auto itr = m_apModelInfo.begin(); itr != m_apModelInfo.end(); itr++)
	{
		// ���b�V���̔j��
		if (itr->pMesh != nullptr)
		{
			itr->pMesh->Release();
			itr->pMesh = nullptr;
		}

		// �}�e���A���̔j��
		if (itr->pBuffMat != nullptr)
		{
			itr->pBuffMat->Release();
			itr->pBuffMat = nullptr;
		}

		ZeroMemory(&itr->filepath, sizeof(itr->filepath));

		itr->nTextureIdx.clear();
	}

	// �v�f�̃N���A
	m_apModelInfo.clear();
}
