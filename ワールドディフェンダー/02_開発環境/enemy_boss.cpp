//=============================================================================
//
// enemy_boss.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_boss.h"
#include "read.h"
#include "game.h"
#include "meshfield.h"
#include "motion_parts.h"
#include "player.h"
#include "input.h"
#include "object_type_list.h"
#include "convenience_function.h"
#include "psychokinesis_area.h"
#include "statgauge.h"
#include "camera.h"
#include "player.h"
#include "sound.h"

const D3DXVECTOR3 CEnemy_Boss::INIT_POS = D3DXVECTOR3(1000.0f, 0.0f, -0.0f);
const float CEnemy_Boss::MOVE_INERTIA = 4.0f;
const float CEnemy_Boss::JUMP_INERTIA = 0.1f;
const float CEnemy_Boss::INIT_RADIUS = 1600.0f;
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEnemy_Boss::CEnemy_Boss()
{
	CMovable_Obj::SetLife(INIT_LIFE);
	CMovable_Obj::SetRadius(INIT_RADIUS);
	m_pLife = nullptr;
	m_bAppearanceMotion = false;
	m_nImmobileTime = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEnemy_Boss::~CEnemy_Boss()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CEnemy_Boss::Init()
{
	//親クラスの初期化
	if (FAILED(CEnemy::Init()))
	{
		return -1;
	}

	//マネージャーからゲームの情報取得
	CManager *pManager = GetManager();
	CGame* pGame = (CGame*)pManager->GetGameObject();

	//周回カメラの設定
	pGame->SetRoundCamera(300);

	//カメラの振動設定
	pGame->SetVibration(300,70);

	CStatGauge::SStatGauge StatGauge;

	StatGauge.pos = D3DXVECTOR3(150.0f, 60.0f, 0.0f);
	StatGauge.rot = D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f);
	StatGauge.color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	StatGauge.fXSize = 60.0f;
	StatGauge.fYSize = 1200.0f;
	StatGauge.nValueMax = INIT_LIFE;
	StatGauge.nRecoveryCntMaqx = 0;
	StatGauge.nRecovery = 0;

	m_pLife = new CStatGauge;
	if (FAILED(m_pLife->Init()))
	{
		return -1;
	}
	m_pLife->CreateStatBar(StatGauge);

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
void CEnemy_Boss::Uninit()
{
	//親クラスの終了処理
	CEnemy::Uninit();
	if (m_pLife != nullptr)
	{
		m_pLife->Uninit();
		delete m_pLife;
		m_pLife = nullptr;
	}

}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CEnemy_Boss::Update()
{
	//ライフゲージの更新と死亡チェック
	if (m_pLife != nullptr)
	{
		//死亡チェック
		if (m_pLife->ValueCheck())
		{
			//サウンド
			PlaySound(SOUND_LABEL_SE_SHOUT);

			//マネージャーの情報を取得
			CManager * pManager = GetManager();

			//次のモードに移行
			pManager->NextMode(TYPE_RESULT,300);

			//マネージャーからゲームの情報を取得
			CGame* pGame = (CGame*)pManager->GetGameObject();

			//周回カメラの設定
			pGame->SetRoundCamera(400);

			//カメラの振動設定
			pGame->SetVibration(400,70);

			//ゲームエンドフラグを立てる
			pGame->GameEnd();

			//タイムをマネージャーに更新確認＆保存
			pGame->TimeHighScoreConfirmation();

			//死亡モーションの再生
			CMotionParts::MoveMotionModel(GetMotionNum(), 4, &GetPos(), &GetRot(), true);

			//これより下の全処理を無視する
			return;
		}

		//ライフの更新
		m_pLife->Update();

	}

	//ランダム行動
	RandomMove();

	//親クラスの更新
	CEnemy::Update();

	//マネージャーの取得
	CManager *pManager = GetManager();

	//マネージャーからゲームの情報取得
	CGame* pGame = (CGame*)pManager->GetGameObject();

	//位置を取得
	D3DXVECTOR3 pos = GetPos();

	//周回カメラのposRの設定
	pGame->SetRoundCameraPosR(pos);

	//モーションの再生
	if (pGame->CheckVibration() && !m_bAppearanceMotion)
	{//登場シーン用
		//サウンド
		PlaySound(SOUND_LABEL_SE_SHOUT);
		m_bAppearanceMotion = true;
		CMotionParts::MoveMotionModel(GetMotionNum(), 3, &GetPos(), &GetRot());
	}
	else
	{//ニュートラルモーション用
		CMotionParts::MoveMotionModel(GetMotionNum(), 0, &GetPos(), &GetRot());
	}

	//動いていなかったら以下の処理を無視する
	if (true)
	{
		return;
	}

	//ボスとプレイヤーとの距離保存用変数
	float fPointDistance = 0.0f;

	//ボスとプレイヤーとの距離
	fPointDistance = CConvenience_Function::PointDistance(GetPos(),pGame->GetPlayer()->GetPos());

	//実際に起こす振動量
	int nVibration = 0;

	//ボスが歩くことによる振動
	for (int nCnt = 1; nCnt <= MOVE_VIBRATION; nCnt++)
	{
		//一定距離離れている場合は振動をなくす
		if (fPointDistance < INIT_RADIUS * nCnt)
		{
			nVibration = MOVE_VIBRATION * nCnt;
		}
	}

	//振動量が0のとき
	if (nVibration == 0)
	{
		return;
	}
	
	//カメラの振動設定
	pGame->SetVibration(10, nVibration);

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CEnemy_Boss::Draw()
{
	//親クラスの描画処理
	CEnemy::Draw();
}

//*****************************************************************************
// 終了条件処理
//*****************************************************************************
bool CEnemy_Boss::IsUnused()
{

	if (CMovable_Obj::CheckLife())
	{
		

		return true;
	}


	return false;
}

//*****************************************************************************
// モーションモデルの設定処理
//*****************************************************************************
void CEnemy_Boss::SetMotionModel()
{
	CRead cRead;

	SetMotionNum(cRead.ReadMotion("data/MOTION/motionboss.txt"));

	CMotionParts::SetLight(GetLight(), GetMotionNum());

	CMotionParts::AllSetObject_Type_List(GetMotionNum(), OBJ_ENEMY);
}

//*****************************************************************************
// HPの減少
//*****************************************************************************
void CEnemy_Boss::AddLife(int nAdd)
{
	CMovable_Obj::AddLife(nAdd);

	m_pLife->SetStatGauge(nAdd);
}

//*****************************************************************************
// ランダム行動
//*****************************************************************************
void CEnemy_Boss::RandomMove()
{
	if (m_nImmobileTime < IMMOBILE_TIME)
	{
		m_nImmobileTime++;
		return;
	}


	if (m_nRandomMoveCnt > 0)
	{
		m_nRandomMoveCnt--;

		D3DXVECTOR3 moveVec = GetMoveVec();

		SetMove(MOVE_INERTIA * moveVec);

		return;
	}

	m_nRandomMoveCnt = rand() % RANDOM_MOVE_CNT_MAX;

	float fAngle = D3DXToRadian(rand() % 360);

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vec.x = cosf(fAngle);
	vec.z = sinf(fAngle);
	vec.y = 0.0f;

	D3DXVec3Normalize(&vec, &vec);

	vec *= MOVE_INERTIA;

	vec.y = -2.0f;

	SetMove(vec);

	//エネミーのrot
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveVec = GetMoveVec();
	rot.y = atan2f(MoveVec.x, MoveVec.z) - D3DX_PI;

	//rot設定
	SetRot(rot);
}

void CEnemy_Boss::KnockBack(D3DXVECTOR3 vec)
{
}
