//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "xaudio2.h"					//�T�E���h�����ɕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,				// �^�C�g��
	SOUND_LABEL_BGM_GAME,				// �Q�[��
	SOUND_LABEL_BGM_RESULT,				// ���U���g
	SOUND_LABEL_SE_DETERMINATION,		// ����
	SOUND_LABEL_SE_BACK,				// �o�b�N
	SOUND_LABEL_SE_UP_DOWN,				// �㉺
	SOUND_LABEL_SE_LEFT_RIGHT,			// ���E
	SOUND_LABEL_SE_WARNING,				// �x����
	SOUND_LABEL_SE_EXPLOSION,			// ������
	SOUND_LABEL_SE_ENEMY_DAMAGE,		// �G�l�~�[�_���[�W
	SOUND_LABEL_SE_PLAYER_DAMAGE,		// �v���C���[�_���[�W
	SOUND_LABEL_SE_ADRENALINE,			// �A�h���i����
	SOUND_LABEL_SE_JUMP,				// �W�����v
	SOUND_LABEL_SE_FLOATING,			// ���V
	SOUND_LABEL_SE_ATTACK,			// �A�^�b�N
	SOUND_LABEL_SE_SHOUT,			// ����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
bool UseSound(SOUND_LABEL label);

#endif
