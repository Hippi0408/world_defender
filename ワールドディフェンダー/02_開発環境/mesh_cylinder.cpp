//=============================================================================
//
// 軌跡処理 [mesh_cylinder.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "mesh_cylinder.h" 
#include "manager.h"

const D3DXVECTOR3 CMesh_Cylinder::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CMesh_Cylinder::CMesh_Cylinder()
{
	
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CMesh_Cylinder::~CMesh_Cylinder()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CMesh_Cylinder::Init()
{
	m_MeshRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//構造の初期化
	ZeroMemory(&m_Mesh_Cylinder_Structure, sizeof(m_Mesh_Cylinder_Structure));
	
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
void CMesh_Cylinder::Uninit()
{
	//親クラスの終了処理
	CMeshfield::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CMesh_Cylinder::Update()
{

}

//*****************************************************************************
// 更新処理(オーバーロードPos更新あり)
//*****************************************************************************
void CMesh_Cylinder::Update(D3DXVECTOR3 Pos)
{
	//現在の位置の更新
	SetPos(Pos);

	//各頂点の更新
	SetMeshTopPos();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CMesh_Cylinder::Draw()
{
	//マネージャー情報の取得
	CManager *pManager = GetManager();

	//レンダー情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	pD3DDevice = pManager->GetDeviceManager();

	//カリングのオフ
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//描画
	CMeshfield::Draw();

	//カリングのオン
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//*****************************************************************************
// 軌跡の設定
//*****************************************************************************
void CMesh_Cylinder::SetMesh_Cylinder(Mesh_Cylinder_Structure mesh_cylinder_structure)
{
	//RotMove
	m_Mesh_Cylinder_Structure.fRotMove = mesh_cylinder_structure.fRotMove;

	//半径
	m_Mesh_Cylinder_Structure.fRadius = mesh_cylinder_structure.fRadius;

	//Yのサイズ最大
	m_Mesh_Cylinder_Structure.fSizeYTop = mesh_cylinder_structure.fSizeYTop;

	//親頂点
	m_Mesh_Cylinder_Structure.ParentPos = mesh_cylinder_structure.ParentPos;

	//カラー減衰率
	m_Mesh_Cylinder_Structure.nAttenuationFrame = mesh_cylinder_structure.nAttenuationFrame;

	//カラー
	m_Mesh_Cylinder_Structure.ColorMax = mesh_cylinder_structure.ColorMax;
	m_Mesh_Cylinder_Structure.ColorLowest = mesh_cylinder_structure.ColorLowest;

	m_Mesh_Cylinder_Structure.bFade = mesh_cylinder_structure.bFade;
	if (m_Mesh_Cylinder_Structure.bFade)
	{
		m_AddColor = (m_Mesh_Cylinder_Structure.ColorLowest - m_Mesh_Cylinder_Structure.ColorMax) / (float)m_Mesh_Cylinder_Structure.nAttenuationFrame;
	}
	
	m_Color = m_Mesh_Cylinder_Structure.ColorMax;

	//何枚で描画するか
	m_Mesh_Cylinder_Structure.nPolygonX = mesh_cylinder_structure.nPolygonY;
	m_Mesh_Cylinder_Structure.nPolygonY = mesh_cylinder_structure.nPolygonX;

	//テクスチャ番号
	m_Mesh_Cylinder_Structure.nTextureNum = mesh_cylinder_structure.nTextureNum;

	
	


	MeshfieldStructure MeshData;
	ZeroMemory(&MeshData, sizeof(MeshData));
	MeshData.pos = mesh_cylinder_structure.ParentPos;
	MeshData.rot = D3DXVECTOR3(D3DXToRadian(0), 0.0f, 0.0f);
	MeshData.col = mesh_cylinder_structure.ColorMax;
	MeshData.fRadiusX = 0.0f;
	MeshData.fRadiusZ = 0.0f;
	MeshData.nMeshX = m_Mesh_Cylinder_Structure.nPolygonX;
	MeshData.nMeshZ = m_Mesh_Cylinder_Structure.nPolygonY;
	MeshData.nTextIndex = m_Mesh_Cylinder_Structure.nTextureNum;


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
void CMesh_Cylinder::SetMeshTopPos()
{
	//Xの頂点数
	int nTopX = m_Mesh_Cylinder_Structure.nPolygonY;

	//角度変数宣言
	
	float fRot, fRotAdd;
	fRot = m_MeshRot.z;
	m_MeshRot.z += m_Mesh_Cylinder_Structure.fRotMove;
	fRotAdd = D3DXToRadian(360) / nTopX;
	
	//SizeY変数宣言
	float fSizeY, fSizeYAdd;
	//現在の高さ
	fSizeY = m_Mesh_Cylinder_Structure.ParentPos.y;
	//加算していく値
	fSizeYAdd = (m_Mesh_Cylinder_Structure.fSizeYTop) / (m_Mesh_Cylinder_Structure.nPolygonY);


	// カラーの設定
	if (m_Mesh_Cylinder_Structure.bFade)
	{
		ChangeColor();
	}

	//各メッシュの数
	int nMeshZ = GetMeshfieldData().nMeshZ;
	int nMeshX = GetMeshfieldData().nMeshX;

	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
	{
		//現在の高さ
		fSizeY = m_Mesh_Cylinder_Structure.ParentPos.y;
		//加算していく値
		fSizeYAdd = (m_Mesh_Cylinder_Structure.fSizeYTop) / (m_Mesh_Cylinder_Structure.nPolygonX);


		D3DXVECTOR3 pos;

		pos.x = m_Mesh_Cylinder_Structure.fRadius * cosf(fRot);
		pos.z = m_Mesh_Cylinder_Structure.fRadius * sinf(fRot);

		
		
		for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
		{

			pos.y = fSizeY;

			//頂点座標の設定//ローカル座標で
			pVtx[0].pos = pos;

			//各頂点の法線の設定（ベクトルの大きさは１にする必要がある）
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = m_Color;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f / nMeshZ  * nCntZ, -1.0f / nMeshX * nCntX);

			pVtx++; //データを1つ分進める


			fSizeY += fSizeYAdd;

			

		}

		fRot += fRotAdd;

	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// 法線の設定
//*****************************************************************************
void CMesh_Cylinder::SetNormal()
{
	VERTEX_3D *pVtx = nullptr;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点数の取得
	int nTopMax = GetMeshfieldData().nTop;

	//各頂点の法線の設定
	for (int nTop = 0; nTop < nTopMax; nTop++)
	{
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx++;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// カラーの設定
//*****************************************************************************
void CMesh_Cylinder::ChangeColor()
{
	m_nAttenuationFrameCnt++;
	m_Color += m_AddColor;

	if (m_Color.r >= 1.0f)
	{
		m_Color.r = 1.0f;
	}
	else if (m_Color.r <= 0.0f)
	{
		m_Color.r = 0.0f;
	}

	if (m_Color.g >= 1.0f)
	{
		m_Color.g = 1.0f;
	}
	else if (m_Color.g <= 0.0f)
	{
		m_Color.g = 0.0f;
	}

	if (m_Color.b >= 1.0f)
	{
		m_Color.b = 1.0f;
	}
	else if (m_Color.b <= 0.0f)
	{
		m_Color.b = 0.0f;
	}

	if (m_Color.a >= 1.0f)
	{
		m_Color.a = 1.0f;
	}
	else if (m_Color.a <= 0.0f)
	{
		m_Color.a = 0.0f;
	}

	if (m_nAttenuationFrameCnt >= m_Mesh_Cylinder_Structure.nAttenuationFrame)
	{
		m_AddColor = -1.0f * m_AddColor;
		m_nAttenuationFrameCnt = 0;
	}

}


