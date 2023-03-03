//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "meshfield.h" 
#include "manager.h"
#include "texture.h"
#include <assert.h>

const D3DXVECTOR3 CMeshfield::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CMeshfield::CMeshfield()
{
	m_nMeshfieldNumMax = 0;
	m_nCntMeshfield = 0;
	m_nCoolTimeMeshfield = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CMeshfield::~CMeshfield()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CMeshfield::Init()
{
	m_pIdxBuff = nullptr;	//インデックスバッファへのポインタ
	ZeroMemory(&m_MeshfieldData,sizeof(m_MeshfieldData));

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CMeshfield::Uninit()
{
	C3DPolygon::Uninit();

	if (m_MeshfieldData.pNormalPolygon != nullptr)
	{
		delete[] m_MeshfieldData.pNormalPolygon;
		m_MeshfieldData.pNormalPolygon = nullptr;
	}

	if (m_MeshfieldData.pNormalTop != nullptr)
	{
		delete[] m_MeshfieldData.pNormalTop;
		m_MeshfieldData.pNormalTop = nullptr;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CMeshfield::Update()
{
	m_nCoolTimeMeshfield++;

	if (m_nCoolTimeMeshfield < 0)
	{
		return;
	}

	m_nCoolTimeMeshfield = 0;

	m_nCntMeshfield++;

	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntZ = 0; nCntZ <= m_MeshfieldData.nMeshZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_MeshfieldData.nMeshX; nCntX++)
		{
			float fY = sinf(D3DXToRadian(0.5f) * (nCntX + m_nCntMeshfield)) * 10.0f;

			//頂点座標の設定//ローカル座標で
			pVtx[0].pos = D3DXVECTOR3(m_MeshfieldData.fRadiusX * nCntX, fY, -m_MeshfieldData.fRadiusZ * nCntZ);

			pVtx++; //データを1つ分進める
		}
	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//法線設定
	SetNormal();

	C3DPolygon::Update();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CMeshfield::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans,pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//頂点バッファをデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pD3DDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_3D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, tex);

	//ポリゴンの描画
	pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,														//描画する最初の頂点インデックス
		0,														//インデックスの最小値
		m_MeshfieldData.nTop,									//頂点の数
		0,														//インデックスの最初の数
		m_MeshfieldData.nPolygon);								//プリミティブ（ポリゴン）数


}

//*****************************************************************************
// セットメッシュフィールド
//*****************************************************************************
void CMeshfield::SetMeshfield(MeshfieldStructure meshfieldStructure)
{
	//データの設定
	SetMeshfieldData(meshfieldStructure);

	//各頂点の位置の設定
	SetMeshTopPos();

	//インデックスバッファの設定
	SetIdxBuff();
	

	//法線設定
	SetNormal();
}

//セットポリゴン（中身無し）
void CMeshfield::SetPolygon()
{
	
}

//各頂点の位置の設定
void CMeshfield::SetMeshTopPos()
{
	int nMeshX = m_MeshfieldData.nMeshX;
	int nMeshZ = m_MeshfieldData.nMeshZ;
	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ

								// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
	{
		float fY = 0.0f;
		for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
		{

			//float fY = sinf(D3DXToRadian(0.5f) * (nCntX + m_nCntMeshfield)) * 10.0f;
			//float fY = rand() % 50;

			//頂点座標の設定//ローカル座標で
			pVtx[0].pos = D3DXVECTOR3(m_MeshfieldData.fRadiusX * nCntX, fY, -m_MeshfieldData.fRadiusZ * nCntZ);

			//各頂点の法線の設定（ベクトルの大きさは１にする必要がある）
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = m_Color;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f / nMeshX  * nCntX, 1.0f / nMeshZ * nCntZ);

			pVtx++; //データを1つ分進める
		}
	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//法線設定
void CMeshfield::SetNormal()
{
	VERTEX_3D *pVtx = nullptr;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nPolygon = 0; nPolygon < m_MeshfieldData.nPolygon; nPolygon++)
	{
		
			D3DXVECTOR3 vec1, vec2,vecResult;
			D3DXVECTOR3 top;

			if (pIdx[0] == pIdx[1]
				|| pIdx[0] == pIdx[2]
				|| pIdx[1] == pIdx[2])
			{
				pIdx++;
				continue;
			}

			vec1 = pVtx[pIdx[1]].pos - pVtx[pIdx[0]].pos;
			vec2 = pVtx[pIdx[2]].pos - pVtx[pIdx[0]].pos;

			//外積
			if (nPolygon % 2 == 1)
			{
				D3DXVec3Cross(&vecResult, &vec2, &vec1);
			}
			else
			{
				D3DXVec3Cross(&vecResult, &vec1, &vec2);
			}
			//正規化
			D3DXVec3Normalize(&vecResult,&vecResult);
			//頂点の法線ベクトルの保存
			top = vecResult;
			//面の法線ベクトルの保存
			m_MeshfieldData.pNormalPolygon[nPolygon] = vecResult;
			//各頂点の法線ベクトルの計算
			for (int nTop = 0;nTop < 3;nTop++)
			{
				D3DXVECTOR3 vec = m_MeshfieldData.pNormalTop[pIdx[0 + nTop]];
				vec += top;
				//正規化
				D3DXVec3Normalize(&vec, &vec);
				m_MeshfieldData.pNormalTop[pIdx[0 + nTop]] = vec;
			}
			
			pIdx++;
	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	for (int nTop = 0; nTop < m_MeshfieldData.nTop; nTop++)
	{
		//各頂点の法線の設定
		pVtx[0].nor = m_MeshfieldData.pNormalTop[nTop];
		pVtx++;
	}


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//データの設定
void CMeshfield::SetMeshfieldData(MeshfieldStructure meshfieldStructure)
{
	if (meshfieldStructure.nMeshX <= 0
		|| meshfieldStructure.nMeshZ <= 0)
	{
		assert(false);
	}

	SetPos(meshfieldStructure.pos);
	SetRot(meshfieldStructure.rot);
	SetColor(meshfieldStructure.col);
	m_MeshfieldData.fRadiusX = meshfieldStructure.fRadiusX;
	m_MeshfieldData.fRadiusZ = meshfieldStructure.fRadiusZ;

	int nMeshX = meshfieldStructure.nMeshX;
	int nMeshZ = meshfieldStructure.nMeshZ;

	m_MeshfieldData.nMeshX = nMeshX;
	m_MeshfieldData.nMeshZ = nMeshZ;

	m_nMeshfieldNumMax = nMeshX * nMeshZ;

	m_nTextIndex = meshfieldStructure.nTextIndex;

	m_MeshfieldData.nTop = (nMeshX + 1) * (nMeshZ + 1);
	m_MeshfieldData.nIdx = (nMeshX + 1) * 2 * nMeshZ + (nMeshZ - 1) * 2;
	m_MeshfieldData.nPolygon = nMeshX * nMeshZ * 2 + (nMeshZ - 1) * 4;

	m_MeshfieldData.pNormalTop = new D3DXVECTOR3[m_MeshfieldData.nTop];

	ZeroMemory(m_MeshfieldData.pNormalTop, sizeof(D3DXVECTOR3) * m_MeshfieldData.nTop);
	m_MeshfieldData.pNormalPolygon = new D3DXVECTOR3[m_MeshfieldData.nPolygon];

	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	// 頂点バッファの生成
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshfieldData.nTop,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点のインデックスバッファの生成
	pD3DDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshfieldData.nIdx,//確保する頂点の数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

}

//インデックスバッファの設定
void CMeshfield::SetIdxBuff()
{
	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntZ = 0; nCntZ < m_MeshfieldData.nMeshZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_MeshfieldData.nMeshX; nCntX++)
		{
			//インデックスの設定
			pIdx[0] = (WORD)(nCntX + (m_MeshfieldData.nMeshX + 1) * (nCntZ + 1));
			pIdx[1] = (WORD)(nCntX + (m_MeshfieldData.nMeshX + 1) * nCntZ);
			pIdx += 2;//インデックスのデータを２すすめる
		}

		if (nCntZ < m_MeshfieldData.nMeshZ - 1)
		{
			//インデックスの設定
			pIdx[0] = pIdx[-1];//ひとつ前のpIdxの中身を入れる（ひとつ前は必ずある）
			pIdx[1] = (WORD)((m_MeshfieldData.nMeshX + 1) * (nCntZ + 2));
			pIdx += 2;//インデックスのデータを２すすめる
		}
	}
	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

}

//引く数のPosがどのマスに居るかを返す
int CMeshfield::CheckPosLocation(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx = nullptr;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nLocation = 0;
	int retval = -1;
	D3DXVECTOR3 vec1, vec2,top0,top1,top2, MeshfieldPos;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2;

	//メッシュの位置
	MeshfieldPos = GetPos();

	for (int nPolygon = 0; nPolygon < m_MeshfieldData.nPolygon; nPolygon++)
	{
		if (pIdx[0] == pIdx[1]
			|| pIdx[0] == pIdx[2]
			|| pIdx[1] == pIdx[2])
		{
			pIdx++;
			continue;
		}

		top0 = pVtx[pIdx[0]].pos + MeshfieldPos;

		{
			D3DXVECTOR3 CheckDistance = top0 - pos;

			if (D3DXVec3LengthSq(&CheckDistance) > 1500.0f * 1500.0f)
			{
				pIdx++;

				//マスの数はポリゴン2枚で１ずつ進む
				if ((nPolygon + 1) % 2 == 0)
				{
					nLocation++;
				}

				continue;
			}
		}

		top1 = pVtx[pIdx[1]].pos + MeshfieldPos;
		top2 = pVtx[pIdx[2]].pos + MeshfieldPos;

		vec1 = top1 - top0;
		vec2 = pos - top0;

		fInnerProduct0 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = top2 - top1;
		vec2 = pos - top1;

		fInnerProduct1 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = top0 - top2;
		vec2 = pos - top2;

		fInnerProduct2 = vec1.x * vec2.z - vec1.z * vec2.x;

		if (
			(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f) ||
			(fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f)
			)
		{
			retval = nLocation;
			break;
		}

		pIdx++;

		//マスの数はポリゴン2枚で１ずつ進む
		if ((nPolygon + 1) % 2 == 0)
		{
			nLocation++;
		}
	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//エラー時は -1
	return retval;
}

D3DXVECTOR3 CMeshfield::Collision(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx = nullptr;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nPolygon = 0; nPolygon < m_MeshfieldData.nPolygon; nPolygon++)
	{
		if (pIdx[0] == pIdx[1]
			|| pIdx[0] == pIdx[2]
			|| pIdx[1] == pIdx[2])
		{
			pIdx++;
			continue;
		}

		D3DXVECTOR3 vec1, vec2;
		float fInnerProduct0 , fInnerProduct1, fInnerProduct2;

		vec1 = pVtx[pIdx[1]].pos - pVtx[pIdx[0]].pos;
		vec2 = pos - pVtx[pIdx[0]].pos;

		fInnerProduct0 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = pVtx[pIdx[2]].pos - pVtx[pIdx[1]].pos;
		vec2 = pos - pVtx[pIdx[1]].pos;

		fInnerProduct1 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = pVtx[pIdx[0]].pos - pVtx[pIdx[2]].pos;
		vec2 = pos - pVtx[pIdx[2]].pos;

		fInnerProduct2 = vec1.x * vec2.z - vec1.z * vec2.x;

		if (
			(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f)
			|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f)
			)
		{
			D3DXVECTOR3 P1 = pVtx[pIdx[0]].pos;
			D3DXVECTOR3 vec = m_MeshfieldData.pNormalPolygon[nPolygon];

			float fPolygonY = P1.y - ((pos.x - P1.x) * vec.x + (pos.z - P1.z) * vec.z) / vec.y;

			//インデックスバッファをアンロック
			m_pIdxBuff->Unlock();
			//頂点バッファをアンロック
			m_pVtxBuff->Unlock();

			return D3DXVECTOR3(pos.x, fPolygonY, pos.z);
		}

		pIdx++;

	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return D3DXVECTOR3(0.0f,0.0f,0.0f);
}
