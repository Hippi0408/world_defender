//=============================================================================
//
// 瓦礫の管理者処理 [ballast_manager.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "ballast_manager.h"
#include "ballast.h"
#include <assert.h>
#include "game.h"
#include "manager.h"
#include "meshfield.h"
#include "convenience_function.h"
#include "enemy_manager.h"
#include "read.h"
#include "ballast_acquired.h"
#include "sound.h"
#include "tutorial.h"
#include "tps_camera.h"

const float CBallast_Manager::MAP_MAX = 15000.0f;
const D3DXVECTOR3 CBallast_Manager::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const float CBallast_Manager::BASE_RADIUS_PER_RUBBLE_ONE = 500.0f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CBallast_Manager::CBallast_Manager()
{
	m_nPlListNumber = 0;
	m_nMeshfieldNumMax = 0;
	m_pMeshfieldCopy = nullptr;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBallast_Manager::~CBallast_Manager()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CBallast_Manager::Init()
{
	m_pBallastListData = nullptr;
	m_nBallastListDataMax = 0;
	m_nPlListNumber = 0;
	m_nMeshfieldNumMax = 0;
	m_pMeshfieldCopy = nullptr;

	CRead Read;
	m_nBallast_Acquired_Model[0] = Read.ReadXFile("data/MODEL/瓦礫01.x");
	m_nBallast_Acquired_Model[1] = Read.ReadXFile("data/MODEL/瓦礫02.x");
	m_nBallast_Acquired_Model[2] = Read.ReadXFile("data/MODEL/瓦礫03.x");
	m_nBallast_Acquired_Model[3] = Read.ReadXFile("data/MODEL/瓦礫04.x");
	m_nBallast_Acquired_Model[4] = Read.ReadXFile("data/MODEL/瓦礫05.x");

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CBallast_Manager::Uninit()
{
	//リストの解放
	for (int nCnt = 0; nCnt < m_nBallastListDataMax; nCnt++)
	{
		//イテレーターループ
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end();)
		{
			//イテレーターから瓦礫のポインタの代入
			CBallast* pBallast = *itr;

			//瓦礫ポインタの解放
			if (pBallast != nullptr)
			{
				//終了処理
				pBallast->Uninit();
				delete pBallast;
				pBallast = nullptr;
			}

			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_pBallastListData[nCnt].erase(itr);
		}
	}

	//瓦礫全体ポインタの解放
	delete[] m_pBallastListData;
	m_pBallastListData = nullptr;
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CBallast_Manager::Update()
{
	//マネージャーからプレイヤーの情報の取得
	CManager* pManager = GetManager();

	//エネミーマネージャーの取得
	CEnemy_Manager* pEnemy_Manager = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:
	case TYPE_GAME:
		//エネミーとの当たり判定
		CollisionEnemy();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
		break;
	default:
		assert(false);
		break;
	}

	//リストの更新
	for (int nCnt = 0; nCnt < m_nBallastListDataMax; nCnt++)
	{
		//イテレーターループ
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end(); )
		{
			//イテレーターから瓦礫のポインタの代入
			CBallast* pBallast = *itr;

			//瓦礫ポインタの更新処理
			if (pBallast == nullptr)
			{
				assert(false);
			}

			//使用状態かどうか
			if (pBallast->GetUse())
			{
				D3DXVECTOR3 pos = pBallast->GetPos() + pBallast->GetParentPos();
				//エリア外にあるかどうか
				if (CConvenience_Function::CircleCollision(D3DXVECTOR3(0.0f,0.0f,0.0f), MAP_MAX, pos, 0.0f))
				{
					//更新処理
					pBallast->Update();

					//イテレーターを進める
					itr++;

					continue;
				}
			}
			
			
			//終了処理
			pBallast->Uninit();

			//破棄
			delete pBallast;
			pBallast = nullptr;

			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_pBallastListData[nCnt].erase(itr);
		}
	}

	
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CBallast_Manager::Draw()
{

	//マネージャーからプレイヤーの情報の取得
	CManager* pManager = GetManager();

	//カメラのポインター
	CTpsCamera* pTpsCamera = nullptr;

	CTutorial* pTutorial = nullptr;
	CGame* pGame = nullptr;

	//注視点と視点
	D3DXVECTOR3 posR, posV;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//チュートリアルの取得
		pTutorial = (CTutorial*)pManager->GetGameObject();
		//カメラの取得
		pTpsCamera = (CTpsCamera*)pTutorial->GetCamera();

		posR = pTpsCamera->GetPosR();
		posV = pTpsCamera->GetPosV();
		break;

	case TYPE_GAME:

		//ゲームの取得
		pGame = (CGame*)pManager->GetGameObject();
		//カメラの取得
		pTpsCamera = (CTpsCamera*)pGame->GetCamera();

		posR = pTpsCamera->GetPosR();
		posV = pTpsCamera->GetPosV();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
		posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	default:
		assert(false);
		break;
	}


	//リストの更新
	for (int nCnt = 0; nCnt < m_nMeshfieldNumMax; nCnt++)
	{
		//イテレーターループ
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end(); itr++)
		{
			//イテレーターから瓦礫のポインタの代入
			CBallast* pBallast = *itr;

			//瓦礫の描画
			if (pBallast != nullptr)
			{
				if (pBallast->IsExistingDuring2Point(posR, posV))
				{
					pBallast->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), 0.0f);
					pBallast->SetWithinRangeColor(true);
				}
				else
				{
					pBallast->SetWithinRangeColor(false);
				}

				//描画処理
				pBallast->Draw();
			}
		}
	}
}

//*****************************************************************************
//瓦礫の設定
//*****************************************************************************
void CBallast_Manager::SetBallast(int nNumber, Object_Data Data)
{
	if (nNumber < 0 || nNumber >= m_nBallastListDataMax)
	{
		return;
	}

	//瓦礫クラスの生成
	CBallast* pBallast = new CBallast;

	//初期化
	if (FAILED(pBallast->Init()))
	{
		assert(false);
	}

	//メッシュ内の位置（番号）
	pBallast->SetListNumber(nNumber);

	//使用するモデル番号、瓦礫の位置、瓦礫の向き
	pBallast->Set3DObject(Data.nPattn, D3DXVECTOR3(0.0f,0.0f,0.0f), Data.rot);
	pBallast->SetParentPos(Data.pos);

	//法線設定
	pBallast->SetNormal();

	//半径設定
	pBallast->SetRadius();

	//影の設定
	pBallast->SetLightVec(m_Light);

	//リストに瓦礫情報を追加
	m_pBallastListData[nNumber].push_back(pBallast);
}

//*****************************************************************************
//メッシュフィールドの設定
//*****************************************************************************
void CBallast_Manager::MeshfieldSet(CMeshfield* pMeshfield)
{
	//保存先のNullチェック
	if (m_pMeshfieldCopy != nullptr)
	{
		assert(false);
	}

	//ゲームから地面（メッシュ）の取得
	m_pMeshfieldCopy = pMeshfield;

	//メッシュのマスの数の最大の設定
	m_nMeshfieldNumMax = m_pMeshfieldCopy->GetMeshfieldNumMax();

}

//*****************************************************************************
//サイコキネシスエリアにあったらそのオブジェクトのポインタを返す
//*****************************************************************************
CBallast * CBallast_Manager::CheckCircleCollision(D3DXVECTOR3 pos, float fRadius, int nAdrenaline)
{
	//リストの更新
	for (int nCnt = 0; nCnt < m_nMeshfieldNumMax; nCnt++)
	{
		//イテレーターループ
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end(); itr++)
		{
			//イテレーターから瓦礫のポインタの代入
			CBallast* pBallast = *itr;

			//瓦礫NULLチェック
			if (pBallast == nullptr)
			{
				assert(false);
			}

			//使用状態かどうか
			if (!pBallast->GetUse())
			{
				//処理を無視する
				continue;
			}

			//浮遊状態かどうか
			if (pBallast->GetFloating())
			{
				continue;
			}

			//サイズポイントの取得
			int nSizePoint = pBallast->GetSizePoint();
			
			//その物を浮かせれるかどうか
			if (nSizePoint > nAdrenaline)
			{
				//処理を無視する
				continue;
			}

			//サイコキネシスエリアにあるかどうか
			if (CConvenience_Function::CircleCollision(pos,fRadius, pBallast->GetParentPos(), 0.0f))
			{
				return pBallast;
			}

		}
	}

	return nullptr;
}

//*****************************************************************************
//サイコキネシスエリアにあったらそのオブジェクトの色を変える
//*****************************************************************************
void CBallast_Manager::WithinRangeColor(int nMapGrid, D3DXVECTOR3 pos, float fRadius, int nAdrenaline)
{
	if (nMapGrid < 0 || nMapGrid >= m_nBallastListDataMax)
	{
		return;
	}

	//イテレーターループ
	for (auto itr = m_pBallastListData[nMapGrid].begin(); itr != m_pBallastListData[nMapGrid].end(); itr++)
	{
		//変数宣言
		D3DXVECTOR3 Extrusion = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//イテレーターから瓦礫のポインタの代入
		CBallast* pBallast = *itr;

		//瓦礫NULLチェック
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//使用状態かどうか
		if (!pBallast->GetUse())
		{
			//処理を無視する
			continue;
		}

		//浮遊状態かどうか
		if (pBallast->GetFloating())
		{
			continue;
		}

		//サイズポイントの取得
		int nSizePoint = pBallast->GetSizePoint();

		//その物を浮かせれるかどうか
		if (nSizePoint > nAdrenaline)
		{
			pBallast->SetWithinRangeColor(false);
			//処理を無視する
			continue;
		}

		//サイコキネシスエリアにあるかどうかpBallast->GetVtxMax().x
		if (CConvenience_Function::CircleCollision(pos, fRadius, pBallast->GetParentPos(), 0.0f))
		{
			pBallast->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f), 0.05f);
			pBallast->SetWithinRangeColor(true);
		}
		else
		{
			pBallast->SetWithinRangeColor(false);
		}
	}
}

//*****************************************************************************
//障害物の当たり判定
//*****************************************************************************
D3DXVECTOR3 CBallast_Manager::CollisionBallast(int nMapGrid, D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	D3DXVECTOR3 Add = pos;

	if (nMapGrid < 0 || nMapGrid >= m_nBallastListDataMax)
	{
		return Add;
	}

	//イテレーターループ
	for (auto itr = m_pBallastListData[nMapGrid].begin(); itr != m_pBallastListData[nMapGrid].end(); itr++)
	{
		//イテレーターから瓦礫のポインタの代入
		CBallast* pBallast = *itr;

		//瓦礫NULLチェック
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//浮遊状態かどうか
		if (pBallast->GetFloating())
		{
			continue;
		}

		//変数宣言
		D3DXVECTOR3 Extrusion = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//押し出し判定
		Extrusion = pBallast->ConclusionCollision(pos, oldpos, max, min);

		//押し出しがあったかどうか
		if (Extrusion != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			//押し出し変数に入れる
			Add = Extrusion;

			//for文を抜ける
			break;
		}

	}

	//押し出す値を返す
	return Add;
}

//*****************************************************************************
//エネミーとの当たり判定
//*****************************************************************************
void CBallast_Manager::CollisionEnemy()
{
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

	//イテレーターループ
	for (auto itr = m_FloatingBallstList.begin(); itr != m_FloatingBallstList.end();)
	{
		//イテレーターから瓦礫のポインタの代入
		CBallast* pBallast = *itr;

		//瓦礫NULLチェック
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//使用状態かどうか
		if (!pBallast->GetUse())
		{
			//イテレーターを進める
			itr++;

			//処理を無視する
			continue;
		}

		//浮遊状態かどうか
		if (pBallast->GetFloating())
		{
			//イテレーターを進める
			itr++;

			//処理を無視する
			continue;
		}

		//判定
		bool bHit = pEnemy_Manager->EnemyCollision(pBallast->GetPos() + pBallast->GetParentPos(), pBallast->GetRadius());

		if (!bHit)
		{
			//イテレーターを進める
			itr++;

			//処理を無視する
			continue;
		}

		//サウンド
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		
		D3DXVECTOR3 vec = pBallast->GetPosMove() * -1.0f;

		D3DXVec3Normalize(&vec, &vec);

		SetBallastAcquired(vec, pBallast->GetParentPos() + pBallast->GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), pBallast->GetRadius());

		//瓦礫の使用状態を変更
		pBallast->SetUse(false);

		//次のイテレーターの代入、現在のイテレーターを破棄
		itr = m_FloatingBallstList.erase(itr);

	}

}

//*****************************************************************************
//浮遊状態のリスト追加
//*****************************************************************************
void CBallast_Manager::SetFloatingBallst(CBallast * pBallast)
{
	//リストに瓦礫情報を追加
	m_FloatingBallstList.push_back(pBallast);

	pBallast->SetWithinRangeColor(false);
}

//*****************************************************************************
//後天的瓦礫の生成(引数は飛ばしたい方向ベクトル,あとは基本情報)
//*****************************************************************************
void CBallast_Manager::SetBallastAcquired(D3DXVECTOR3 vec, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
{
	if (BASE_RADIUS_PER_RUBBLE_ONE > fRadius)
	{
		return;
	}

	int nBallastNum = (int)fRadius / BASE_RADIUS_PER_RUBBLE_ONE;

	for (int nCnt = 0; nCnt < nBallastNum; nCnt++)
	{
		//マップチップの位置
		int nNumber = 0;

		//吹っ飛ぶ方向
		D3DXVECTOR3 Vec = vec;

		//後天的瓦礫クラスの生成
		CBallast_Acquired* pBallastAcquired = new CBallast_Acquired;

		//初期化
		if (FAILED(pBallastAcquired->Init()))
		{
			assert(false);
		}

		//マップチップの位置確認
		nNumber = m_pMeshfieldCopy->CheckPosLocation(pos);

		if (nNumber < 0 || nNumber >= m_nBallastListDataMax)
		{
			return;
		}

		//メッシュ内の位置（番号）
		pBallastAcquired->SetListNumber(nNumber);

		//使用するモデル番号、瓦礫の位置、瓦礫の向き
		pBallastAcquired->Set3DObject(m_nBallast_Acquired_Model[rand() % BALLAST_PATTERN], D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot);

		pBallastAcquired->SetParentPos(pos);

		//吹っ飛ぶ方向(乱数を含める)
		Vec.x += (float)(rand() % 20 - 10);
		Vec.y += (float)(rand() % 20 - 10);
		Vec.z += (float)(rand() % 20 - 10);

		//ノーマライズ
		D3DXVec3Normalize(&Vec, &Vec);

		//吹っ飛ぶ方向の設定
		pBallastAcquired->SetVec(Vec);

		//法線設定
		pBallastAcquired->SetNormal();

		//半径設定
		pBallastAcquired->SetRadius();

		//影の設定
		pBallastAcquired->SetLightVec(m_Light);

		//リストに瓦礫情報を追加
		m_pBallastListData[nNumber].push_back(pBallastAcquired);
	}
}

//*****************************************************************************
//リスト内の入れ替え
//*****************************************************************************
void CBallast_Manager::ReplacementList(CBallast * pBallast, int nNext)
{
	//最後に登録されていた番号
	int nLastNum = pBallast->GetListNumber();

	if (nNext == nLastNum)
	{
		return;
	}

	//イテレーターループ
	for (auto itr = m_pBallastListData[nLastNum].begin(); itr != m_pBallastListData[nLastNum].end();)
	{
		//イテレーターから瓦礫のポインタの代入
		CBallast* pballast = *itr;

		//瓦礫NULLチェック
		if (pballast == nullptr)
		{
			assert(false);
		}

		//指定のポインタかどうか
		if (pBallast == pballast)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_pBallastListData[nLastNum].erase(itr);

			//処理を抜ける
			break;
		}

		//イテレーターを進める
		itr++;
	}

	//リストに瓦礫情報を追加
	m_pBallastListData[nNext].push_back(pBallast);
}

//*****************************************************************************
//リストの個数分確保
//*****************************************************************************
void CBallast_Manager::CreateBallastListData(int nDataNum)
{
	//データの確保
	m_pBallastListData = new std::list<CBallast*>[nDataNum];

	//Maxの保存
	m_nBallastListDataMax = nDataNum;
}

