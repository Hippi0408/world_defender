//=============================================================================
//
// enemy_tutorial.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_tutorial.h"
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

const D3DXVECTOR3 CEnemy_Tutorial::INIT_POS = D3DXVECTOR3(1000.0f, 0.0f, -0.0f);
const float CEnemy_Tutorial::MOVE_INERTIA = 2.5f;
const float CEnemy_Tutorial::JUMP_INERTIA = 0.1f;
const float CEnemy_Tutorial::INIT_RADIUS = 100.0f;
const float CEnemy_Tutorial::SEARCH_RANGE = 1500.0f;
const D3DXVECTOR3 CEnemy_Tutorial::ENEMY_SIZE_MAX = D3DXVECTOR3(INIT_RADIUS, INIT_RADIUS, INIT_RADIUS);
const D3DXVECTOR3 CEnemy_Tutorial::ENEMY_SIZE_MIN = D3DXVECTOR3(-INIT_RADIUS, 0.0f, -INIT_RADIUS);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEnemy_Tutorial::CEnemy_Tutorial()
{
	m_nRandomMoveCnt = 0;
	CMovable_Obj::SetLife(INIT_LIFE);
	CMovable_Obj::SetRadius(INIT_RADIUS);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEnemy_Tutorial::~CEnemy_Tutorial()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CEnemy_Tutorial::Init()
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
void CEnemy_Tutorial::Uninit()
{
	//モーション
	CMotionParts::DestructionMotionModel(GetMotionNum());

	//親クラスの終了処理
	CEnemy::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CEnemy_Tutorial::Update()
{
	//親クラスの更新処理
	CEnemy::Update();

	//モーション
	CMotionParts::MoveMotionModel(GetMotionNum(), 0, &GetPos(), &GetRot());

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CEnemy_Tutorial::Draw()
{
	//親クラスの描画処理
	CEnemy::Draw();
}

//*****************************************************************************
// 終了条件処理
//*****************************************************************************
bool CEnemy_Tutorial::IsUnused()
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
void CEnemy_Tutorial::SetMotionModel()
{
	CMotionParts::AllSetObject_Type_List(GetMotionNum(), OBJ_ENEMY);
}

void CEnemy_Tutorial::KnockBack(D3DXVECTOR3 vec)
{
}
