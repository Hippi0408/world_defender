//=============================================================================
//
// 瓦礫処理 [ballast_acquired.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "ballast_acquired.h"
#include "convenience_function.h"
#include "manager.h"
#include "game.h"
#include "meshfield.h"
#include "ballast_manager.h"
#include "tutorial.h"
#include <assert.h>


const float CBallast_Acquired::GRAVITY = -0.5f;
const float CBallast_Acquired::MOVE_ATTENUATION = 0.1f;
const float CBallast_Acquired::MOVE_MAGNIFICATION = 3.0f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CBallast_Acquired::CBallast_Acquired()
{
	m_Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bFallenGroundOnce = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBallast_Acquired::~CBallast_Acquired()
{
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CBallast_Acquired::Update()
{
	//親クラスの更新
	CBallast::Update();

	if (m_bFallenGroundOnce)
	{
		return;
	}

	if (GetParentPos().y < 0.0f)
	{
		m_bFallenGroundOnce = true;
		SetPosMove(D3DXVECTOR3(0.0f,0.0f,0.0f));

		D3DXVECTOR3 pos = GetParentPos();
		SetParentPos(D3DXVECTOR3(pos.x, 0.0f, pos.z));

		//マネージャーからプレイヤーの情報の取得
		CManager* pManager = GetManager();

		//瓦礫マネージャーの取得
		CBallast_Manager* pBallast_Manager = nullptr;

		//メッシュフィールドの取得
		CMeshfield* pMeshfield = nullptr;

		CTutorial* pTutorial = nullptr;
		CGame* pGame = nullptr;

		switch (pManager->GetCurrentMode())
		{
		case TYPE_TUTORIAL:

			//チュートリアルの取得
			pTutorial = (CTutorial*)pManager->GetGameObject();
			//瓦礫マネージャーの取得
			pBallast_Manager = pTutorial->GetBallast_Manager();
			//メッシュフィールドの取得
			pMeshfield = pTutorial->GetMeshfield();
			break;

		case TYPE_GAME:

			//ゲームの取得
			pGame = (CGame*)pManager->GetGameObject();
			//瓦礫マネージャーの取得
			pBallast_Manager = pGame->GetBallast_Manager();
			//メッシュフィールドの取得
			pMeshfield = pGame->GetMeshfield();
			break;

		case TYPE_TITLE:
		case TYPE_RESULT:
		default:
			assert(false);
			break;
		}


		//マップチップの番号
		int nMap = pMeshfield->CheckPosLocation(GetParentPos());
		//上記の保存
		SetListNumber(nMap);

		//リストの入れ替え
		pBallast_Manager->ReplacementList(this, nMap);
	}
	else if(GetParentPos().y > 0.0f && !GetFloating())
	{
		D3DXVECTOR3 move = GetPosMove();

		move += (move - D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * MOVE_ATTENUATION;

		move.y += GRAVITY;

		SetPosMove(move);
	}
}

//*****************************************************************************
// ぶっ飛ぶベクトルの設定
//*****************************************************************************
void CBallast_Acquired::SetVec(D3DXVECTOR3 Vec)
{
	//保存
	m_Vec = Vec;

	//移動量
	D3DXVECTOR3 move = m_Vec * MOVE_MAGNIFICATION;

	SetPosMove(move);
}
