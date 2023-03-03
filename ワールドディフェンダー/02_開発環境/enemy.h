//=============================================================================
//
// enemyu.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ENEMY_H_			//���̃}�N����`������Ȃ�������
#define _ENEMY_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "movable_obj.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
#ifdef _DEBUG
class CPsychokinesis_Area;
#endif // _DEBUG

enum ENEMY_TYPE;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CMovable_Obj
{
private:
	
	static const int INIT_LIFE = 1;
	static const D3DXVECTOR3 INIT_POS;
	static const float MOVE_INERTIA;
	static const float JUMP_INERTIA;
	static const float INIT_RADIUS;

public:
	CEnemy();
	virtual ~CEnemy() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual bool IsUnused();
	virtual void SetMotionModel();

	void SetMovingDistance(float fMovingDistance) { m_fMovingDistance = fMovingDistance; }
	float GetMovingDistance() { return m_fMovingDistance; }

	//������Get
	virtual float GetMoveInertia() override { return MOVE_INERTIA; }

	//�W�����v������Get
	virtual float GetJumpMoveInertia() override { return JUMP_INERTIA; }

	void SetType(ENEMY_TYPE type) { m_Type = type; }
	ENEMY_TYPE GetType() { return m_Type; }

	virtual void KnockBack(D3DXVECTOR3 vec);

private:
	float m_fMovingDistance;
	ENEMY_TYPE m_Type;

#ifdef _DEBUG
	CPsychokinesis_Area* m_pPsychokinesis_Area;
#endif // _DEBUG

};

#endif