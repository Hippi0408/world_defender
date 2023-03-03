//=============================================================================
//
// player.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player.h"
#include "input.h"
#include "read.h"
#include "texture.h"
#include "game.h"
#include "tutorial.h"
#include "tps_camera.h"
#include "meshfield.h"
#include "motion_parts.h"
#include "convenience_function.h"
#include "psychokinesis_area.h"
#include "object_type_list.h"
#include "psychokinesis.h"
#include "ballast_manager.h"
#include "enemy_manager.h"
#include "adrenaline_gauge.h"
#include "tps_camera.h"
#include <assert.h>
#include "adrenaline_item.h"
#include "sound.h"

const D3DXVECTOR3 CPlayer::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
const float CPlayer::PLAYER_GRAVITY = 2.0f;
const float CPlayer::MOVE_NORMAL = 5.0f;
const float CPlayer::MOVE_BACK = MOVE_NORMAL * 0.6;
const float CPlayer::MOVE_DASH = MOVE_NORMAL * 2.0f;
const float CPlayer::MOVE_INERTIA = 0.1f;
const float CPlayer::JUMP_INERTIA = 0.01f;
const float CPlayer::JUMP_POWER = 60.0f;
const float CPlayer::INIT_RADIUS = 60.0f;
const float CPlayer::KNOCK_BACK = 100.0f;
const D3DXVECTOR3 CPlayer::PLAYER_SIZE_MAX = D3DXVECTOR3(15.0f, 100.0f, 15.0f);
const D3DXVECTOR3 CPlayer::PLAYER_SIZE_MIN = D3DXVECTOR3(-15.0f, 0.0f, -15.0f);

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayer::CPlayer()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPlayer::~CPlayer()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CPlayer::Init()
{
	m_nMapGrid = 0;

	SetLife(INIT_LIFE);

	SetPos(INIT_POS);

	SetOldPos(INIT_POS);

	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_CameraVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_RotLowerBody = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_DestRotLowerBody = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bHit = false;

	m_nInvincibleTime = 0;

	SetRadius(INIT_RADIUS);

	CRead cRead;

	SetMotionNum(cRead.ReadMotion("data/MOTION/motionplayer1.txt"));

	m_nMotionNum1 = cRead.ReadMotion("data/MOTION/motionplayer2.txt");


	CMotionParts::AllSetObject_Type_List(GetMotionNum(), OBJ_PLAYER);
	CMotionParts::AllSetObject_Type_List(m_nMotionNum1, OBJ_PLAYER);

	CMotionParts::SettingParent(m_nMotionNum1, GetMotionNum());

	CMotionParts::SetLight(GetLight(), GetMotionNum());
	CMotionParts::SetLight(GetLight(), m_nMotionNum1);


	//サイコキネシスエリアの情報の確保
	m_pPsychokinesis_Area = new CPsychokinesis_Area;

	//初期化
	if (FAILED(m_pPsychokinesis_Area->Init()))
	{
		return -1;
	}

	//サイコキネシスの情報の確保
	m_pPsychokinesis = new CPsychokinesis;

	//初期化
	if (FAILED(m_pPsychokinesis->Init()))
	{
		return -1;
	}

	//アドレナリンゲージ
	m_pAdrenaline_Gauge = new CAdrenaline_Gauge;
	if (FAILED(m_pAdrenaline_Gauge->Init()))
	{
		return -1;
	}


	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CPlayer::Uninit()
{
	//サイコキネシスエリアの解放
	if (m_pPsychokinesis_Area != nullptr)
	{
		//終了処理
		m_pPsychokinesis_Area->Uninit();
		delete m_pPsychokinesis_Area;
		m_pPsychokinesis_Area = nullptr;
	}

	//サイコキネシスの解放
	if (m_pPsychokinesis != nullptr)
	{
		//終了処理
		m_pPsychokinesis->Uninit();
		delete m_pPsychokinesis;
		m_pPsychokinesis = nullptr;
	}

	//アドレナリンゲージの解放
	if (m_pAdrenaline_Gauge != nullptr)
	{
		//終了処理
		m_pAdrenaline_Gauge->Uninit();
		delete m_pAdrenaline_Gauge;
		m_pAdrenaline_Gauge = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CPlayer::Update()
{
	//入力デバイスの取得
	CInput *pInput = CInput::GetKey();

	//親クラスの更新
	CMovable_Obj::Update();

	// 当たり判定系処理
	Collision();

	//ダメージ中は動けない
	if (m_nInvincibleTime < 60)
	{
		//移動の処理の更新
		Move();
	}

	// モーション処理
	Motion();

	//無敵時間の更新
	if (m_bHit)
	{
		if (m_nInvincibleTime > 0)
		{
			m_nInvincibleTime--;
		}
		else
		{
			m_nInvincibleTime = 0;

			m_bHit = false;
		}
	}

	//現在のプレイヤーの位置の取得
	D3DXVECTOR3 pos = GetPos();

	//アドレナリンゲージ更新処理
	m_pAdrenaline_Gauge->Update();


	//マネージャーの取得
	CManager *pManager = GetManager();

	//TPSカメラのポインタ
	CTpsCamera* pTpsCamera = nullptr;

	//チュートリアルのポインタ
	CTutorial* pTutorial = nullptr;

	//ゲームのポインタ
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//TPSカメラの取得
		pTpsCamera = (CTpsCamera*)pTutorial->GetCamera();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//TPSカメラの取得
		pTpsCamera = (CTpsCamera*)pGame->GetCamera();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}


	pTpsCamera->RateCalculation(m_pAdrenaline_Gauge->GetRateCalculation());
	m_pPsychokinesis_Area->RateCalculation(m_pAdrenaline_Gauge->GetRateCalculation());


	//サイコキネシスエリアの更新（Posあり）
	m_pPsychokinesis_Area->Update(pos);

	//サイコキネシスの更新
	m_pPsychokinesis->Update(m_nMapGrid, pos, GetRot(), m_CameraVec, m_pPsychokinesis_Area->GetRadius(), m_pPsychokinesis_Area->GetSizeTop(), m_pAdrenaline_Gauge->GetGaugeValue() + 20);

	/*if (pInput->Trigger(DIK_T))
	{
		m_pAdrenaline_Gauge->AddGauge(10);
	}
*/

	
	CMotionParts::AllSetShadowPos(D3DXVECTOR3(pos.x, 2.0f, pos.z), GetMotionNum());
	CMotionParts::AllSetShadowPos(D3DXVECTOR3(pos.x,2.0f, pos.z), m_nMotionNum1);

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CPlayer::Draw()
{
	//サイコキネシスエリアの描画
	m_pPsychokinesis_Area->Draw();

	//アドレナリンゲージ描画処理
	m_pAdrenaline_Gauge->Draw();
}

//*****************************************************************************
// 移動処理
//*****************************************************************************
void CPlayer::Move()
{
	//入力デバイスの取得
	CInput *pInput = CInput::GetKey();

	//マネージャーの取得
	CManager *pManager = GetManager();

	//TPSカメラのポインタ
	CTpsCamera* pTpsCamera = nullptr;

	//チュートリアルのポインタ
	CTutorial* pTutorial = nullptr;

	//ゲームのポインタ
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//TPSカメラの取得
		pTpsCamera = (CTpsCamera*)pTutorial->GetCamera();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//TPSカメラの取得
		pTpsCamera = (CTpsCamera*)pGame->GetCamera();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}


	//カメラのベクトルの保存
	m_CameraVec = pTpsCamera->GetCameraVec();

	//カメラの向き（Y軸のみ）
	float rotY = pTpsCamera->GetRot();

	//移動量の一時保管
	D3DXVECTOR3 move = GetMove();

	//Move倍率
	float fMove = MOVE_DASH;

	if (pInput->Press(DIK_S))
	{
		fMove = MOVE_BACK;
	}

	//視点移動
	if (pInput->Press(KEY_UP))
	{//上キーが押された
		if (pInput->Press(KEY_LEFT))
		{
			move.x = -sinf(rotY + D3DX_PI * 0.75f) * fMove;
			move.z = -cosf(rotY + D3DX_PI * 0.75f) * fMove;
		}
		else if (pInput->Press(KEY_RIGHT))
		{
			move.x = -sinf(rotY + D3DX_PI * -0.75f) * fMove;
			move.z = -cosf(rotY + D3DX_PI * -0.75f) * fMove;
		}
		else
		{
			move.x = sinf(rotY) * fMove;
			move.z = cosf(rotY) * fMove;
		}
	}
	else if (pInput->Press(KEY_DOWN))
	{//下キーが押された
		if (pInput->Press(KEY_LEFT))
		{
			move.x = -sinf(rotY + D3DX_PI * 0.25f) * fMove;
			move.z = -cosf(rotY + D3DX_PI * 0.25f) * fMove;
		}
		else if (pInput->Press(KEY_RIGHT))
		{
			move.x = -sinf(rotY + D3DX_PI * -0.25f) * fMove;
			move.z = -cosf(rotY + D3DX_PI * -0.25f) * fMove;
		}
		else
		{
			move.x = sinf(rotY + D3DX_PI) * fMove;
			move.z = cosf(rotY + D3DX_PI) * fMove;
		}
	}
	else if (pInput->Press(KEY_LEFT))
	{//左キーが押された
		move.x = sinf(rotY + D3DX_PI * -0.5f) * fMove;
		move.z = cosf(rotY + D3DX_PI * -0.5f) * fMove;
	}
	else if (pInput->Press(KEY_RIGHT))
	{//右キーが押された
		move.x = sinf(rotY + D3DX_PI * 0.5f) * fMove;
		move.z = cosf(rotY + D3DX_PI * 0.5f) * fMove;
	}

	//移動量を保管
	SetMove(move);

	
}

//*****************************************************************************
// 当たり判定系処理
//*****************************************************************************
void CPlayer::Collision()
{
	//マネージャーの取得
	CManager *pManager = GetManager();

	//床のポインタ
	CMeshfield* pMeshfield = nullptr;

	//瓦礫マネージャーのポインタ
	CBallast_Manager* pBallast_Manager = nullptr;

	//エネミーマネージャーのポインタ
	CEnemy_Manager* pEnemy_Manager = nullptr;

	//チュートリアルのポインタ
	CTutorial* pTutorial = nullptr;

	//ゲームのポインタ
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//床の取得
		pMeshfield = pTutorial->GetMeshfield();
		//エネミーマネージャーの取得
		pEnemy_Manager = pTutorial->GetEnemy_Manager();
		//瓦礫マネージャーの取得
		pBallast_Manager = pTutorial->GetBallast_Manager();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//床の取得
		pMeshfield = pGame->GetMeshfield();
		//エネミーマネージャーの取得
		pEnemy_Manager = pGame->GetEnemy_Manager();
		//瓦礫マネージャーの取得
		pBallast_Manager = pGame->GetBallast_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}



	//現在のプレイヤーの位置
	D3DXVECTOR3 pos = GetPos();
	
	//-------------------------------------------------------
	// 床当たり判定
	//-------------------------------------------------------

	////床との当たり判定用変数
	//D3DXVECTOR3 groundpos;

	////プレイヤーがいる床の高さ
	//groundpos = pMeshfield->Collision(pos);

	////地面の高さに合わせる
	//SetPos(groundpos);

	//プレイヤーが既定の高さより下だったら
	if (pos.y < -100.0f)
	{
		pos.y = 0.0f;
		SetPos(pos);
	}


	//-------------------------------------------------------
	// 瓦礫との当たり判定
	//-------------------------------------------------------


	//マップ上のどこに居るか
	m_nMapGrid = pMeshfield->CheckPosLocation(pos);

	//マップの奥行にメッシュ数
	int nDepthGrid = pMeshfield->GetMeshZ();

	//当たり判定をチェックするメッシュ
	int aMapGrid[CHECK_RANGE];

	//プレイヤーのいるメッシュ
	int nPlMapGrid = m_nMapGrid - nDepthGrid;

	//プレイヤーのいるメッシュから周り８箇所の割り出し
	for (int nCnt = 0; nCnt < CHECK_RANGE_X; nCnt++)
	{
		aMapGrid[nCnt * CHECK_RANGE_X] = nPlMapGrid + nDepthGrid * nCnt - 1;
		aMapGrid[nCnt * CHECK_RANGE_X + 1] = nPlMapGrid + nDepthGrid * nCnt;
		aMapGrid[nCnt * CHECK_RANGE_X + 2] = nPlMapGrid + nDepthGrid * nCnt + 1;
	}

	//瓦礫との当たり判定
	D3DXVECTOR3 Add = GetPos();

	//指定範囲の瓦礫の当たり判定
	for (int nCnt = 0; nCnt < CHECK_RANGE; nCnt++)
	{
		//瓦礫の当たり判定
		Add = pBallast_Manager->CollisionBallast(aMapGrid[nCnt], GetPos(), GetOldPos(), PLAYER_SIZE_MAX, PLAYER_SIZE_MIN);

		if (Add != GetPos())
		{
			D3DXVECTOR3 move = GetMove();
			SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
			break;
		}

	}

	//瓦礫との当たり判定
	SetPos(Add);

	//アドレナリンアイテムの取得
	bool bGet = CAdrenalineItem::PlayerCollision(GetPos());

	//取得していた場合
	if (bGet)
	{
		//サウンド
		PlaySound(SOUND_LABEL_SE_ADRENALINE);
		m_pAdrenaline_Gauge->AddGauge(10);
	}

	if (m_bHit)
	{
		return;
	}

	//-------------------------------------------------------
	// エネミーとの当たり判定
	//-------------------------------------------------------

	D3DXVECTOR3 KnockBack = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//判定
	m_bHit = pEnemy_Manager->PlayerCollision(GetPos(),GetRadius(),&KnockBack);

	//ノックバックの発生
	if (!m_bHit)
	{
		return;
	}

	//-------------------------------------------------------
	//ノックバック
	//-------------------------------------------------------

	//サウンド
	PlaySound(SOUND_LABEL_SE_PLAYER_DAMAGE);

	//方向ベクトルにノックバック用移動量を掛ける
	KnockBack *= KNOCK_BACK;

	//移動量の設定
	SetMove(KnockBack);

	//無敵時間の発生
	m_nInvincibleTime = INVINCIBLE_TIME;
}

//*****************************************************************************
// モーション処理
//*****************************************************************************
void CPlayer::Motion()
{
	//入力デバイスの取得
	CInput *pInput = CInput::GetKey();

	//マネージャーの取得
	CManager *pManager = GetManager();

	//TPSカメラのポインタ
	CTpsCamera* pTpsCamera = nullptr;

	//チュートリアルのポインタ
	CTutorial* pTutorial = nullptr;

	//ゲームのポインタ
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//TPSカメラの取得
		pTpsCamera = (CTpsCamera*)pTutorial->GetCamera();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//TPSカメラの取得
		pTpsCamera = (CTpsCamera*)pGame->GetCamera();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//カメラの向き（Y軸のみ）
	float rotY = pTpsCamera->GetRot();

	//プレイヤーをカメラの見てる方向にする
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, rotY + D3DX_PI, 0.0f);

	//Rotの保管
	SetRot(rot);

	//現在のプレイヤーの位置
	D3DXVECTOR3 PLpos = GetPos();

	//再生するモーション番号
	int nMotionNumUp = 0;
	int nMotionNumDown = 0;

	if (m_nInvincibleTime > 60)
	{
		if (m_nInvincibleTime >= INVINCIBLE_TIME)
		{
			//ダウン用のモーション番号
			nMotionNumUp = 4;
			nMotionNumDown = nMotionNumUp;
		}
	}
	else if (pInput->Press(KEY_MOVE))
	{//プレイヤーが動いていたら
		if (pInput->Press(KEY_DOWN))
		{
			//後ろ歩き用のモーション番号
			nMotionNumUp = 3;
		}
		else
		{
			//ダッシュ用のモーション番号
			nMotionNumUp = 2;
		}

		nMotionNumDown = nMotionNumUp;
	}

	if (pInput->Press(MOUSE_INPUT_LEFT) && !m_bHit)
	{
		//攻撃モーション
		nMotionNumDown = 7;
		if (!pInput->Press(KEY_MOVE))
		{
			//攻撃モーション
			nMotionNumUp = 7;
		}
	}


	//下半身のモーション設定
	CMotionParts::MoveMotionModel(GetMotionNum(), nMotionNumUp, &PLpos, &rot);
	//上半身のモーション設定
	CMotionParts::MoveMotionModel(m_nMotionNum1, nMotionNumDown);
}

void CPlayer::AddGauge()
{
	m_pAdrenaline_Gauge->AddGauge(10);
}
