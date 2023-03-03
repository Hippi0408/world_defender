//=============================================================================
//
// ビルボード処理 [billcoard.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "billboard.h"
#include "manager.h"
#include "texture.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CBillcoard::CBillcoard()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBillcoard::~CBillcoard()
{
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CBillcoard::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライト無効
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	//ビューマトリックスの取得
	D3DXMATRIX mtxView;
	pD3DDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//カメラのベクトルと正反対にする
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	//位置の反映

	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//頂点バッファをデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_3D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, tex);

	//ポリゴンの描画
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数

				//ライト有効
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

void CBillcoard::SetPolygon()
{
	VERTEX_3D *pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定//ローカル座標で
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[1].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[2].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y + m_UVSize.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y + m_UVSize.y);

	// 頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
