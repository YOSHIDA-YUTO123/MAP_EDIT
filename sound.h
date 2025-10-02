//================================================
//
// �T�E���h���� [sound.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*************************************************
// �C���N���[�h�t�@�C��
//*************************************************
#include"main.h"

//*************************************************
// �T�E���h�N���X�̒�`
//*************************************************
class CSound
{
public:

	// �T�E���h�̎��
	typedef enum
	{
		SOUND_LABEL_PARRYPARFECT = 0,	// �p���B�p�[�t�F�N�g
		SOUND_LABEL_PARRYNORMAL,		// �p���B����
		SOUND_LABEL_PARRYWEAK,			// �p���B����
		SOUND_LABEL_ROAR,				// ���K
		SOUND_LABEL_EXPLOSION,			// ����
		SOUND_LABEL_WARK000,			// ����0
		SOUND_LABEL_WARK001,			// ����1
		SOUND_LABEL_BIRDSOUND,			// ���̖���
		SOUND_LABEL_BIRDFLY,			// ������ї���
		SOUND_LABEL_WIND,				// ��
		SOUND_LABEL_PERFECT,			// �p�[�t�F�N�g
		SOUND_LABEL_NORMAL,				// ����
		SOUND_LABEL_WEAK,				// �ア
		SOUND_LABEL_GAME_BGM,			// �Q�[������BGM
		SOUND_LABEL_WARK003,			// �G�̑���
		SOUND_LABEL_SPIKE,				// �X�p�C�N�g���b�v�ɂԂ�������
		SOUND_LABEL_END_RUSH,			// �ːi�I��
		SOUND_LABEL_IMPACT000,			// �Ռ��g0
		SOUND_LABEL_ENEMY_JUMP,			// �G�̃W�����v
		SOUND_LABEL_SWING,				// ���؂艹
		SOUND_LABEL_SP,					// �K�E�Z
		SOUND_LABEL_MENU,				// ���j���[�I��
		SOUND_LABEL_ENTER,				// ����
		SOUND_LABEL_IMPACT001,			// �Ռ��g1
		SOUND_LABEL_TITLE_BGM,			// �^�C�g����BGM
		SOUND_LABEL_RESULT_LOSE,		// �s�k����BGM
		SOUND_LABEL_AVOID,				// ���
		SOUND_LABEL_ENEMY_DEATH,		// �G���S
		SOUND_LABEL_ENEMY_ATTACK,		// �G�̍U��
		SOUND_LABEL_ENEMY_DAMAGE,		// �G�̃_���[�W
		SOUND_LABEL_BLOW,				// �_���[�W���󂯂Ĕw�����璅�n
		SOUND_LABEL_BARRIEL,			// �M
		SOUND_LABEL_RESULT000,			// ���U���g
		SOUND_LABEL_RESULT001,			// ���U���g
		SOUND_LABEL_BGM_RANKING,			// �����L���OBGM
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	HRESULT Play(SOUND_LABEL label,const float fVolume);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	IXAudio2* m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	// �T�E���h���\���̂̒�`
	struct SOUND_INFO
	{
		const char* pFilename;  // �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g
	};

	// �T�E���h�̏��
	SOUND_INFO m_aSoundInfo[SOUND_LABEL_MAX] = 
	{
		{"data/SE/parryParfect.wav", 0},  // �p���B�p�[�t�F�N�g
		{"data/SE/parryNormal.wav", 0},   // ����
		{"data/SE/parryWeak.wav", 0},     // ����
		{"data/SE/roar.wav", 0},		  // ���K
		{"data/SE/explosion.wav", 0},	  // ����
		{"data/SE/wark000.wav", 0},		  // ����0
		{"data/SE/wark001.wav", 0},		  // ����1
		{"data/SE/birdsound.wav", 0},	  // ���̖���
		{"data/SE/BirdFly.wav", 0},		  // �����H�΂���
		{"data/SE/wind.wav", 0},		  // ��
		{"data/SE/perfect.wav", 0},		  // �p�[�t�F�N�g
		{"data/SE/normal.wav", 0},		  // ����
		{"data/SE/weak.wav", 0},		  // �ア
		{"data/BGM/game_bgm.wav", -1},	  // �Q�[������BGM
		{"data/SE/wark003.wav", 0},		  // �G�̑���
		{"data/SE/spike.wav", 0},		  // �X�p�C�N�g���b�v�ɂԂ�������
		{"data/SE/EndRush.wav", 0},		  // �ːi�I��
		{"data/SE/Impact000.wav", 0},	  // �Ռ��g0
		{"data/SE/enemyJump.wav", 0},	  // �G�̃W�����v
		{"data/SE/swing.wav", 0},		  // ���؂艹
		{"data/SE/SP.wav", 0},			  // �K�E�Z
		{"data/SE/menu000.wav", 0},		  // ���j���[�I��
		{"data/SE/Enter.wav", 0},		  // ����
		{"data/SE/impact001.wav", 0},	  // �Ռ��g1
		{"data/BGM/title_bgm.wav", -1},	  // �^�C�g����BGM
		{"data/BGM/result_lose.wav", -1}, // �s�k��BGM
		{"data/SE/avoid.wav", 0},		  // ���
		{"data/SE/enemy_death.wav", 0},	  // �G���S
		{"data/SE/enemy_attack.wav", 0},  // �G�U��
		{"data/SE/enemy_damage.wav", 0},  // �G�_���[�W
		{"data/SE/blow.wav", 0},		  // �������
		{"data/SE/barriel.wav", 0},		  // �M
		{"data/SE/result000.wav", 0},	  // ���U���g
		{"data/SE/result001.wav", 0},     // ���U���g
		{"data/BGM/ranking.wav", -1},	  // �����L���OBGM
	};				
};
#endif
