//=============================================================================
//
// enemy.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy.h"
#include "read.h"
#include "game.h"
#include "meshfield.h"
#include "motion_parts.h"
#include "player.h"
#include "input.h"
#include "object_type_list.h"
#include "convenience_function.h"
#include "enemy_manager.h"

#ifdef _DEBUG
#include "psychokinesis_area.h"
#endif // _DEBUG

const D3DXVECTOR3 CEnemy::INIT_POS = D3DXVECTOR3(1000.0f, 0.0f, -0.0f); 
const float CEnemy::MOVE_INERTIA = 0.1f; 
const float CEnemy::JUMP_INERTIA = 0.01f;
const float CEnemy::INIT_RADIUS = 1600.0f;
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEnemy::CEnemy()
{
	CMovable_Obj::SetLife(INIT_LIFE);
	CMovable_Obj::SetRadius(INIT_RADIUS);
	m_Type = ENEMY_00;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEnemy::~CEnemy()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CEnemy::Init()
{
	//親クラスの初期化
	if (FAILED(CMovable_Obj::Init()))
	{
		return -1;
	}


#ifdef _DEBUG
	//サイコキネシスエリアの情報の確保
	m_pPsychokinesis_Area = new CPsychokinesis_Area;

	//初期化
	if (FAILED(m_pPsychokinesis_Area->Init()))
	{
		return -1;
	}

#endif // _DEBUG

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CEnemy::Uninit()
{

#ifdef _DEBUG
	//サイコキネシスエリアの解放
	if (m_pPsychokinesis_Area != nullptr)
	{
		//終了処理
		m_pPsychokinesis_Area->Uninit();
		delete m_pPsychokinesis_Area;
		m_pPsychokinesis_Area = nullptr;
	}
#endif // _DEBUG

	//親クラスの終了処理
	CMovable_Obj::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CEnemy::Update()
{
	D3DXVECTOR3 pos = GetPos();
	CMotionParts::AllSetShadowPos(D3DXVECTOR3(pos.x, 2.0f, pos.z), GetMotionNum());

	D3DXVECTOR3 move = GetMove();


	if (pos.y < 0.0f)
	{
		move.y = 0.0f;
		pos.y = 0.0f;

		SetPos(pos);

	}
	else if(pos.y > 0.0f)
	{
		move.y += m_fMovingDistance;
	}
	
	
	SetMove(move);


	//親クラスの更新
	CMovable_Obj::Update();
	
	

	
#ifdef _DEBUG
	//サイコキネシスエリアの更新（Posあり）
	m_pPsychokinesis_Area->Update(GetPos());

	m_pPsychokinesis_Area->SetRadius(GetRadius());
#endif // _DEBUG

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CEnemy::Draw()
{
	//親クラスの描画
	CMovable_Obj::Draw();

#ifdef _DEBUG
	//サイコキネシスエリアの描画
	m_pPsychokinesis_Area->Draw();
#endif // _DEBUG
}

//*****************************************************************************
// 終了条件処理
//*****************************************************************************
bool CEnemy::IsUnused()
{
	//ライフの確認
	if (CMovable_Obj::CheckLife())
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// モーションモデルの設定処理
//*****************************************************************************
void CEnemy::SetMotionModel()
{
}

void CEnemy::KnockBack(D3DXVECTOR3 vec)
{
}
