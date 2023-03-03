//=============================================================================
//
// game.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _GAME_H_			//このマクロ定義がされなかったら
#define _GAME_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCamera;
class CLight;
class C3DPolygon;
class CMeshfield;
class CPlayer;
class CBallast_Manager;
class CEnemy_Manager;
class CMesh_Cylinder;
class CTime;
class CUfo;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CObject
{
private:
	static const int TIME_MAX = 7200;
public:
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	CCamera* GetCamera() { return m_pCamera; }
	CCamera* GetCameraRound() { return m_pCameraRound; }
	CMeshfield* GetMeshfield() { return m_pMeshfieldBG; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	CBallast_Manager* GetBallast_Manager() { return m_pBallastManager; }
	CEnemy_Manager* GetEnemy_Manager() { return m_pEnmeyManager; }

	void SetRoundCamera(int nRoundCntMax);
	void SetRoundCameraPosR(D3DXVECTOR3 posV);

	void SetVibration(int nVibrationMax, int nVibration);
	bool CheckVibration();

	void GameEnd() { m_bGameEnd = true; }

	void TimeHighScoreConfirmation();

private:
	CCamera* m_pCameraRound;
	CCamera* m_pCamera;
	CLight* m_pLight;
	CMeshfield* m_pMeshfieldBG;
	CPlayer* m_pPlayer;
	CEnemy_Manager* m_pEnmeyManager;
	CBallast_Manager* m_pBallastManager;
	CMesh_Cylinder* m_pMesh_Cylinder;
	CTime* m_pTime;
	CUfo* m_pUfo;
	D3DXVECTOR3 m_LightVec;
	int m_nRoundCnt;
	int m_nRoundCntMax;
	bool m_bRoundCamera;
	bool m_bGameEnd;
};

#endif