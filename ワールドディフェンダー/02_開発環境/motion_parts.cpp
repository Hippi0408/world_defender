//=============================================================================
//
// motion_parts.cpp
// Author : kozuna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "motion_parts.h"
#include "manager.h"
#include <assert.h>
#include "convenience_function.h"
#include "object_type_list.h"

const D3DXVECTOR3 CMotionParts::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CMotionParts::m_nModelMax = 0;
std::list<CMotionParts*> CMotionParts::m_MotionPartslist;			 //全てのパーツのリスト
std::map<int, int> CMotionParts::m_nMotionPlayMotonNum;			//今再生中のモーション番号
std::map<int, int> CMotionParts::m_nMotionDestMotonNum;			//次再生モーション番号
std::map<int, int> CMotionParts::m_nMotionRegistrationNum;			//登録したモーションモデル群のモーションの登録数

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CMotionParts::CMotionParts()
{
	m_nFrame = 0;
	m_nKey = 0;
	m_pParent = nullptr;
	

	m_nModelObjNum = m_nModelMax;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CMotionParts::~CMotionParts()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CMotionParts::Init()
{
	if (FAILED(C3DObject::Init()))
	{
		return -1;
	}
	m_nDestFrame = 0;
	m_bTopUnUpdate = false;
	m_bUnUpdate = true;
	m_bDraw = true;
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CMotionParts::Uninit()
{
	for (int nMotion = 0; nMotion < m_nMotionRegistrationNum[m_nModelObjNum]; nMotion++)
	{
		if (m_MotionKey[nMotion].pKey != nullptr)
		{
			delete[] m_MotionKey[nMotion].pKey;
			m_MotionKey[nMotion].pKey = nullptr;
		}
	}

	C3DObject::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CMotionParts::Update()
{
	// 目的のフレーム
	m_nDestFrame = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey].nFrame;

	// フレームの加算
	m_nFrame++;
	// 目的のフレーム　＜　現在のフレーム
	if (m_nDestFrame < m_nFrame)
	{
		// 次の位置までの計算
		NextMotionPosition();
		//キー数の確認
		KeyCheck();
		
		m_nFrame = 0;
	}

	// posの加算
	AddPosMove(m_PosMove);
	// rotの加算
	AddRot(m_RotMove);

	D3DXVECTOR3 rot = GetRot();

	//正規化
	rot = CConvenience_Function::NormalizationRot(rot);

	SetRot(rot);


}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CMotionParts::Draw()
{
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtx;
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();
	// 親が存在する場合
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxParent);
	}
	
	//自身のマトリックスの計算
	CalculationMatrix();
	mtx = GetMatrix();

	//モデルのマトリックス　＊　親のワールドマトリックス
	D3DXMatrixMultiply(&mtx, &mtx, &mtxParent);

	SetMatrix(mtx);

	C3DObject::Draw();
}

//*****************************************************************************
//	終了条件処理
//*****************************************************************************
bool CMotionParts::IsUnused()
{
	return false;
}

//*****************************************************************************
//	モーション番号指定でセットする
//*****************************************************************************
void CMotionParts::SetMotion(int nMotionNum)
{
	m_nFrame = 0;
	m_nKey = 0;
	D3DXVECTOR3 pos = m_MotionKey[nMotionNum].pKey[m_nKey].pos;
	D3DXVECTOR3 rot = m_MotionKey[nMotionNum].pKey[m_nKey].rot;

	// 目的のフレーム
	m_nDestFrame = m_MotionKey[nMotionNum].pKey[m_nKey].nFrame;

	m_PosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetParentPos(pos);
	SetRot(rot);
}

//*****************************************************************************
// モーションの移動量などのクリア
//*****************************************************************************
void CMotionParts::ClearMotionMove()
{
	m_nFrame = 0;
	m_nKey = 0;
	m_PosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 目的のフレーム
	m_nDestFrame = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey].nFrame;

}

//*****************************************************************************
//実際の動きの登録
//*****************************************************************************
void CMotionParts::SetMotionData(KEY_SET KeyData)
{
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].bLoop = KeyData.bLoop;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].nKeyMax = KeyData.nKeyMax;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].pKey = KeyData.pKey;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].nNextMotionNum = KeyData.nNextMotionNum;
}

//*****************************************************************************
// キー数の確認
//*****************************************************************************
void CMotionParts::KeyCheck()
{
	//キーのカウントを進める
	m_nKey++;

	//目的のキー数
	int nDestKey = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].nKeyMax;

	//キーが規定より多い場合リセット
	if (m_nKey >= nDestKey)
	{
		m_nKey = 0;

		//今のモーション番号
		int nNowMotionNum = m_nMotionPlayMotonNum[m_nModelObjNum];

		//次に再生するモーション番号（例外の場合再生してた番号にする）
		int nNextMotionNum = nNowMotionNum;

		//ループする場合無視
		if (!m_MotionKey[nNowMotionNum].bLoop)
		{
			//次に再生するモーション番号
			nNextMotionNum = m_MotionKey[nNowMotionNum].nNextMotionNum;

			//現在のパーツがパーツ番号が０かどうか
			if (m_nPartsNum == 0)
			{
				m_nMotionPlayMotonNum[m_nModelObjNum] = 0;

				ClearMotionMove();

				AllNextMotionPosition(m_nModelObjNum,m_bTopUnUpdate);
			}
			else
			{
				ClearMotionMove();
			}
		}
	}
}

//*****************************************************************************
// 次の位置までの計算
//*****************************************************************************
void CMotionParts::NextMotionPosition()
{
	D3DXVECTOR3 nowPos, nextPos,nowRot, nextRot;
	int nFrameRatio;
	//目的のキー数
	int nDestKey = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].nKeyMax;
	
	//現在のKEYが目的を超えたら
	if (m_nKey >= nDestKey - 1)
	{
		//今の位置
		nowPos = GetPosMove();
		//次の位置
		nextPos = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].pos;
		//今の向き
		nowRot = GetRot();
		//次の向き
		nextRot = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].rot;
		//全体フレーム数に対しての割合
		nFrameRatio = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].nFrame;
	}
	else
	{
		//今の位置
		nowPos = GetPosMove();
		//次の位置
		nextPos = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].pos;
		//今の向き
		nowRot = GetRot();
		
		//全体フレーム数に対しての割合
		nFrameRatio = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].nFrame;

		//次の向き
		nextRot = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].rot;
	}
	
	//1フレームあたりの動く向き
	D3DXVECTOR3 rotMove;
	//1フレームあたりの動く位置
	D3DXVECTOR3 posMove;
	
	//目的の位置　ー　現在の位置
	D3DXVECTOR3 pos = nextPos - nowPos;

	//目的の向き　ー　現在の向き
	D3DXVECTOR3 rot = nextRot - nowRot;

	//正規化
	rot = CConvenience_Function::NormalizationRot(rot);

	//割合計算
	//位置
	posMove = pos / (float)nFrameRatio;
	m_PosMove = posMove;
	//向き
	rotMove = CConvenience_Function::NormalizationRot(rot / (float)nFrameRatio) ;

	//正規化
	rotMove = CConvenience_Function::NormalizationRot(rotMove);

	m_RotMove = rotMove;
}

//*****************************************************************************
//全部のパーツ次の位置までの計算
//*****************************************************************************
void CMotionParts::AllNextMotionPosition(int nMotionNum, bool bUnUpdate)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		if (pMotionParts->GetModelObjNum() == nMotionNum)
		{
			if (bUnUpdate)
			{
				pMotionParts->SetUnUpdate(false);
			}
			else
			{
				pMotionParts->ClearMotionMove();
				pMotionParts->NextMotionPosition();
			}

		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
// 引数との一致があるかどうか
//*****************************************************************************
bool CMotionParts::GetMotionParts(int nMotionNum, int nPartsNum)
{
	if (m_nModelObjNum == nMotionNum
		&& m_nPartsNum == nPartsNum)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// 引数との一致があるかどうか
//*****************************************************************************
bool CMotionParts::GetMotionParts(int nMotionNum)
{
	if (m_nModelObjNum == nMotionNum)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
//更新と描画のオンオフ
//*****************************************************************************
void CMotionParts::UnUpdateDraw(int nMotionNum, bool bUnUpdate, bool bDraw)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//モーションモデルの更新と描画の有無
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			//更新
			pMotionParts->SetUnUpdate(bUnUpdate);
			//描画
			pMotionParts->SetBoolDraw(bDraw);

		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
// ALL終了処理
//*****************************************************************************
void CMotionParts::ALLUninit()
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts != nullptr)
		{
			//終了処理
			pMotionParts->Uninit();
			delete pMotionParts;
			pMotionParts = nullptr;
		}

		//次のイテレーターの代入、現在のイテレーターを破棄
		itr = m_MotionPartslist.erase(itr);
	}
}

//*****************************************************************************
// ALL更新処理
//*****************************************************************************
void CMotionParts::ALLUpdate()
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからエネミーのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//更新処理をしない
		if (!pMotionParts->GetUnUpdate())
		{
			//イテレーターを進める
			itr++;

			//以下の処理を無視する
			continue;
		}

		//モーションパーツの更新処理
		pMotionParts->Update();

		//終了条件処理
		if (pMotionParts->IsUnused())
		{
			//終了処理
			pMotionParts->Uninit();
			delete pMotionParts;
			pMotionParts = nullptr;

			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
// ALL描画処理
//*****************************************************************************
void CMotionParts::ALLDraw()
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからエネミーのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//描画をしない
		if (!pMotionParts->GetBoolDraw())
		{
			//イテレーターを進める
			itr++;

			//以下の処理を無視する
			continue;
		}
		//モーションパーツの更新処理
		pMotionParts->Draw();

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
// 動くOBJの生成
//*****************************************************************************
int CMotionParts::CreateMotionObj(MotionData* pMotionData, int nPartsMax)
{
	for (int nCnt = 0; nCnt < nPartsMax; nCnt++)
	{
		CMotionParts* pMotionParts = new CMotionParts;

		if (FAILED(pMotionParts->Init()))
		{
			assert(false);
		}

		pMotionParts->SetPartsNum(nCnt);//一つのモーションの中の番号

		pMotionParts->Set3DObject(pMotionData[nCnt].nModelPattern, pMotionData[nCnt].pos, pMotionData[nCnt].rot);
		
		if (pMotionData[nCnt].nParentNum >= 0)
		{
			//親の設定
			pMotionParts->SetMotionParent(GetMotionPartsPointer(m_nModelMax, pMotionData[nCnt].nParentNum));
		}
		
		pMotionParts->SetNormal();

		//リストにエネミー情報を追加
		m_MotionPartslist.push_back(pMotionParts);
	}

	int nModelNum = m_nModelMax;

	//動く物体の登録完了
	SettingCompletion();

	//モーションモデルの再生に必要な情報の初期化
	m_nMotionPlayMotonNum[nModelNum] = 0;
	m_nMotionDestMotonNum[nModelNum] = 0;

	return nModelNum;

}

//*****************************************************************************
// 条件に合うポインタを返す
//*****************************************************************************
CMotionParts * CMotionParts::GetMotionPartsPointer(int nMotionNum, int nPartsNum)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		// 条件に合うポインタを返す
		if (pMotionParts->GetMotionParts(nMotionNum, nPartsNum))
		{
			return pMotionParts;
		}

		//イテレーターを進める
		itr++;
	}

	return nullptr;
}

//*****************************************************************************
//モーションモデルの移動
//*****************************************************************************
void CMotionParts::MoveMotionModel(int nModelNum, int nMotionNum, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bUnUpdate)
{
	CMotionParts* pMotionParts = CMotionParts::GetMotionPartsPointer(nModelNum, 0);

	pMotionParts->SetTopUnUpdate(bUnUpdate);

	if (pMotionParts->GetMotionData().bLoop)
	{
		if (m_nMotionPlayMotonNum[nModelNum] != nMotionNum)
		{
			m_nMotionPlayMotonNum[nModelNum] = nMotionNum;
			//全体の次までの位置の計算
			AllNextMotionPosition(nModelNum);
		}
	}

	if (pos == rot)
	{
		return;
	}

	if (pos != nullptr)
	{
		GetMotionPartsPointer(nModelNum, 0)->SetParentPos(*pos);
	}

	if (rot != nullptr)
	{
		GetMotionPartsPointer(nModelNum, 0)->SetParentRot(*rot);
	}

}

//*****************************************************************************
//モーションモデルの描画の有無
//*****************************************************************************
void CMotionParts::SetBoolDraw(bool bDraw, int nMotionNum)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//モーションモデルの描画の有無
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			pMotionParts->SetBoolDraw(bDraw);
		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
//モーションモデルのライトベクトル
//*****************************************************************************
void CMotionParts::SetLight(D3DXVECTOR3 vec, int nMotionNum)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//モーションモデルのライトベクトル
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			pMotionParts->SetLightVec(vec);
		}
		
		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
//影の設定
//*****************************************************************************
void CMotionParts::AllSetShadowPos(D3DXVECTOR3 pos, int nMotionNum)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//影の設定
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			D3DXVECTOR3 ShadowPos = pMotionParts->GetWorldPos();
			ShadowPos.y = pos.y;
			pMotionParts->SetShadowPos(ShadowPos);
		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
//モーションの登録
//*****************************************************************************
void CMotionParts::SetMotionFileData(const MotionMoveData MotionMoveData, int nMotionNum)
{
	CMotionParts* pMotionParts = nullptr;//一時保管場所
	int nPartsNum = 0;//カウンター
	pMotionParts = GetMotionPartsPointer(nMotionNum, nPartsNum);//条件に合うポインタの獲得
	while (pMotionParts != nullptr)
	{
		//一時保管場所
		KEY_SET KeySet;

		KeySet.pKey = new KEY[MotionMoveData.nKeyMax];

		for (int nCnt = 0; nCnt < MotionMoveData.nKeyMax; nCnt++)
		{
			//フレーム
			KeySet.pKey[nCnt].nFrame = MotionMoveData.pMotionKeyData[nCnt].nFrame;
			//Pos
			KeySet.pKey[nCnt].pos = MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData[nPartsNum].pos;
			//Rot
			KeySet.pKey[nCnt].rot = MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData[nPartsNum].rot;
		}

		//ループ設定
		KeySet.bLoop = MotionMoveData.bLoop;
		//キーの最大
		KeySet.nKeyMax = MotionMoveData.nKeyMax;
		//次に再生するモーション番号
		KeySet.nNextMotionNum = MotionMoveData.nNextMotionNum;

		//登録
		pMotionParts->SetMotionData(KeySet);

		nPartsNum++;//カウンターを進める
		pMotionParts = GetMotionPartsPointer(nMotionNum, nPartsNum);//条件に合うポインタの獲得
	}
	
	//モーションの登録完了数
	m_nMotionRegistrationNum[nMotionNum]++;
}

//*****************************************************************************
//当たり判定（自分のモデル番号、自分の位置）
//*****************************************************************************
bool CMotionParts::AllCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, Object_Type_List myobject_type_list)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//当たり判定（自分のモデル番号、自分の位置）
		Object_Type_List object_type_list = pMotionParts->GetObject_Type_List();
		if (object_type_list == myobject_type_list)
		{
			if (pMotionParts->NormalCollision(pos))
			{
				return true;
			}

		}

		//イテレーターを進める
		itr++;
	}

	return false;
}

//*****************************************************************************
//親を後天的に設定する（子供番号、親番号）
//*****************************************************************************
void CMotionParts::SettingParent(int nChildren, int nParent)
{
	if (nChildren == nParent)
	{
		return;
	}

	CMotionParts* pMotionPartsChildren = nullptr;
	CMotionParts* pMotionPartsParent = nullptr;

	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//親を後天的に設定する（子供番号、親番号）
		if (pMotionParts->GetMotionParts(nChildren, 0))
		{
			pMotionPartsChildren = pMotionParts;
		}

		if (pMotionParts->GetMotionParts(nParent, 0))
		{
			pMotionPartsParent = pMotionParts;
		}

		//イテレーターを進める
		itr++;
	}

	if (pMotionPartsChildren == nullptr || pMotionPartsParent == nullptr)
	{
		return;
	}

	if (pMotionPartsChildren->GetMotionParent() == nullptr)
	{
		pMotionPartsChildren->SetMotionParent(pMotionPartsParent);
	}
}

//*****************************************************************************
//モデルが何かを設定する
//*****************************************************************************
void CMotionParts::AllSetObject_Type_List(int nModelNum, Object_Type_List object_type_list)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターからモーションパーツのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_MotionPartslist.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//モデルが何かを設定する
		int nNum = pMotionParts->GetModelObjNum();
		if (nNum == nModelNum)
		{
			pMotionParts->SetObject_Type_List(object_type_list);

		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
//モーションデータの複製
//*****************************************************************************
int CMotionParts::CopyMotionModel(int nModelNum)
{
	int nPartsNum = 0;//カウンター
	
	//対象のモーションモデルポインタ
	CMotionParts* pTargetMotionParts = nullptr;

	//複製元
	pTargetMotionParts = GetMotionPartsPointer(nModelNum, nPartsNum);//条件に合うポインタの獲得

	//パーツの複製
	while (pTargetMotionParts != nullptr)
	{
		CMotionParts* pMotionParts = new CMotionParts;

		if (FAILED(pMotionParts->Init()))
		{
			assert(false);
		}

		pMotionParts->SetPartsNum(nPartsNum);//一つのモーションの中の番号


		int nModelPattern = pTargetMotionParts->GetModelPattnNum();
		D3DXVECTOR3 pos = pTargetMotionParts->GetPos();
		D3DXVECTOR3 rot = pTargetMotionParts->GetRot();

		//基本情報の保存
		pMotionParts->Set3DObject(nModelPattern, pos, rot);

		D3DXVECTOR3 vec = pTargetMotionParts->GetLightVec();
		//影用設定
		pMotionParts->SetLightVec(vec);

		//親の設定(親が存在するなら)
		if (nPartsNum > 0)
		{
			//親番号の取得
			int nParent = pTargetMotionParts->GetMotionParent()->GetPartsNum();

			//親の設定
			pMotionParts->SetMotionParent(GetMotionPartsPointer(m_nModelMax, nParent));
		}

		//モーションデータの番号
		pMotionParts->SetModelObjNum(m_nModelMax);

		//法線設定
		pMotionParts->SetNormal();

		//リストにモーションモデル情報を追加
		m_MotionPartslist.push_back(pMotionParts);

		//カウンターを進める
		nPartsNum++;

		//対象のモーションモデルポインタ
		pTargetMotionParts = GetMotionPartsPointer(nModelNum, nPartsNum);

	}

	//登録したモーション数の初期化
	m_nMotionRegistrationNum[m_nModelMax] = 0;

	//一時保管場所
	CMotionParts* pMotionParts = nullptr;

	//動きの複製
	nPartsNum = 0;//カウンター

	//複製先
	pMotionParts = GetMotionPartsPointer(m_nModelMax, nPartsNum);//条件に合うポインタの獲得

	//モデルデータ情報の設定
	while (pMotionParts != nullptr)
	{
		//コピー元のモーションデータ
		CMotionParts* pSourceInformationMotionParts = GetMotionPartsPointer(nModelNum, nPartsNum);
		

		//モーションデータ情報の設定
		for (int nCntMotion = 0; nCntMotion < m_nMotionRegistrationNum[nModelNum]; nCntMotion++)
		{
			//一時保管場所
			KEY_SET KeySet;

			//コピー元の情報取得
			KEY_SET KeySourceInformation = pSourceInformationMotionParts->GetKeyData(nCntMotion);

			//Keyの最大数
			int nKeyMax = KeySourceInformation.nKeyMax;

			//Key情報の生成
			KeySet.pKey = new KEY[nKeyMax];

			//Keyデータ情報の設定
			for (int nCntKey = 0; nCntKey < nKeyMax; nCntKey++)
			{
				//フレーム
				KeySet.pKey[nCntKey].nFrame = KeySourceInformation.pKey[nCntKey].nFrame;
				//Pos
				KeySet.pKey[nCntKey].pos = KeySourceInformation.pKey[nCntKey].pos;
				//Rot
				KeySet.pKey[nCntKey].rot = KeySourceInformation.pKey[nCntKey].rot;
			}

			//ループ設定
			KeySet.bLoop = KeySourceInformation.bLoop;
			//キーの最大
			KeySet.nKeyMax = KeySourceInformation.nKeyMax;
			//次に再生するモーション番号
			KeySet.nNextMotionNum = KeySourceInformation.nNextMotionNum;

			//登録
			pMotionParts->SetMotionData(KeySet);

			//モーションの登録完了数
			m_nMotionRegistrationNum[m_nModelMax]++;

		}

		nPartsNum++;//カウンターを進める
		pMotionParts = GetMotionPartsPointer(m_nModelMax, nPartsNum);//条件に合うポインタの獲得

		//登録したモーション数の初期化
		m_nMotionRegistrationNum[m_nModelMax] = 0;
	}

	//モーションモデルの再生に必要な情報の初期化
	m_nMotionPlayMotonNum[nModelNum] = 0;
	m_nMotionDestMotonNum[nModelNum] = 0;

	// 登録したモーション数
	m_nMotionRegistrationNum[m_nModelMax] = m_nMotionRegistrationNum[nModelNum];

	int nNewModelNum = m_nModelMax;

	//動く物体の登録完了
	SettingCompletion();

	return nNewModelNum;

}

//*****************************************************************************
//指定データの破壊
//*****************************************************************************
void CMotionParts::DestructionMotionModel(int nModelNum)
{
	//イテレーターループ
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//イテレーターから指定データのポインタの代入
		CMotionParts* pMotionParts = *itr;

		//モーションパーツポインタの解放
		if (pMotionParts == nullptr)
		{
			assert(false);
		}

		//対象の情報ではなかったら
		if (pMotionParts->GetModelObjNum() != nModelNum)
		{
			//イテレーターを進める
			itr++;

			//以下の処理を無視する
			continue;
		}

		//終了処理
		pMotionParts->Uninit();
		delete pMotionParts;
		pMotionParts = nullptr;

		//次のイテレーターの代入、現在のイテレーターを破棄
		itr = m_MotionPartslist.erase(itr);
	}
		

	//Mapの要素を破壊
	m_nMotionPlayMotonNum.erase(nModelNum);			//今再生中のモーション番号
	m_nMotionDestMotonNum.erase(nModelNum);			//次再生モーション番号
	m_nMotionRegistrationNum.erase(nModelNum);			//登録したモーションモデル群のモーションの登録数
}
