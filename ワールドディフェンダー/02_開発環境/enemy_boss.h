//=============================================================================
//
// enemyu_boss.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ENEMY_BOSS_H_			//このマクロ定義がされなかったら
#define _ENEMY_BOSS_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CStatGauge;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy_Boss : public CEnemy
{
private:
	static const int INIT_LIFE = 2000;
	static const int MOVE_VIBRATION = 5;
	static const D3DXVECTOR3 INIT_POS;
	static const float MOVE_INERTIA;
	static const float JUMP_INERTIA;
	static const float INIT_RADIUS;
	static const int RANDOM_MOVE_CNT_MAX = 5 * 60;
	static const int IMMOBILE_TIME = 300;
	
public:
	CEnemy_Boss();
	~CEnemy_Boss() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool IsUnused() override;
	void SetMotionModel() override;

	void AddLife(int nAdd) override;

	// ランダム行動
	void RandomMove();

	//慣性のGet
	float GetMoveInertia() override { return MOVE_INERTIA; }

	//ジャンプ慣性のGet
	float GetJumpMoveInertia() override { return JUMP_INERTIA; }

	void KnockBack(D3DXVECTOR3 vec) override;

private:
	CStatGauge *m_pLife;
	bool m_bAppearanceMotion;
	int m_nRandomMoveCnt;
	int m_nImmobileTime;
};

#endif