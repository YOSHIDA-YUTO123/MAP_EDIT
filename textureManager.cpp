//===========================================
//
// �e�N�X�`���}�l�[�W���[ [textureManager.h]
// Author:YUTO YOSHIDA
//
//===========================================

//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"
#include<stdio.h>
#include"LoadManager.h"

//*******************************************
// �ÓI�����o�ϐ��錾
//*******************************************
int CTextureManager::m_nNumAll = 0;	// �e�N�X�`���̑���

using namespace std; // ���O���std���g�p

//===========================================
// �R���X�g���N�^
//===========================================
CTextureManager::CTextureManager()
{
}

//===========================================
// �f�X�g���N�^
//===========================================
CTextureManager::~CTextureManager()
{
}

//===========================================
// �e�N�X�`���̔ԍ��̎擾
//===========================================
int CTextureManager::Register(const char* pFilename)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �C���f�b�N�X
	int nIdx = -1;

	// �e�N�X�`���̐�
	int nNumTexture = static_cast<int>(m_apTextureInfo.size());

	for (int nCnt = 0; nCnt < nNumTexture; nCnt++)
	{
		// �ǂ��炩��null��������
		if (m_apTextureInfo[nCnt].filepath == NULL || pFilename == NULL)
		{
			continue;
		}

		// �����񂪈�v���Ă�����
		if (strcmp(m_apTextureInfo[nCnt].filepath, pFilename) == 0)
		{
			return nCnt;
		}
	}

	// �w�肳�ꂽ�e�N�X�`�����o�^����Ă��Ȃ�������
	if (nIdx == -1)
	{
		// �e�N�X�`���̏��
		TextureInfo info;

		// �e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice,
			pFilename,
			&info.pTexture)))
		{
			return -1;
		}

		// ������̃R�s�[
		strncpy(info.filepath, pFilename, sizeof(info.filepath));

		// �v�f�̐ݒ�
		m_apTextureInfo.push_back(info);

		// �C���f�b�N�X�ԍ���Ԃ�
		nIdx = m_nNumAll;

		// �e�N�X�`���̑���
		m_nNumAll++;
	}

	return nIdx;
}

//===========================================
// �e�N�X�`���̎擾����
//===========================================
LPDIRECT3DTEXTURE9 CTextureManager::GetAdress(int nIdx)
{
	// �e�N�X�`���̐�
	int nNumTexture = static_cast<int>(m_apTextureInfo.size());

	// �͈͊O��������
	if (nIdx < 0 || nIdx >= nNumTexture)
	{
		return nullptr;
	}

	return m_apTextureInfo[nIdx].pTexture;
}

//===========================================
// ���ׂẴe�N�X�`���̓ǂݍ��ݏ���
//===========================================
HRESULT CTextureManager::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �t�@�C�����J��
	fstream file("data/TXT/texture.txt");
	string line,input;

	// null����Ȃ�������
	if (file.is_open())
	{
		// ���[�h�}�l�[�W���[�̐���
		unique_ptr<CLoadManager> pLoad = make_unique<CLoadManager>();

		// �t�@�C������s���ǂݎ��
		while (getline(file,line))
		{
			size_t equal_pos = line.find("="); // =�̈ʒu

			// [=] ���������߂�
			input = line.substr(equal_pos + 1);

			// �����񂪈�v���Ă�����
			if (line.find("TEXTURE_NAME") != string::npos)
			{
				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pLoad->SetInputvalue(input);

				// ���f���̖��O�i�[�p�ϐ�
				string fileName;

				// ���l��������
				value_Input >> fileName;

				// ���f���̖��O����
				const char* FILE_NAME = fileName.c_str();

				// �e�N�X�`���̏��
				TextureInfo info;

				// �l���N���A���Ă���
				memset(&info, NULL, sizeof(info));

				// ������̃R�s�[
				strncpy(info.filepath, FILE_NAME, sizeof(info.filepath));

				// �e�N�X�`���̓ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					FILE_NAME,
					&info.pTexture)))
				{
					MessageBox(NULL, FILE_NAME, "�e�N�X�`�����ǂݍ��߂܂���", MB_OK | MB_ICONWARNING);
				}

				// �����𑝂₷
				m_nNumAll++;

				// �v�f�̐ݒ�
				m_apTextureInfo.push_back(info);
			}
		}

		// �t�@�C�������
		file.close();
		file.clear();
	}
	else
	{
		MessageBox(NULL, "�t�@�C�����J���܂���", "textureManager.cpp", MB_OK);
		return S_OK;
	}

	return S_OK;
}

//===========================================
// ���ׂẴe�N�X�`���̉������
//===========================================
void CTextureManager::UnLoad(void)
{
	// �e�N�X�`���̐�
	int nNumTexture = static_cast<int>(m_apTextureInfo.size());

	// ���ׂẴe�N�X�`������
	for (int nCnt = 0; nCnt < nNumTexture; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTextureInfo[nCnt].pTexture != nullptr)
		{
			m_apTextureInfo[nCnt].pTexture->Release();
			m_apTextureInfo[nCnt].pTexture = nullptr;
		}
	}

	// �v�f�̃N���A
	m_apTextureInfo.clear();
}
