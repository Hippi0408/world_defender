//=============================================================================
//
// 3dobject.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "3dobject.h"
#include "manager.h"
#include "object_type_list.h"
#include <assert.h>

#ifdef _DEBUG
#include "line.h"
#endif // _DEBUG

C3DObject::ModelPattern C3DObject::m_ModelPattern[MODEL_PATTERN_MAX] = {};
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
C3DObject::C3DObject()
{
	ZeroMemory(&m_Model, sizeof(m_Model));
	m_fSize = 1.0f;
	m_Object_Type_List = OBJ_OTHER;
	m_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_ColorChange = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ColorMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ColorMin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fAttenuationColor = 0.0f;
	m_bColor = false;
	m_pDefaultColor = nullptr;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
C3DObject::~C3DObject()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT C3DObject::Init()
{
	ZeroMemory(&m_Model, sizeof(m_Model));
	m_fSize = 1.0f;

#ifdef _DEBUG
	m_pLine = nullptr;
#endif // _DEBUG

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void C3DObject::Uninit()
{
	if (m_Model.pTopPos != nullptr)
	{
		delete[] m_Model.pTopPos;
		m_Model.pTopPos = nullptr;
	}

	if(m_Model.pNormalPolygon != nullptr)
	{
		delete[] m_Model.pNormalPolygon;
		m_Model.pNormalPolygon = nullptr;
	}

	if (m_pDefaultColor != nullptr)
	{
		delete[] m_pDefaultColor;
		m_pDefaultColor = nullptr;
	}

#ifdef _DEBUG
	if (m_pLine != nullptr)
	{
		delete m_pLine;
		m_pLine = nullptr;
	}
#endif // _DEBUG

}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void C3DObject::Update()
{
	m_Color += m_ColorChange * m_fAttenuationColor;

	//r
	if (m_Color.r >= m_ColorMax.r)
	{
		m_Color.r = m_ColorMax.r;
		m_ColorChange.r *= -1.0f;
	}
	else if (m_Color.r <= m_ColorMin.r)
	{
		m_Color.r = m_ColorMin.r;
		m_ColorChange.r *= -1.0f;
	}

	//g
	if (m_Color.g >= m_ColorMax.g)
	{
		m_Color.g = m_ColorMax.g;
		m_ColorChange.g *= -1.0f;
	}
	else if (m_Color.g <= m_ColorMin.g)
	{
		m_Color.g = m_ColorMin.g;
		m_ColorChange.g *= -1.0f;
	}

	//b
	if (m_Color.b >= m_ColorMax.b)
	{
		m_Color.b = m_ColorMax.b;
		m_ColorChange.b *= -1.0f;
	}
	else if (m_Color.b <= m_ColorMin.b)
	{
		m_Color.b = m_ColorMin.b;
		m_ColorChange.b *= -1.0f;
	}

	//a
	if (m_Color.a >= m_ColorMax.a)
	{
		m_Color.a = m_ColorMax.a;
		m_ColorChange.a *= -1.0f;
	}
	else if (m_Color.a <= m_ColorMin.a)
	{
		m_Color.a = m_ColorMin.a;
		m_ColorChange.a *= -1.0f;
	}


	
	
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void C3DObject::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DMATERIAL9 matDef;			//現在のマテリアル保存
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	bool bShadow = false;
	//影をつける
	if (m_LightVec != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		bShadow = true;
		D3DXMATRIX mtxShadow;
		D3DXPLANE pIaneField;
		D3DXVECTOR4 vecLight;
		D3DXVECTOR3 pos, normal;

		pos = m_ShadowPos;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		vecLight = D3DXVECTOR4(m_LightVec.x, m_LightVec.y, m_LightVec.z,0.0f) * -1.0f;

		D3DXPlaneFromPointNormal(&pIaneField, &pos, &normal);

		D3DXMatrixShadow(&mtxShadow, &vecLight, &pIaneField);

		D3DXMatrixMultiply(&mtxShadow, &m_Model.mtxWorld, &mtxShadow);

		//ワールドマトリックスの設定
		pD3DDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//現在のマテリアルを保存
		pD3DDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

		//テクスチャの設定
		pD3DDevice->SetTexture(0, NULL);

		for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
		{

			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			//マテリアルの設定
			pD3DDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pD3DDevice->SetTexture(0, m_ModelPattern[m_Model.nPattn].pTexture[nCntMat]);

			//モデルパーツの描画
			m_ModelPattern[m_Model.nPattn].pMeshModel->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pD3DDevice->SetMaterial(&matDef);

	}

	//ワールドマトリックスの設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_Model.mtxWorld);

	//現在のマテリアルを保存
	pD3DDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

	//テクスチャの設定
	pD3DDevice->SetTexture(0, NULL);

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
	{
		
		if (m_bColor)
		{
			pMat[nCntMat].MatD3D.Diffuse = m_Color;
		}
		else
		{
			pMat[nCntMat].MatD3D.Diffuse = m_pDefaultColor[nCntMat];
		}

		//マテリアルの設定
		pD3DDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pD3DDevice->SetTexture(0, m_ModelPattern[m_Model.nPattn].pTexture[nCntMat]);

		//モデルパーツの描画
		m_ModelPattern[m_Model.nPattn].pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pD3DDevice->SetMaterial(&matDef);


	if (m_Model.pNormalPolygon == nullptr)
	{
		//法線の設定
		SetNormal();
	}
	else
	{
		//法線の更新
		UpdateNormal();
	}
	
#ifdef _DEBUG

	//ラインのワールドマトリックスの設定
	m_pLine->SetMtxWorld(m_Model.mtxWorld);

	//ラインの描画
	m_pLine->Draw();

#endif // _DEBUG]

}

//*****************************************************************************
// ３Dオブジェクトの配置
//*****************************************************************************
void C3DObject::Set3DObject(int nPattn, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nNumVix;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	m_Model.pos = pos;
	m_Model.rot = rot;
	m_Model.nPattn = nPattn;

	//デフォルトカラーの保存
	m_pDefaultColor = new D3DCOLORVALUE[m_ModelPattern[m_Model.nPattn].nNumMatModel];

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;				
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
	{
		m_pDefaultColor[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
	}


	//モデルのサイズの比較用初期値
	m_Model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_Model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//頂点数の取得
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();

	//頂点フォーマット取得
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_Model.nPattn].pMeshModel->GetFVF());

	//頂点ロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//すべての頂点POSの取得
	for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (m_Model.vtxMax.x < vtx.x)
		{//X
			m_Model.vtxMax.x = vtx.x;
		}
		if (m_Model.vtxMin.x > vtx.x)
		{
			m_Model.vtxMin.x = vtx.x;
		}

		if (m_Model.vtxMax.y < vtx.y)
		{//Y
			m_Model.vtxMax.y = vtx.y;
		}
		if (m_Model.vtxMin.y > vtx.y)
		{
			m_Model.vtxMin.y = vtx.y;
		}

		if (m_Model.vtxMax.z < vtx.z)
		{//Z
			m_Model.vtxMax.z = vtx.z;
		}
		if (m_Model.vtxMin.z > vtx.z)
		{
			m_Model.vtxMin.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点アンロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockVertexBuffer();

#ifdef _DEBUG

	//ラインを生成
	m_pLine = new CLine;

	//初期化
	if (m_pLine->Init())
	{
		assert(false);
	}

	//ラインの情報の設定
	m_pLine->SetLine(m_Model.posParent, m_Model.vtxMax, m_Model.vtxMin, D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));

#endif // _DEBUG]

}

//*****************************************************************************
// マトリックスの計算
//*****************************************************************************
void C3DObject::CalculationMatrix()
{
	
	D3DXMATRIX mtxRoot;					//元の親のワールドマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//計算用のマトリックス
	D3DXMATRIX mtxRotParent, mtxRotChild;

	//ワールドマトリックスの初期化（元の親）
	D3DXMatrixIdentity(&mtxRoot);

	//サイズの変更
	D3DXMatrixScaling(&mtxScaling, m_fSize, m_fSize, m_fSize);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxScaling);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Model.rotParent.y, m_Model.rotParent.x, m_Model.rotParent.z);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//ワールドマトリックス向きだけの保存
	mtxRotParent = mtxRot;

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, m_Model.posParent.x, m_Model.posParent.y, m_Model.posParent.z);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxTrans);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_Model.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Model.rot.y, m_Model.rot.x, m_Model.rot.z);
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxRot);

	//ワールドマトリックス向きだけの保存
	mtxRotChild = mtxRot;

	D3DXVECTOR3 pos = m_Model.pos + m_Model.posMove;
	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxTrans);

	//モデルのマトリックス　＊　親のワールドマトリックス
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxRoot);

	//モデルのマトリックス　＊　親のワールドマトリックス(向きだけ)
	D3DXMatrixMultiply(&m_Model.mtxWorldRot, &mtxRotChild, &mtxRotParent);


	m_Model.vtxMax2 = m_Model.pos + m_Model.vtxMax;
	m_Model.vtxMin2 = m_Model.pos + m_Model.vtxMin;

	//モデルの最大最小をワールドマトリックス変換し別保存
	D3DXVec3TransformCoord(&m_Model.vtxMax2, &m_Model.vtxMax, &m_Model.mtxWorld);
	D3DXVec3TransformCoord(&m_Model.vtxMin2, &m_Model.vtxMin, &m_Model.mtxWorld);

}

//*****************************************************************************
//法線設定
//*****************************************************************************
void C3DObject::SetNormal()
{
	int nNumVix;		//頂点数
	int nNumIndex;		//インデックス数
	int nNumPolygon;	//ポリゴン数

	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ
	BYTE *pIndexBuff;	//インデックスバッファへのポインタ

	//頂点数の取得
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();
	//ポリゴン数の取得
	nNumPolygon = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumFaces();
	//インデクス数の取得
	nNumIndex = nNumPolygon + 2;
	
	//頂点の設定
	m_Model.pTopPos = new D3DXVECTOR3[nNumVix];

	//法線の設定
	m_Model.pNormalPolygon = new D3DXVECTOR3[nNumPolygon];

	//頂点フォーマット取得
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_Model.nPattn].pMeshModel->GetFVF());

	//頂点ロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);


	//すべての頂点POSの取得
	for (int nCnt = 0; nCnt < nNumVix; nCnt++)
	{
		//頂点座標の取得
		m_Model.pTopPos[nCnt] = *(D3DXVECTOR3*)pVtxBuff;
		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点アンロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockVertexBuffer();

	//インデックスバッファのロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	int nIndex1, nIndex2, nIndex3;
	//法線の計算
	for (int nCnt = 0; nCnt < nNumPolygon; nCnt++)
	{
		
		nIndex1 = *(WORD*)pIndexBuff;
		nIndex2 = *((WORD*)pIndexBuff + 1);
		nIndex3 = *((WORD*)pIndexBuff + 2);

		//頂点座標の代入
		D3DXVECTOR3 vtx1 = m_Model.pTopPos[nIndex1];
		D3DXVECTOR3 vtx2 = m_Model.pTopPos[nIndex2];
		D3DXVECTOR3 vtx3 = m_Model.pTopPos[nIndex3];

		D3DXVECTOR3 vec1, vec2, vecResult;

		vec1 = vtx2 - vtx1;
		vec2 = vtx3 - vtx1;

		//外積
		/*if (nCnt % 2 == 1)
		{
			D3DXVec3Cross(&vecResult, &vec2, &vec1);
		}
		else
		{*/
			D3DXVec3Cross(&vecResult, &vec1, &vec2);
		//}
		//正規化
		D3DXVec3Normalize(&vecResult, &vecResult);

		//面の法線ベクトルの保存
		m_Model.pNormalPolygon[nCnt] = vecResult;

		//データを進める
		pIndexBuff+=sizeof(WORD)*3;
	}

	//インデックスバッファのアンロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockIndexBuffer();
}

//*****************************************************************************
//法線の向きの更新
//*****************************************************************************
void C3DObject::UpdateNormal()
{
	int nNumPolygon;	//ポリゴン数
	//ポリゴン数の取得
	nNumPolygon = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumFaces();

	BYTE *pIndexBuff;	//インデックスバッファへのポインタ

	//インデックスバッファのロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	int nIndex1, nIndex2, nIndex3;
	//法線の計算
	for (int nCnt = 0; nCnt < nNumPolygon; nCnt++)
	{

		nIndex1 = *(WORD*)pIndexBuff;
		nIndex2 = *((WORD*)pIndexBuff + 1);
		nIndex3 = *((WORD*)pIndexBuff + 2);

		//頂点座標の代入
		D3DXVECTOR3 vtx1 = m_Model.pTopPos[nIndex1];
		D3DXVECTOR3 vtx2 = m_Model.pTopPos[nIndex2];
		D3DXVECTOR3 vtx3 = m_Model.pTopPos[nIndex3];

		D3DXVECTOR3 vec1, vec2, vecResult;

		vec1 = vtx2 - vtx1;
		vec2 = vtx3 - vtx1;

		//外積
		/*if (nCnt % 2 == 1)
		{
		D3DXVec3Cross(&vecResult, &vec2, &vec1);
		}
		else
		{*/
		D3DXVec3Cross(&vecResult, &vec1, &vec2);
		//}
		//正規化
		D3DXVec3Normalize(&vecResult, &vecResult);

		//面の法線ベクトルの保存
		m_Model.pNormalPolygon[nCnt] = vecResult;

		//データを進める
		pIndexBuff += sizeof(WORD) * 3;
	}

	//インデックスバッファのアンロック
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockIndexBuffer();
}

//*****************************************************************************
//当たり判定
//*****************************************************************************
D3DXVECTOR3 C3DObject::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

bool C3DObject::NormalCollision(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vtx[8];

	vtx[0] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMax2.y, m_Model.vtxMin2.z);
	vtx[1] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMax2.y, m_Model.vtxMin2.z);
	vtx[2] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMin2.y, m_Model.vtxMin2.z);
	vtx[3] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMin2.y, m_Model.vtxMin2.z);

	vtx[4] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMax2.y, m_Model.vtxMax2.z);
	vtx[5] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMax2.y, m_Model.vtxMax2.z);
	vtx[6] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMin2.y, m_Model.vtxMax2.z);
	//vtx[7] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMin2.y, m_Model.vtxMax2.z);

	//四角形の内にいるかどうか
	if(!SquareInOut(pos, vtx[0], vtx[1], vtx[2], vtx[3]))
	{
		return false;
	}
	if (!SquareInOut(pos, vtx[1], vtx[5], vtx[6], vtx[2]))
	{
		return false;
	}
	if (!SquareInOut(pos, vtx[4], vtx[5], vtx[1], vtx[0]))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
//三角形の内にいるかどうか
//*****************************************************************************
bool C3DObject::TriangleInOut(D3DXVECTOR3 pos, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0[3], fInnerProduct1[3], fInnerProduct2[3];

	vec1 = vtx1 - vtx0;
	vec2 = pos - vtx0;

	fInnerProduct0[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct0[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct0[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx2 - vtx1;
	vec2 = pos - vtx1;

	fInnerProduct1[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct1[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct1[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx0 - vtx2;
	vec2 = pos - vtx2;

	fInnerProduct2[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct2[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct2[2] = vec1.z * vec2.y - vec1.y * vec2.z;


	if (
		(fInnerProduct0[0] > 0.0f && fInnerProduct1[0] > 0.0f && fInnerProduct2[0] > 0.0f)
		|| (fInnerProduct0[0] < 0.0f && fInnerProduct1[0] < 0.0f && fInnerProduct2[0] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[1] > 0.0f && fInnerProduct1[1] > 0.0f && fInnerProduct2[1] > 0.0f)
		|| (fInnerProduct0[1] < 0.0f && fInnerProduct1[1] < 0.0f && fInnerProduct2[1] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[2] > 0.0f && fInnerProduct1[2] > 0.0f && fInnerProduct2[2] > 0.0f)
		|| (fInnerProduct0[2] < 0.0f && fInnerProduct1[2] < 0.0f && fInnerProduct2[2] < 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
//四角形の内にいるかどうか
//*****************************************************************************
bool C3DObject::SquareInOut(D3DXVECTOR3 pos, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0[3], fInnerProduct1[3], fInnerProduct2[3] , fInnerProduct3[3];

	vec1 = vtx1 - vtx0;
	vec2 = pos - vtx0;

	fInnerProduct0[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct0[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct0[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx2 - vtx1;
	vec2 = pos - vtx1;

	fInnerProduct1[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct1[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct1[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx3 - vtx2;
	vec2 = pos - vtx2;

	fInnerProduct2[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct2[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct2[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx0 - vtx3;
	vec2 = pos - vtx3;

	fInnerProduct3[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct3[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct3[2] = vec1.z * vec2.y - vec1.y * vec2.z;


	if (
		(fInnerProduct0[0] > 0.0f && fInnerProduct1[0] > 0.0f && fInnerProduct2[0] > 0.0f && fInnerProduct3[0] > 0.0f)
		|| (fInnerProduct0[0] < 0.0f && fInnerProduct1[0] < 0.0f && fInnerProduct2[0] < 0.0f && fInnerProduct3[0] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[1] > 0.0f && fInnerProduct1[1] > 0.0f && fInnerProduct2[1] > 0.0f && fInnerProduct3[1] > 0.0f)
		|| (fInnerProduct0[1] < 0.0f && fInnerProduct1[1] < 0.0f && fInnerProduct2[1] < 0.0f && fInnerProduct3[1] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[2] > 0.0f && fInnerProduct1[2] > 0.0f && fInnerProduct2[2] > 0.0f && fInnerProduct3[2] > 0.0f)
		|| (fInnerProduct0[2] < 0.0f && fInnerProduct1[2] < 0.0f && fInnerProduct2[2] < 0.0f && fInnerProduct3[2] < 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// カラー設定
//*****************************************************************************
void C3DObject::SetColor(D3DXCOLOR colorMax, D3DXCOLOR colorMin, float fAttenuation)
{
	if (m_bColor)
	{
		return;
	}

	m_ColorMax = colorMax;
	m_ColorMin = colorMin;
	m_fAttenuationColor = fAttenuation;
	m_ColorChange = colorMax - colorMin;
}

//*****************************************************************************
// モデルのセット
//*****************************************************************************
int C3DObject::SetModel(ModelPattern *pModel)
{
	for (int nCnt = 0; nCnt < MODEL_PATTERN_MAX; nCnt++)
	{
		if (!m_ModelPattern[nCnt].bUse)
		{
			m_ModelPattern[nCnt].nNumMatModel = pModel->nNumMatModel;
			m_ModelPattern[nCnt].pBuffMatModel = pModel->pBuffMatModel;
			m_ModelPattern[nCnt].pMeshModel = pModel->pMeshModel;

			int nNumMatModel = pModel->nNumMatModel;
			for (int nCntTex = 0; nCntTex < nNumMatModel; nCntTex++)
			{
				m_ModelPattern[nCnt].pTexture[nCntTex] = pModel->pTexture[nCntTex];
			}

			m_ModelPattern[nCnt].bUse = true;
			return nCnt;
		}
	}
	return -1;
}

//*****************************************************************************
// モデルの全終了処理
//*****************************************************************************
void C3DObject::UninitAllModel()
{
	for (int nCnt = 0; nCnt < MODEL_PATTERN_MAX; nCnt++)
	{
		for (int i = 0; i < (int)m_ModelPattern[nCnt].nNumMatModel; i++)
		{
			if (m_ModelPattern[nCnt].pTexture[i] != nullptr)
			{// テクスチャの解放
				m_ModelPattern[nCnt].pTexture[i]->Release();
				m_ModelPattern[nCnt].pTexture[i] = nullptr;
			}
		}

		//メッシュの破棄
		if (m_ModelPattern[nCnt].pMeshModel != nullptr)
		{
			m_ModelPattern[nCnt].pMeshModel->Release();
			m_ModelPattern[nCnt].pMeshModel = nullptr;
		}

		//マテリアルの破棄
		if (m_ModelPattern[nCnt].pBuffMatModel != nullptr)
		{
			m_ModelPattern[nCnt].pBuffMatModel->Release();
			m_ModelPattern[nCnt].pBuffMatModel = nullptr;
		}

	}

	ZeroMemory(&m_ModelPattern, sizeof(m_ModelPattern));
}

//*****************************************************************************
// このモデルのワールド座標
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetWorldPos()
{
	D3DXVECTOR3 pos;
	pos.x = m_Model.mtxWorld._41;
	pos.y = m_Model.mtxWorld._42;
	pos.z = m_Model.mtxWorld._43;

	return pos;
}

D3DXVECTOR3 C3DObject::GetWorlMatrixRot()
{
	D3DXVECTOR3 pos;
	pos.x = m_Model.mtxWorldRot._41;
	pos.y = m_Model.mtxWorldRot._42;
	pos.z = m_Model.mtxWorldRot._43;
	return pos;
}

//*****************************************************************************
// このモデルのワールドRot
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetWorldRot()
{
	D3DXVECTOR3 rot;
	rot.x = m_Model.mtxWorld._41;
	rot.y = m_Model.mtxWorld._42;
	rot.z = m_Model.mtxWorld._43;

	return rot;
}

//*****************************************************************************
// モデルのサイズ
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetSize()
{
	D3DXVECTOR3 size;

	D3DXVECTOR3 minpos, maxpos;

	minpos = m_Model.pos + m_Model.vtxMin;

	maxpos = m_Model.pos + m_Model.vtxMax;

	size = maxpos - minpos;
	
	return size;
}