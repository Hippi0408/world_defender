//=============================================================================
//
// ufo.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "ufo.h"
#include <assert.h>
#include "convenience_function.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "enemy_manager.h"

const float CUfo::POP_RANGE = 30000.0f;
const float CUfo::MOVING_DISTANCE = 30.0f;
const float CUfo::POP_POS_Y = 10000.0f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CUfo::CUfo()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CUfo::~CUfo()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CUfo::Init()
{
	m_nMoveCounter = MOVE_COUNTER_MAX;

	m_nPopInterval = 0;

	//親クラスの初期化
	if (FAILED(C3DObject::Init()))
	{
		assert(false);
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CUfo::Uninit()
{
	//親クラスの終了処理
	C3DObject::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CUfo::Update()
{
	//UFOがエリア外に出たかどうか
	bool bOutsideArea = CConvenience_Function::CircleCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), POP_RANGE, GetParentPos(), 0.0f);

	if (m_nMoveCounter < MOVE_COUNTER_MAX)
	{
		if (!bOutsideArea)
		{
			m_nMoveCounter++;
			return;
		}
	}
	else
	{
		m_nMoveCounter = 0;

		float fAngle = D3DXToRadian(rand() % 360);

		D3DXVECTOR3 Pop = D3DXVECTOR3(0.0f, POP_POS_Y, 0.0f);
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		Pop.x = cosf(fAngle) * POP_RANGE;
		Pop.z = sinf(fAngle) * POP_RANGE;

		SetParentPos(Pop);

		Move = Pop * -1.0f;

		Move.y = 0.0f;

		D3DXVec3Normalize(&Move, &Move);

		Move *= MOVING_DISTANCE;

		SetPosMove(Move);

	}


	//各情報の取得
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move = GetPosMove();
	D3DXVECTOR3 rot = GetRot();

	//位置の更新
	pos = GetParentPos();
	pos += move;
	SetParentPos(pos);

	rot.y += D3DXToRadian(5.0f);

	rot = CConvenience_Function::NormalizationRot(rot);

	SetRot(rot);

	//親クラスの更新処理
	C3DObject::Update();

	// 雑魚敵の出現処理
	EnemyPop();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CUfo::Draw()
{
	//マトリックスの計算
	CalculationMatrix();
	//親クラスの描画処理
	C3DObject::Draw();
}

//*****************************************************************************
// 雑魚敵の出現処理
//*****************************************************************************
void CUfo::EnemyPop()
{
	if (m_nPopInterval < POP_INTERVAL_MAX)
	{
		m_nPopInterval++;
		return;
	}

	m_nPopInterval = 0;

	//マネージャーからプレイヤーの情報の取得
	CManager* pManager = GetManager();

	//エネミーマネージャーの取得
	CEnemy_Manager* pEnemy_Manager = nullptr;

	CTutorial* pTutorial = nullptr;
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();
		//エネミーマネージャーの取得
		pEnemy_Manager = pTutorial->GetEnemy_Manager();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();
		//エネミーマネージャーの取得
		pEnemy_Manager = pGame->GetEnemy_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//エネミーの情報の設定
	EnemyInitData EnemyInitData;
	EnemyInitData.fmove = -50.0f;
	EnemyInitData.pos = GetParentPos();
	EnemyInitData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EnemyInitData.type = ENEMY_01;

	//エネミーの出現
	pEnemy_Manager->CreateEnemy(EnemyInitData);

}
