//=============================================================================
//
// enemyu_smallfish.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ENEMY_SMALL_FISH_H_			//���̃}�N����`������Ȃ�������
#define _ENEMY_SMALL_FISH_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "enemy.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy_SmallFish : public CEnemy
{
private:
	static const int CHECK_RANGE_X = 3;
	static const int CHECK_RANGE_Z = 3;
	static const int RANDOM_MOVE_CNT_MAX = 5 * 60;
	static const int CHECK_RANGE = CHECK_RANGE_X * CHECK_RANGE_Z;
	static const int INIT_LIFE = 1;
	static const D3DXVECTOR3 INIT_POS;
	static const float MOVE_INERTIA;
	static const float JUMP_INERTIA;
	static const float INIT_RADIUS; 
	static const float SEARCH_RANGE;
	static const D3DXVECTOR3 ENEMY_SIZE_MAX;
	static const D3DXVECTOR3 ENEMY_SIZE_MIN;
	
public:
	CEnemy_SmallFish();
	~CEnemy_SmallFish() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool IsUnused() override;
	void SetMotionModel() override;

	void RandomMove();

	//������Get
	float GetMoveInertia() override { return MOVE_INERTIA; }

	//�W�����v������Get
	float GetJumpMoveInertia() override { return JUMP_INERTIA; }

	void KnockBack(D3DXVECTOR3 vec) override;

private:
	int m_nRandomMoveCnt;
};

#endif