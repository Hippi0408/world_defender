//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "xaudio2.h"					//サウンド処理に必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,				// タイトル
	SOUND_LABEL_BGM_GAME,				// ゲーム
	SOUND_LABEL_BGM_RESULT,				// リザルト
	SOUND_LABEL_SE_DETERMINATION,		// 決定
	SOUND_LABEL_SE_BACK,				// バック
	SOUND_LABEL_SE_UP_DOWN,				// 上下
	SOUND_LABEL_SE_LEFT_RIGHT,			// 左右
	SOUND_LABEL_SE_WARNING,				// 警告音
	SOUND_LABEL_SE_EXPLOSION,			// 爆発音
	SOUND_LABEL_SE_ENEMY_DAMAGE,		// エネミーダメージ
	SOUND_LABEL_SE_PLAYER_DAMAGE,		// プレイヤーダメージ
	SOUND_LABEL_SE_ADRENALINE,			// アドレナリン
	SOUND_LABEL_SE_JUMP,				// ジャンプ
	SOUND_LABEL_SE_FLOATING,			// 浮遊
	SOUND_LABEL_SE_ATTACK,			// アタック
	SOUND_LABEL_SE_SHOUT,			// 叫び
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
bool UseSound(SOUND_LABEL label);

#endif
