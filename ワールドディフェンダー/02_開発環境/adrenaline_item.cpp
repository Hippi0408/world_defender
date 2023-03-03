//=============================================================================
//
// adrenaline_item.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "adrenaline_item.h"
#include <assert.h>
#include "convenience_function.h"
#include "read.h"

const float CAdrenalineItem::RADIUS = 100.0f;

//静的メンバー変数
int CAdrenalineItem::m_nModelNum;
D3DXVECTOR3 CAdrenalineItem::m_LightVec;
std::list<CAdrenalineItem*> CAdrenalineItem::m_AdrenalineItemList;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAdrenalineItem::CAdrenalineItem()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAdrenalineItem::~CAdrenalineItem()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAdrenalineItem::Init()
{
	//初期化
	if (FAILED(C3DObject::Init()))
	{
		assert(false);
	}
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CAdrenalineItem::Uninit()
{
	//終了処理
	C3DObject::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CAdrenalineItem::Update()
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += D3DXToRadian(5.0f);

	rot = CConvenience_Function::NormalizationRot(rot);

	SetRot(rot);

	//アイテムの更新処理
	C3DObject::Update();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CAdrenalineItem::Draw()
{
	//マトリックスの計算
	CalculationMatrix();
	//アイテムの描画処理
	C3DObject::Draw();
}

//*****************************************************************************   
// 初期化
//*****************************************************************************
HRESULT CAdrenalineItem::AllInit()
{
	CRead Read;

	m_nModelNum = Read.ReadXFile("data/MODEL/アドレナリン.x");

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CAdrenalineItem::AllUninit()
{
	//イテレーターループ
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//イテレーターからアイテムのポインタの代入
		CAdrenalineItem* pAdrenalineItem = *itr;

		//アイテムポインタの解放
		if (pAdrenalineItem != nullptr)
		{
			//終了処理
			pAdrenalineItem->Uninit();
			delete pAdrenalineItem;
			pAdrenalineItem = nullptr;
		}

		//次のイテレーターの代入、現在のイテレーターを破棄
		itr = m_AdrenalineItemList.erase(itr);
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CAdrenalineItem::AllUpdate()
{
	//イテレーターループ
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//イテレーターからアイテムのポインタの代入
		CAdrenalineItem* pAdrenalineItem = *itr;

		//アイテムポインタの解放
		if (pAdrenalineItem == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_AdrenalineItemList.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//アイテムの更新処理
		pAdrenalineItem->Update();

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CAdrenalineItem::AllDraw()
{

	//イテレーターループ
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//イテレーターからアイテムのポインタの代入
		CAdrenalineItem* pAdrenalineItem = *itr;

		//アイテムポインタの解放
		if (pAdrenalineItem == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_AdrenalineItemList.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//アイテムの更新処理
		pAdrenalineItem->Draw();


		//イテレーターを進める
		itr++;
	}

}

//*****************************************************************************
// クリエイト処理
//*****************************************************************************
void CAdrenalineItem::DropAdrenalineItem(D3DXVECTOR3 pos)
{
	//アイテムクラスのポインタ変数
	CAdrenalineItem* pAdrenalineItem = nullptr;

	//動的確保
	pAdrenalineItem = new CAdrenalineItem;
	
	//初期化
	if (FAILED(pAdrenalineItem->Init()))
	{
		assert(false);
	}

	//影の設定
	pAdrenalineItem->SetLightVec(m_LightVec);

	//細かい設定
	pAdrenalineItem->Set3DObject(m_nModelNum, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//アイテムの位置
	pAdrenalineItem->SetParentPos(pos);

	//リストにアイテム情報を追加
	m_AdrenalineItemList.push_back(pAdrenalineItem);
}

//*****************************************************************************
// プレイヤーとのの当たり判定処理
//*****************************************************************************
bool CAdrenalineItem::PlayerCollision(D3DXVECTOR3 pos)
{
	//イテレーターループ
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//イテレーターからアイテムのポインタの代入
		CAdrenalineItem* pAdrenalineItem = *itr;

		//アイテムポインタの解放
		if (pAdrenalineItem == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_AdrenalineItemList.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//判定
		bool bHit = CConvenience_Function::CircleCollision(pAdrenalineItem->GetParentPos(), RADIUS, pos, 0.0f);

		//上記の結果がtrue
		if (bHit)
		{
			//アイテムポインタの解放
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_AdrenalineItemList.erase(itr);

			//終了処理
			pAdrenalineItem->Uninit();
			delete pAdrenalineItem;
			pAdrenalineItem = nullptr;

			//処理を抜ける
			return true;
		}

		//イテレーターを進める
		itr++;
	}

	return false;
}
