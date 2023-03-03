//=============================================================================
//
// ライン処理 [line.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "line.h"
#include "manager.h"
#include <assert.h>

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CLine::CLine()
{
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CLine::~CLine()
{
}

//*****************************************************************************
//ライン初期化
//*****************************************************************************
HRESULT CLine::Init(void)
{
	//マネージャー情報の取得
	CManager *pManager = GetManager();

	//レンダー情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	pD3DDevice = pManager->GetDeviceManager();

	//頂点バッファの生成
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_LINE) * LINE_BUFF_MAX,//確保する頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffLine,
		NULL);

	return S_OK;
}

//*****************************************************************************
//ラインの終了処理
//*****************************************************************************
void CLine::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuffLine != NULL)
	{
		m_pVtxBuffLine->Release();
		m_pVtxBuffLine = NULL;
	}
}

//*****************************************************************************
//ラインの更新処理
//*****************************************************************************
void CLine::Update(void)
{

}

//*****************************************************************************
//ラインの描画処理
//*****************************************************************************
void CLine::Draw(void)
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//ライト無効
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//頂点バッファをデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuffLine, 0, sizeof(VERTEX_LINE));

	//頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_LINE);


	//ポリゴンの描画
	pD3DDevice->DrawPrimitive(D3DPT_LINELIST,		//プリミティブの種類
		0,					//描画する最初の頂点インデックス
		LINE_BUFF_MAX);		//プリミティブ（ポリゴン）数


	//ライト有効
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);



}

//-----------------------------------------------------------------------------
//セットライン処理
//-----------------------------------------------------------------------------
void CLine::SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXCOLOR col)
{
	VERTEX_LINE *pVtx = NULL;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);


	//基本情報の保存
	m_Pos = pos;
	m_SizeMax = vtxMax;
	m_SizeMin = vtxMin;
	m_Color = col;

	//頂点座標の設定//ローカル座標で
	pVtx[0].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	pVtx[1].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	pVtx[2].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	pVtx[3].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);
	pVtx[4].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	pVtx[5].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);
	pVtx[6].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);
	pVtx[7].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);
	pVtx[8].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);
	pVtx[9].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);
	pVtx[10].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);
	pVtx[11].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	pVtx[12].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	pVtx[13].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);
	pVtx[14].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);
	pVtx[15].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);

	//頂点カラーの設定
	for (int nCnt = 0; nCnt < LINE_BUFF_MAX; nCnt++)
	{
		pVtx[nCnt].col = m_Color;
	}
	
	//頂点バッファをアンロック
	m_pVtxBuffLine->Unlock();
}
