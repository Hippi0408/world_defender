//=============================================================================
//
// movable_obj.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _MOVABLE_OBJ_H_			//���̃}�N����`������Ȃ�������
#define _MOVABLE_OBJ_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMovable_Obj : public CObject
{
private:

public:
	CMovable_Obj();
	virtual ~CMovable_Obj() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//��{����Get�ASet�AAdd
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() {return m_Pos; }
	void AddPos(D3DXVECTOR3 addpos) { m_Pos += addpos; }
	void SetOldPos(D3DXVECTOR3 oldpos) { m_OldPos = oldpos; }
	D3DXVECTOR3 GetOldPos() { return m_OldPos; }
	void AddOldPos(D3DXVECTOR3 addpos) { m_OldPos += addpos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void AddRot(D3DXVECTOR3 addrot) { m_Rot += addrot; }
	void SetOldRot(D3DXVECTOR3 oldrot) { m_OldRot = oldrot; }
	D3DXVECTOR3 GetOldRot() { return m_OldRot; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	D3DXVECTOR3 GetMove() { return m_Move; }
	void SetMoveVec(D3DXVECTOR3 movevec) { m_MoveVec = movevec; }
	D3DXVECTOR3 GetMoveVec() { return m_MoveVec; }


	//�g�p���Ă��郂�[�V�������f���ԍ���Get�ASet
	int GetMotionNum() { return m_nMotionNum; }
	void SetMotionNum(int nMotionNum) { m_nMotionNum = nMotionNum; }

	//���C�g�̕���Vec��Set,Get
	void SetLight(D3DXVECTOR3 vec) { m_Light = vec; }
	D3DXVECTOR3 GetLight() { return m_Light; }

	//���C�t��Get�ASet�AAdd
	int GetLife() { return m_nLife; }
	void SetLife(int nLife) { m_nLife = nLife; }
	virtual void AddLife(int nAdd) { m_nLife += nAdd; }
	//���C�t���w�萔�ȉ�(false)�A�ȏ�itrue�j�̏ꍇ,true��Ԃ�
	bool CheckLife(int nLife = 0, bool b = false);

	//���a�̎擾
	float GetRadius() { return m_fRadius; }
	//���a�̐ݒ�
	void SetRadius(float fRadius) { m_fRadius = fRadius; }

	//������Get
	virtual float GetMoveInertia() = 0;

	//�W�����v������Get
	virtual float GetJumpMoveInertia() = 0;
private:
	
	D3DXVECTOR3 m_Pos;			//�ʒu
	D3DXVECTOR3 m_OldPos;		//�O��̈ʒu
	D3DXVECTOR3 m_Rot;			//����
	D3DXVECTOR3 m_OldRot;		//�O��̌���
	D3DXVECTOR3 m_Move;			//�ړ���
	D3DXVECTOR3 m_MoveVec;		//�ړ�����

	int m_nLife;				//���C�t
	float m_fRadius;			//���a
	int m_nMotionNum;			//�g�p���郂�[�V�������f���ԍ�
	D3DXVECTOR3 m_Light;		//���C�g�̌����i�K���K�v�ł͂Ȃ��j
};

#endif