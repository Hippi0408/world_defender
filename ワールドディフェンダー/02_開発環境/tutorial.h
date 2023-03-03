//=============================================================================
//
// tutorial.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _TUTORIAL_H_			//このマクロ定義がされなかったら
#define _TUTORIAL_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCamera;
class CLight;
class C2DPolygon;
class CMeshfield;
class CPlayer;
class CBallast_Manager;
class CEnemy_Manager;
class CMesh_Cylinder;
class CAdrenalineItemTutorial;
class CEnemy_Tutorial;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTutorial : public CObject
{
private:
	static const int ITEM_COOL_TIME = 100;
	static const int ENEMY_COOL_TIME = 100;
public:
	CTutorial();
	~CTutorial() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	CCamera* GetCamera() { return m_pCamera; }
	CMeshfield* GetMeshfield() { return m_pMeshfieldBG; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	CBallast_Manager* GetBallast_Manager() { return m_pBallastManager; }
	CEnemy_Manager* GetEnemy_Manager() { return m_pEnmeyManager; }

	void GameEnd() { m_bGameEnd = true; }
private:
	CCamera* m_pCamera;
	CLight* m_pLight;
	CMeshfield* m_pMeshfieldBG;
	CPlayer* m_pPlayer;
	CEnemy_Manager* m_pEnmeyManager;
	CBallast_Manager* m_pBallastManager;
	CMesh_Cylinder* m_pMesh_Cylinder;
	C2DPolygon* m_pUi;
	CAdrenalineItemTutorial* m_pAdrenalineItemTutorial;
	D3DXVECTOR3 m_LightVec;
	bool m_bGameEnd;
	CEnemy_Tutorial* m_pEnemyTutorial;
	int m_nEnemyCoolTime;
	int m_nItemCoolTime;
	bool m_bEnemyCoolTime;
};

#endif