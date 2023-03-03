//=============================================================================
//
//　瓦礫の処理 [ballast.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _BALLAST_H_			//このマクロ定義がされなかったら
#define _BALLAST_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "3dobject.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBallast : public C3DObject
{
private:
	static const int SURFACE_MAX = 6;
	static const int TOP_MAX = 4;
	
public:
	CBallast();
	~CBallast() override;

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	virtual void Draw() override;//描画処理

	//瓦礫の位置のSet、Get
	void SetListNumber(int nNumber) { m_nListNumber = nNumber; }
	int GetListNumber() { return m_nListNumber; }

	//瓦礫のプレイヤーとの位置のSet、Get、Add
	void SetPositionWithPlayer(D3DXVECTOR3 PositionWithPlayer) { m_PositionWithPlayer = PositionWithPlayer; }
	D3DXVECTOR3 GetPositionWithPlayer() { return m_PositionWithPlayer; }
	void AddPositionWithPlayer(D3DXVECTOR3 PositionWithPlayer) { m_PositionWithPlayer += PositionWithPlayer; }

	//瓦礫のプレイヤーとの向きのSet、Get
	void SetFacingWithPlayer(D3DXVECTOR3 FacingWithPlayer) { m_FacingWithPlayer = FacingWithPlayer; }
	D3DXVECTOR3 GetFacingWithPlayer() { return m_FacingWithPlayer; }

	//瓦礫の親位置の移動量のSet、Get
	void SetParentPosMove(D3DXVECTOR3 ParentPosMove) { m_ParentPosMove = ParentPosMove; }
	D3DXVECTOR3 GetParentPosMove() { return m_ParentPosMove; }

	//使用状態系
	void SetUse(bool bUse) { m_bUse = bUse; }
	bool GetUse() { return m_bUse; }

	//浮遊状態系
	void SetFloating(bool bFloating) { m_bFloating = bFloating; }
	bool GetFloating() { return m_bFloating; }

	//向きのMove
	void SetMoveRot(D3DXVECTOR3 MoveRot) { m_MoveRot = MoveRot; }

	//半径の取得
	float GetRadius() { return m_fRadius; }
	//半径の設定
	void SetRadius();
	//サイズポイントの取得
	int GetSizePoint() { return m_nSizePoint; }

	//当たり判定(まとめ)
	D3DXVECTOR3 ConclusionCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min);

	//カメラ描画の有無
	bool IsExistingDuring2Point(D3DXVECTOR3 posR, D3DXVECTOR3 posV);
	
private:
	D3DXVECTOR3 m_PositionWithPlayer;
	D3DXVECTOR3 m_FacingWithPlayer;
	D3DXVECTOR3 m_ParentPosMove;
	D3DXVECTOR3 m_MoveRot;
	int m_nListNumber;
	bool m_bUse;
	bool m_bFloating;
	float m_fRadius;
	int m_nSizePoint;
	
};

#endif
