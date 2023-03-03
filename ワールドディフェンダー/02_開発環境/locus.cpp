//=============================================================================
//
// 軌跡処理 [locus.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "locus.h" 
#include "manager.h"


const D3DXVECTOR3 CLocus::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CLocus::CLocus()
{
	//各ポインタの初期化
	m_pTopPosOld = nullptr;
	m_pDownPosOld = nullptr;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CLocus::~CLocus()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CLocus::Init()
{
	//軌跡構造の初期化
	ZeroMemory(&m_LocusData, sizeof(m_LocusData));
	m_nCarrent = 0;

	//親クラスの初期化
	if (FAILED(CMeshfield::Init()))
	{
		return -1;
	}

	
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CLocus::Uninit()
{
	//TopPosの解放
	if (m_pTopPosOld != nullptr)
	{
		delete[] m_pTopPosOld;
		m_pTopPosOld = nullptr;
	}

	//DownPosの解放
	if (m_pDownPosOld != nullptr)
	{
		delete[] m_pDownPosOld;
		m_pDownPosOld = nullptr;
	}

	//親クラスの終了処理
	CMeshfield::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CLocus::Update()
{
	
}

//*****************************************************************************
// 更新処理(オーバーロードPos更新あり)
//*****************************************************************************
void CLocus::Update(D3DXVECTOR3 TopPos, D3DXVECTOR3 DownPos)
{
	
	//現在の位置の更新
	m_LocusData.TopPos = TopPos;
	m_LocusData.DownPos = DownPos;

	//Oldの配列更新
	m_pTopPosOld[m_nCarrent] = TopPos;
	m_pDownPosOld[m_nCarrent] = DownPos;

	//各頂点の更新
	SetMeshTopPos();

	//保存先を変更
	m_nCarrent++;

	//保存先の番号が四角の数を超えたら
	if (m_nCarrent >= m_nCarrentMax)
	{
		m_nCarrent = 0;
	}

	
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CLocus::Draw()
{
	//マネージャー情報の取得
	CManager *pManager = GetManager();

	//レンダー情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	pD3DDevice = pManager->GetDeviceManager();

	//カリングのオフ
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//αブレンディングを加算合成に設定
	if (m_LocusData.bAddColor)
	{
		//αブレンディングを加算合成に設定
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//描画
	CMeshfield::Draw();

	if (m_LocusData.bAddColor)
	{
		//αブレンディングを元の戻す
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	//カリングのオン
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//*****************************************************************************
// 軌跡の設定
//*****************************************************************************
void CLocus::SetLocus(LocusStructure locusStructure)
{
	//上頂点
	m_LocusData.TopPos = locusStructure.TopPos;

	//下頂点
	m_LocusData.DownPos = locusStructure.DownPos;

	//始めのカラー
	m_LocusData.BeginningCol = locusStructure.BeginningCol;

	//終わりのカラー
	m_LocusData.EndCol = locusStructure.EndCol;

	//加算合成を行うかどうか
	m_LocusData.bAddColor = locusStructure.bAddColor;

	//何枚で描画するか
	m_LocusData.nPolygon = locusStructure.nPolygon;

	//テクスチャ番号
	m_LocusData.nTextureNum = locusStructure.nTextureNum;

	//保存する間隔
	m_LocusData.nSaveInterval = locusStructure.nSaveInterval;

	//OldPosの保存可能数
	m_nCarrentMax = LOWEST_VALUE + locusStructure.nSaveInterval * locusStructure.nPolygon;

	//上頂点のメモリーの確保
	m_pTopPosOld = new D3DXVECTOR3[m_nCarrentMax];
	//下頂点のメモリーの確保
	m_pDownPosOld = new D3DXVECTOR3[m_nCarrentMax];

	//情報の初期化
	for (int nCnt = 0; nCnt < m_nCarrentMax; nCnt++)
	{
		m_pTopPosOld[nCnt] = locusStructure.TopPos;
		m_pDownPosOld[nCnt] = locusStructure.DownPos;
	}

	MeshfieldStructure MeshData;
	ZeroMemory(&MeshData, sizeof(MeshData));
	MeshData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MeshData.rot = D3DXVECTOR3(D3DXToRadian(0), 0.0f, 0.0f);
	MeshData.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	MeshData.fRadiusX = 0.0f;
	MeshData.fRadiusZ = 0.0f;
	MeshData.nMeshX = 1;
	MeshData.nMeshZ = locusStructure.nPolygon;
	MeshData.nTextIndex = locusStructure.nTextureNum;

	//データの設定
	SetMeshfieldData(MeshData);

	//インデックスバッファの設定
	SetIdxBuff();

	//法線
	SetNormal();
}

//*****************************************************************************
// メッシュの各頂点の位置の設定
//*****************************************************************************
void CLocus::SetMeshTopPos()
{
	int nMeshX = GetMeshfieldData().nMeshX;
	int nMeshZ = GetMeshfieldData().nMeshZ;

	//カラーの設定
	D3DXCOLOR col = m_LocusData.BeginningCol;

	D3DXCOLOR addcol = m_LocusData.EndCol - m_LocusData.BeginningCol;

	float fRatio = (float)(nMeshZ + 1);

	addcol = addcol / fRatio;

	VERTEX_3D *pVtx = nullptr;		//頂点情報へのポインタ

	int nCarrent = m_nCarrent;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt <= nMeshZ; nCnt++)
	{
		

		D3DXVECTOR3 TopPosOld = m_pTopPosOld[nCarrent];
		D3DXVECTOR3 DownPosOld = m_pDownPosOld[nCarrent];

		//頂点座標の設定//ローカル座標で
		pVtx[0].pos = TopPosOld;
		pVtx[1].pos = DownPosOld;

		col += addcol;

		//頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(1.0f / nMeshX  * nCnt, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / nMeshX  * nCnt, 1.0f);


		nCarrent = m_nCarrent - (m_LocusData.nSaveInterval + m_LocusData.nSaveInterval * nCnt);
		if (nCarrent < 0)
		{
			nCarrent = m_nCarrentMax + nCarrent;
		}

		pVtx += 2; //データを2つ分進める

	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//*****************************************************************************
// 法線の設定
//*****************************************************************************
void CLocus::SetNormal()
{
	VERTEX_3D *pVtx = nullptr;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点数の取得
	int nTopMax = GetMeshfieldData().nTop;

	//各頂点の法線の設定
	for (int nTop = 0; nTop < nTopMax; nTop++)
	{
		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx++;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}


