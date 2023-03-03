//=============================================================================
//
// psychokinesis.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _PSYCHOKINESIS_H_			//このマクロ定義がされなかったら
#define _PSYCHOKINESIS_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include <list>

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBallast;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPsychokinesis : public CObject
{
private:
	static const float BALLAST_MOVE;
	static const float BALLAST_Y_POS;
	static const float SIN_CURVE;
	static const int TIME = 100;
	static const int COOL_TIME = 30;

public:
	CPsychokinesis();
	~CPsychokinesis() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Update(int nPlMap, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 CameraVec, float fRadius, float fFloatingHeight,int nAdrenalineValue);
	void Draw() override;

	void SetFloating();

	void Psychokinesis();

	void PsychokinesisAttack();

	void BallastWithinRangeColor();

private:
	D3DXVECTOR3 m_CameraRot;
	D3DXVECTOR3 m_PlPos;
	float m_fFloatingHeight;
	float m_fRadius;
	int m_nCoolTime;
	int m_nAdrenalineValue;
	D3DXVECTOR3 m_CameraVec;
	D3DXVECTOR3 m_PositionWithPlayer;
	int m_nCurrentFloatingNumber;
	
	std::list<CBallast*> m_BallastCopyList;//サイコキネシスで浮かせてる物体のリスト（コピーなので解放しない）
	CBallast* m_pBallastCopy;			
};

#endif