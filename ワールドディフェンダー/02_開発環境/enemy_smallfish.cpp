//=============================================================================
//
// enemyu_smallfish.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_smallfish.h"
#include "read.h"
#include "game.h"
#include "meshfield.h"
#include "motion_parts.h"
#include "player.h"
#include "input.h"
#include "object_type_list.h"
#include "convenience_function.h"
#include "enemy_manager.h"
#include "ballast_manager.h"
#include <assert.h>
#include "tutorial.h"
#include "adrenaline_item.h"

const D3DXVECTOR3 CEnemy_SmallFish::INIT_POS = D3DXVECTOR3(1000.0f, 0.0f, -0.0f);
const float CEnemy_SmallFish::MOVE_INERTIA = 2.5f;
const float CEnemy_SmallFish::JUMP_INERTIA = 0.1f; 
const float CEnemy_SmallFish::INIT_RADIUS = 100.0f;
const float CEnemy_SmallFish::SEARCH_RANGE = 1500.0f;
const D3DXVECTOR3 CEnemy_SmallFish::ENEMY_SIZE_MAX = D3DXVECTOR3(INIT_RADIUS, INIT_RADIUS, INIT_RADIUS);
const D3DXVECTOR3 CEnemy_SmallFish::ENEMY_SIZE_MIN = D3DXVECTOR3(-INIT_RADIUS, 0.0f, -INIT_RADIUS);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEnemy_SmallFish::CEnemy_SmallFish()
{
	m_nRandomMoveCnt = 0;
	CMovable_Obj::SetLife(INIT_LIFE);
	CMovable_Obj::SetRadius(INIT_RADIUS);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEnemy_SmallFish::~CEnemy_SmallFish()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CEnemy_SmallFish::Init()
{

	//親クラスの初期化
	if (FAILED(CEnemy::Init()))
	{
		return -1;
	}

	//ライフの設定
	SetLife(INIT_LIFE);

	//初期の位置
	SetPos(INIT_POS);

	//１フレーム前の位置
	SetOldPos(INIT_POS);

	//移動量の初期化
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//向きの初期化
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// モーションモデルの設定処理
	SetMotionModel();

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CEnemy_SmallFish::Uninit()
{
	//モーション
	CMotionParts::DestructionMotionModel(GetMotionNum());

	//親クラスの終了処理
	CEnemy::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CEnemy_SmallFish::Update()
{
	//CMotionParts::UnUpdateDraw(GetMotionNum(),false,false);

	//自身の位置
	D3DXVECTOR3 pos = GetPos();

	//親クラスの更新処理
	CEnemy::Update();

	//マネージャーからプレイヤーの情報の取得
	CManager* pManager = GetManager();

	//プレイヤーの取得
	CPlayer* pPlayer = nullptr;

	//エネミーマネージャーの取得
	CEnemy_Manager* pEnemy_Manager = nullptr;

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

		//プレイヤーの取得
		pPlayer = pTutorial->GetPlayer();
		//エネミーマネージャーの取得
		pEnemy_Manager = pTutorial->GetEnemy_Manager();
		//瓦礫マネージャーの取得
		pBallast_Manager = pTutorial->GetBallast_Manager();
		//メッシュフィールドの取得
		pMeshfield = pTutorial->GetMeshfield();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//プレイヤーの取得
		pPlayer = pGame->GetPlayer();
		//エネミーマネージャーの取得
		pEnemy_Manager = pGame->GetEnemy_Manager();
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
	
	//プレイヤーの位置
	D3DXVECTOR3 PLpos = pPlayer->GetPos();

	//自身の位置
	pos = GetPos();

	//索敵範囲にいるかどうか
	if (CConvenience_Function::CircleCollision(pos, SEARCH_RANGE, PLpos, 0.0f))
	{
		//プレイヤーの方向へのベクトル
		D3DXVECTOR3 vec = CConvenience_Function::PointOrientationVectorGeneration(PLpos, pos);

		vec *= MOVE_INERTIA;

		//Y方向をなくす
		vec.y = GetMove().y;

		//プレイヤーに近づく
		SetMove(vec);
	}
	else
	{
		//ランダム行動
		RandomMove();
	}
	



	//当たり判定(他のエネミーとの)
	pEnemy_Manager->EnemyOnEnemyCollision(this);

	

	

	//-------------------------------------------------------
	// 瓦礫との当たり判定
	//-------------------------------------------------------

	//マップ上のどこに居るか
	int nMapGrid = pMeshfield->CheckPosLocation(pos);

	//マップの奥行にメッシュ数
	int nDepthGrid = pMeshfield->GetMeshZ();

	//当たり判定をチェックするメッシュ
	int aMapGrid[CHECK_RANGE];

	//プレイヤーのいるメッシュ
	int nEnemyMapGrid = nMapGrid - nDepthGrid;

	//プレイヤーのいるメッシュから周り８箇所の割り出し
	for (int nCnt = 0; nCnt < CHECK_RANGE_X; nCnt++)
	{
		aMapGrid[nCnt * CHECK_RANGE_X] = nEnemyMapGrid + nDepthGrid * nCnt - 1;
		aMapGrid[nCnt * CHECK_RANGE_X + 1] = nEnemyMapGrid + nDepthGrid * nCnt;
		aMapGrid[nCnt * CHECK_RANGE_X + 2] = nEnemyMapGrid + nDepthGrid * nCnt + 1;
	}

	//瓦礫との当たり判定
	D3DXVECTOR3 Add = GetPos();

	//指定範囲の瓦礫の当たり判定
	for (int nCnt = 0; nCnt < CHECK_RANGE; nCnt++)
	{
		//瓦礫の当たり判定
		Add = pBallast_Manager->CollisionBallast(aMapGrid[nCnt], GetPos(), GetOldPos(), ENEMY_SIZE_MAX, ENEMY_SIZE_MIN);

		if (Add != GetPos())
		{
			break;
		}

	}

	//瓦礫との当たり判定
	SetPos(Add);


	//エネミーのrot
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveVec = GetMoveVec();
	rot.y = atan2f(MoveVec.x, MoveVec.z) - D3DX_PI;

	//rot設定
	SetRot(rot);


	//モーション
	CMotionParts::MoveMotionModel(GetMotionNum(), 1, &GetPos(), &GetRot());

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CEnemy_SmallFish::Draw()
{
	//親クラスの描画処理
	CEnemy::Draw();
}

//*****************************************************************************
// 終了条件処理
//*****************************************************************************
bool CEnemy_SmallFish::IsUnused()
{
	if (CMovable_Obj::CheckLife())
	{
		CAdrenalineItem::DropAdrenalineItem(GetPos());
		return true;
	}

	return false;
}

//*****************************************************************************
// モーションモデルの設定処理
//*****************************************************************************
void CEnemy_SmallFish::SetMotionModel()
{
	CRead cRead;

	//SetMotionNum(cRead.ReadMotion("data/MOTION/motionenemy.txt"));

	CMotionParts::AllSetObject_Type_List(GetMotionNum(), OBJ_ENEMY);
}

//*****************************************************************************
// ランダム行動
//*****************************************************************************
void CEnemy_SmallFish::RandomMove()
{
	if (m_nRandomMoveCnt > 0)
	{
		m_nRandomMoveCnt--;

		D3DXVECTOR3 moveVec =  GetMoveVec();

		SetMove(MOVE_INERTIA * moveVec);

		return;
	}

	m_nRandomMoveCnt = rand() % RANDOM_MOVE_CNT_MAX;

	float fAngle = D3DXToRadian(rand() % 360);

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vec.x = cosf(fAngle);
	vec.z = sinf(fAngle);
	vec.y = 0.0f;

	D3DXVec3Normalize(&vec,&vec);

	vec *= MOVE_INERTIA;

	vec.y = -2.0f;

	SetMove(vec);
}

void CEnemy_SmallFish::KnockBack(D3DXVECTOR3 vec)
{
	//方向ベクトルにノックバック用移動量を掛ける
	D3DXVECTOR3 KnockBack = vec * 200.0f;

	//移動量の設定
	SetMove(KnockBack);
}
