//=============================================================================
//
// ElasticityPolygon.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "elasticitypolygon.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CElasticityPolygon::CElasticityPolygon()
{
	m_nTextIndex = 0;
	m_pVtxBuff = nullptr;//頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_angleTop = 0.0f;
	m_angleUnder = 0.0f;
	m_lenTop = 0.0f;
	m_lenUnder = 0.0f;
	m_fHeightElasticity = 0.0f;
	m_fHeightMax = 0.0f;
	m_UVSizeMax = D3DXVECTOR2(1.0f, 1.0f);
	m_UVSizeMin = D3DXVECTOR2(0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CElasticityPolygon::~CElasticityPolygon()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CElasticityPolygon::Init()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	// 頂点バッファの生成
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMax.y);
	pVtx[1].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMax.y);
	pVtx[2].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMin.y);
	pVtx[3].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMin.y);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CElasticityPolygon::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CElasticityPolygon::Update()
{
	if (m_nFlashingCnt > 0)
	{
		m_nFlashingCnt--;
		if (!m_bFadeInOut)
		{
			m_Col.a += m_fFadeSpeed;
			if (m_Col.a > 1.0f)
			{
				m_Col.a = 1.0f;
				m_bFadeInOut = true;
			}
		}
		else if (m_bFadeInOut)
		{
			m_Col.a -= m_fFadeSpeed;
			if (m_Col.a < 0.2f)
			{
				m_Col.a = 0.2f;
				m_bFadeInOut = false;
			}
		}

		if (m_nFlashingCnt <= 0)
		{
			m_Col.a = 1.0f;
		}
	}

	SetPolygon();
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CElasticityPolygon::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//頂点バッファをデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, tex);

	//ポリゴンの描画
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数
}

void CElasticityPolygon::SetHeightElasticity(float fHeightElasticity)
{
	m_fHeightElasticity = fHeightElasticity;
	//対角線の長さを算出する
	m_lenTop = sqrtf(m_fWidth * m_fWidth + fHeightElasticity * fHeightElasticity);
	//対角線の角度を算出する
	m_angleTop = atan2f(m_fWidth, fHeightElasticity);
}

void CElasticityPolygon::SetDiagonalLine(float fwidth, float fheight)
{
	m_fWidth = fwidth * 0.5f;
	m_fHeight = fheight * 0.5f;

	//対角線の長さを算出する
	m_lenTop = sqrtf(m_fWidth * m_fWidth + fheight * fheight);
	//対角線の角度を算出する
	m_angleTop = atan2f(m_fWidth, fheight);

	//対角線の長さを算出する
	m_lenUnder = sqrtf(m_fWidth * m_fWidth);
	//対角線の角度を算出する
	m_angleUnder = atan2f(m_fWidth, 0.0f);

}

void CElasticityPolygon::SetPolygon()
{
	float rot = m_Rot.z;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + (0.0f - D3DX_PI + m_angleTop)) * m_lenTop, m_Pos.y + cosf(rot + (0.0f - D3DX_PI + m_angleTop)) * m_lenTop, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + (0.0f + D3DX_PI - m_angleTop)) * m_lenTop, m_Pos.y + cosf(rot + (0.0f + D3DX_PI - m_angleTop)) * m_lenTop, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + sinf(rot - m_angleUnder) * m_lenUnder, m_Pos.y + cosf(rot - m_angleUnder) * m_lenUnder, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + m_angleUnder) * m_lenUnder, m_Pos.y + cosf(rot + m_angleUnder) * m_lenUnder, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMin.y);
	pVtx[1].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMin.y);
	pVtx[2].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMax.y);
	pVtx[3].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMax.y);

	// 頂点カラーの設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// 当たり判定
//*****************************************************************************
bool CElasticityPolygon::Collision(D3DXVECTOR3 pos, float radius)
{
	//当たってない
	return false;

}