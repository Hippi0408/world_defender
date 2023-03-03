//=============================================================================
//
// player.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _PLAYER_H_			//���̃}�N����`������Ȃ�������
#define _PLAYER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "movable_obj.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CShadow;
class CPsychokinesis_Area;
class CPsychokinesis;
class CAdrenaline_Gauge;

//*****************************************************************************
// �N���X��`
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

	//���[�V�����i���o�[
	int GetMotionNum1() { return m_nMotionNum1; }

	//������Get
	float GetMoveInertia() override { return MOVE_INERTIA; }

	//�W�����v������Get
	float GetJumpMoveInertia() override { return JUMP_INERTIA; }

	//�v���C���[�̂��鏡��
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
	int m_nMapGrid;//�}�b�v��̈ʒu
	bool m_bHit;//�_���[�W�̗L��
	int m_nInvincibleTime;
};

#endif