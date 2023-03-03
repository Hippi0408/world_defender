//=============================================================================
//
// psychokinesis.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "psychokinesis.h"
#include "ballast_manager.h"
#include "ballast.h"
#include <assert.h>
#include "input.h"
#include "game.h"
#include "manager.h"
#include <time.h>
#include "convenience_function.h"
#include "meshfield.h"
#include "tutorial.h"
#include "sound.h"

const float CPsychokinesis::BALLAST_MOVE = 0.01f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPsychokinesis::CPsychokinesis()
{
	m_nCurrentFloatingNumber = 0;
	m_nCoolTime = 0;
	m_fRadius = 0.0f;
	m_nAdrenalineValue = 0;
	m_PlPos = D3DXVECTOR3();
	m_CameraVec = D3DXVECTOR3();
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPsychokinesis::~CPsychokinesis()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CPsychokinesis::Init()
{

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CPsychokinesis::Uninit()
{
	//全要素の削除
	m_BallastCopyList.clear();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CPsychokinesis::Update()
{
	// 浮遊処理
	SetFloating();

	//入力デバイスの取得
	CInput *pInput = CInput::GetKey();

	//サイコキネシス発動状態だったら
	if (pInput->Press(MOUSE_INPUT_LEFT))
	{
		// サイコキネシス処理
		Psychokinesis();
	}
	else
	{//辞めたら

		if (pInput->Press(MOUSE_INPUT_RIGHT))
		{//クールタイムあり

			//クールタイム計算
			m_nCoolTime--;

			//クールタイムが０より多かったら
			if (m_nCoolTime > 0)
			{
				//クールタイム中
				return;
			}

			//クールタイムの発生
			m_nCoolTime = COOL_TIME;
		}

		// サイコキネシス攻撃処理
		PsychokinesisAttack();
	}
}

//*****************************************************************************
// 更新処理(位置、向き、カメラのVec、半径更新、出現位置)
//*****************************************************************************
void CPsychokinesis::Update(int nPlMap, D3DXVECTOR3 pos, D3DXVECTOR3 rot,  D3DXVECTOR3 CameraVec, float fRadius, float fFloatingHeight, int nAdrenalineValue)
{
	//アドレナリンの値の保存
	m_nAdrenalineValue = nAdrenalineValue;

	//カメラのベクトル
	m_CameraVec = CameraVec;

	//プレイヤーの位置更新
	m_PlPos = pos;

	//カメラの向き更新
	m_CameraRot = rot;

	//サイコキネシスエリアの半径の更新
	m_fRadius = fRadius;

	//サイコキネシスで浮く高さの更新
	m_fFloatingHeight = fFloatingHeight;

	//範囲内の瓦礫の色変え
	BallastWithinRangeColor();

	//更新処理
	Update();

	//マネージャーの取得
	CManager *pManager = GetManager();

	//床のポインタ
	CMeshfield* pMeshfield = nullptr;

	//瓦礫マネージャーのポインタ
	CBallast_Manager* pBallast_Manager = nullptr;

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

		//瓦礫マネージャーの取得
		pBallast_Manager = pTutorial->GetBallast_Manager();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//床の取得
		pMeshfield = pGame->GetMeshfield();
	
		//瓦礫マネージャーの取得
		pBallast_Manager = pGame->GetBallast_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//マップの奥行にメッシュ数
	int nDepthGrid = pMeshfield->GetMeshZ();

	//当たり判定をチェックするメッシュ
	int aMapGrid[9];

	//プレイヤーのいるメッシュ
	int nPlMapGrid = nPlMap - nDepthGrid;

	//プレイヤーのいるメッシュから周り８箇所の割り出し
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		aMapGrid[nCnt * 3] = nPlMapGrid + nDepthGrid * nCnt - 1;
		aMapGrid[nCnt * 3 + 1] = nPlMapGrid + nDepthGrid * nCnt;
		aMapGrid[nCnt * 3 + 2] = nPlMapGrid + nDepthGrid * nCnt + 1;
	}

	//指定範囲の瓦礫の当たり判定
	for (int nCnt = 0; nCnt < 9; nCnt++)
	{
		pBallast_Manager->WithinRangeColor(aMapGrid[nCnt], pos, fRadius, m_nAdrenalineValue);
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CPsychokinesis::Draw()
{

}

//*****************************************************************************
// 浮遊処理
//*****************************************************************************
void CPsychokinesis::SetFloating()
{
	//Sin用カウント
	int nCnt = 0;

	//DestPosの設定
	for (auto itr = m_BallastCopyList.begin(); itr != m_BallastCopyList.end(); itr++)
	{
		//Sin用カウントを進める
		nCnt++;

		//イテレーターから瓦礫のポインタの代入
		CBallast* pBallast = *itr;

		//瓦礫NULLチェック
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//時間をサインカーブに使用
		int nTime = (int)(time(NULL) % TIME) + nCnt;

		//上下させるサインカーブ
		D3DXVECTOR3 Add = D3DXVECTOR3(0.0f, sinf((float)nTime) * 2.0f, 0.0f);
		pBallast->AddPositionWithPlayer(Add);

		//一時保管場所
		D3DXVECTOR3 Pos, Rot, DestPos, move, PositionWithPlayer;

		//計算用のマトリックス
		D3DXMATRIX mtxRot;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxRot);

		Rot = pBallast->GetFacingWithPlayer();

		Rot = CConvenience_Function::NormalizationRot(m_CameraRot - Rot);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);

		//プレイヤーとの位置
		PositionWithPlayer = pBallast->GetPositionWithPlayer();

		//マトリックス変換
		D3DXVec3TransformCoord(&DestPos, &PositionWithPlayer, &mtxRot);

		//瓦礫の現在の位置
		Pos = pBallast->GetPos();

		//移動量の計算
		move = (DestPos - Pos) * 0.01f;

		//瓦礫のMoveを変更
		pBallast->SetPosMove(move);

		//プレイヤーの位置の登録
		pBallast->SetParentPos(m_PlPos);
	}
}

//*****************************************************************************
// サイコキネシス処理
//*****************************************************************************
void CPsychokinesis::Psychokinesis()
{
	//浮遊可能数
	if (m_nCurrentFloatingNumber > (int)m_nAdrenalineValue * 0.1f)
	{
		return;
	}

	//瓦礫のポインタ
	CBallast* pBallast = nullptr;

	//マネージャーの取得
	CManager *pManager = GetManager();

	//瓦礫マネージャーのポインタ
	CBallast_Manager* pBallast_Manager = nullptr;

	//チュートリアルのポインタ
	CTutorial* pTutorial = nullptr;

	//ゲームのポインタ
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//瓦礫マネージャーの取得
		pBallast_Manager = pTutorial->GetBallast_Manager();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();

		//瓦礫マネージャーの取得
		pBallast_Manager = pGame->GetBallast_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//瓦礫マネージャーから対象の瓦礫ポインタの取得
	pBallast = pBallast_Manager->CheckCircleCollision(m_PlPos, m_fRadius, m_nAdrenalineValue);

	//NULLチェック
	if (pBallast == nullptr)
	{
		return;
	}

	//サウンド
	PlaySound(SOUND_LABEL_SE_FLOATING);

	//浮遊数の増加
	m_nCurrentFloatingNumber++;

	//一時保管場所
	D3DXVECTOR3 Pos, DestPos, move;

	//プレイヤーまでの位置
	D3DXVECTOR3 PositionWithPlayer;
	PositionWithPlayer = pBallast->GetParentPos() - m_PlPos;

	//位置の設定
	pBallast->SetPos(PositionWithPlayer);

	//カメラの向きの保存
	pBallast->SetFacingWithPlayer(m_CameraRot);

	//Y値の調整
	PositionWithPlayer.y = m_fFloatingHeight * 3.0f;

	//プレイヤーとの位置設定
	pBallast->SetPositionWithPlayer(PositionWithPlayer);

	//瓦礫の現在の位置
	Pos = pBallast->GetParentPos();

	//移動量の計算
	move = (PositionWithPlayer - Pos) * 0.01f;

	//瓦礫のMoveを変更
	pBallast->SetPosMove(move);

	//プレイヤーの位置の登録
	pBallast->SetParentPos(m_PlPos);

	// 現在時刻の情報で初期化
	srand((unsigned int)time(NULL));

	//回転の設定
	int nRand = rand() % 8 + 1;
	float fRadianMove = (nRand * 0.1f) + 0.2f;
	D3DXVECTOR3 rotmove = D3DXVECTOR3(0.0f, D3DXToRadian(-fRadianMove), 0.0f);
	pBallast->SetMoveRot(rotmove);

	//浮遊状態の設定
	pBallast->SetFloating(true);

	//リストに追加
	m_BallastCopyList.push_back(pBallast);

	//リストに追加
	pBallast_Manager->SetFloatingBallst(pBallast);
}

//*****************************************************************************
// サイコキネシス攻撃処理
//*****************************************************************************
void CPsychokinesis::PsychokinesisAttack()
{
	//Moveの設定
	for (auto itr = m_BallastCopyList.begin(); itr != m_BallastCopyList.end();)
	{
		//イテレーターから瓦礫のポインタの代入
		CBallast* pBallast = *itr;

		//瓦礫を投げる
		if (pBallast != nullptr)
		{
			//各情報のリセット
			D3DXVECTOR3 WorldPos = pBallast->GetWorldPos();
			pBallast->SetParentPos(WorldPos);
			pBallast->SetPos(D3DXVECTOR3(0.0f,0.0f,0.0f));
			pBallast->SetPosMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//pBallast->SetMoveRot(D3DXVECTOR3(D3DXToRadian(-10), 0.0f, 0.0f));

			//投げた方向への移動量の算出
			D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			//計算用のマトリックス
			D3DXMATRIX mtxRot;

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxRot);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_CameraVec.y, m_CameraVec.x, m_CameraVec.z);

			D3DXVec3TransformCoord(&Move,&Move,&mtxRot);

			Move = Move * 50.0f;

		  /*Move.x = sinf(m_CameraRot.y) * -50.0f;
			Move.z = cosf(m_CameraRot.y) * -50.0f;*/

			//移動量の設定
			pBallast->SetPosMove(Move);

			//浮遊状態の設定
			pBallast->SetFloating(false);

		}

		//次のイテレーターの代入、現在のイテレーターを破棄
		itr = m_BallastCopyList.erase(itr);

		//浮遊数の減少
		m_nCurrentFloatingNumber--;

		//サウンド
		PlaySound(SOUND_LABEL_SE_ATTACK);
	}

	//浮遊数の調整
	if (m_nCurrentFloatingNumber < 0)
	{
		m_nCurrentFloatingNumber = 0;
	}
}

//*****************************************************************************
// 範囲内の瓦礫の色変え
//*****************************************************************************
void CPsychokinesis::BallastWithinRangeColor()
{


}
