//=============================================================================
//
// movable_obj.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _MOVABLE_OBJ_H_			//このマクロ定義がされなかったら
#define _MOVABLE_OBJ_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// クラス定義
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

	//基本情報のGet、Set、Add
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


	//使用しているモーションモデル番号のGet、Set
	int GetMotionNum() { return m_nMotionNum; }
	void SetMotionNum(int nMotionNum) { m_nMotionNum = nMotionNum; }

	//ライトの方向VecのSet,Get
	void SetLight(D3DXVECTOR3 vec) { m_Light = vec; }
	D3DXVECTOR3 GetLight() { return m_Light; }

	//ライフのGet、Set、Add
	int GetLife() { return m_nLife; }
	void SetLife(int nLife) { m_nLife = nLife; }
	virtual void AddLife(int nAdd) { m_nLife += nAdd; }
	//ライフが指定数以下(false)、以上（true）の場合,trueを返す
	bool CheckLife(int nLife = 0, bool b = false);

	//半径の取得
	float GetRadius() { return m_fRadius; }
	//半径の設定
	void SetRadius(float fRadius) { m_fRadius = fRadius; }

	//慣性のGet
	virtual float GetMoveInertia() = 0;

	//ジャンプ慣性のGet
	virtual float GetJumpMoveInertia() = 0;
private:
	
	D3DXVECTOR3 m_Pos;			//位置
	D3DXVECTOR3 m_OldPos;		//前回の位置
	D3DXVECTOR3 m_Rot;			//向き
	D3DXVECTOR3 m_OldRot;		//前回の向き
	D3DXVECTOR3 m_Move;			//移動量
	D3DXVECTOR3 m_MoveVec;		//移動方向

	int m_nLife;				//ライフ
	float m_fRadius;			//半径
	int m_nMotionNum;			//使用するモーションモデル番号
	D3DXVECTOR3 m_Light;		//ライトの向き（必ず必要ではない）
};

#endif