//=============================================================================
//
// player.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _PLAYER_H_			//このマクロ定義がされなかったら
#define _PLAYER_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "movable_obj.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CShadow;
class CPsychokinesis_Area;
class CPsychokinesis;
class CAdrenaline_Gauge;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CMovable_Obj
{
private:
	static const int INIT_LIFE = 100;
	static const int INVINCIBLE_TIME = 180;
	static const int CHECK_RANGE_X = 3;
	static const int CHECK_RANGE_Z = 3;
	static const int CHECK_RANGE = CHECK_RANGE_X * CHECK_RANGE_Z;
	static const float JUMP_POWER;
	static const D3DXVECTOR3 INIT_POS; 
	static const float PLAYER_GRAVITY;
	static const float MOVE_NORMAL;
	static const float MOVE_BACK;
	static const float MOVE_DASH;
	static const float MOVE_INERTIA;
	static const float JUMP_INERTIA;
	static const float INIT_RADIUS;
	static const float KNOCK_BACK;
	static const D3DXVECTOR3 PLAYER_SIZE_MAX;
	static const D3DXVECTOR3 PLAYER_SIZE_MIN;

	

public:
	CPlayer();
	~CPlayer() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void Collision();
	void Motion();

	//モーションナンバー
	int GetMotionNum1() { return m_nMotionNum1; }

	//慣性のGet
	float GetMoveInertia() override { return MOVE_INERTIA; }

	//ジャンプ慣性のGet
	float GetJumpMoveInertia() override { return JUMP_INERTIA; }

	//プレイヤーのいる升目
	int GetMpaGrid() { return m_nMapGrid; }

	void AddGauge();


private:
	int m_nMotionNum1;
	D3DXVECTOR3 m_RotLowerBody;
	D3DXVECTOR3 m_DestRotLowerBody;
	D3DXVECTOR3 m_CameraVec;
	CPsychokinesis_Area* m_pPsychokinesis_Area;
	CPsychokinesis* m_pPsychokinesis;
	CAdrenaline_Gauge* m_pAdrenaline_Gauge;
	int m_nMapGrid;//マップ上の位置
	bool m_bHit;//ダメージの有無
	int m_nInvincibleTime;
};

#endif