//=============================================================================
//
// 瓦礫処理 [ballast.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "ballast.h"
#include "convenience_function.h"
#include "game.h"
#include "manager.h"
#include "ballast_manager.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CBallast::CBallast()
{
	m_nListNumber = 0;
	m_MoveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bFloating = false;
	m_ParentPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBallast::~CBallast()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CBallast::Init()
{
	m_nSizePoint = 0;

	//使用する
	m_bUse = true;

	//初期化
	if (FAILED(C3DObject::Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CBallast::Uninit()
{
	//親クラスの終了処理
	C3DObject::Uninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CBallast::Update()
{
	//各情報の取得
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move = GetPosMove();
	D3DXVECTOR3 rot = GetRot();

	if (!m_bFloating)
	{
		//子の位置の更新
		pos = GetParentPos();
		pos += move;
		SetParentPos(pos);
	}
	else
	{
		//子の位置の更新
		pos = GetPos();
		pos += move;
		SetPos(pos);
	}


	//向きの更新
	rot += m_MoveRot;
	SetRot(rot);

	//親位置の更新
	AddParentPos(m_ParentPosMove);

	//親クラスの更新
	C3DObject::Update();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CBallast::Draw()
{
	//マトリックスの計算
	CalculationMatrix();
	//親クラスの描画処理
	C3DObject::Draw();
}

//*****************************************************************************
//半径の設定
//*****************************************************************************
void CBallast::SetRadius()
{
	//各辺
	float fSideX, fSideY, fSideZ;

	//サイズ
	D3DXVECTOR3 Max, Min;

	//サイズの最大最小の取得
	Max = GetVtxMax();
	Min = GetVtxMin();

	//各辺の計算
	fSideX = Max.x - Min.x;
	fSideY = Max.y - Min.y;
	fSideZ = Max.z - Min.z;

	//対角線の生成
	float fDiagonalLine = sqrt(fSideX * fSideX + fSideY * fSideY + fSideZ * fSideZ);

	//対角線から半径の生成
	m_fRadius = fDiagonalLine / 2.0f;

	//サイズポイントの計算
	m_nSizePoint = (int)m_fRadius / 10.0f;

	//サイズポイントの調整
	if (m_nSizePoint > 100)
	{
		m_nSizePoint = 100;
	}


}

//*****************************************************************************
//	当たり判定
//*****************************************************************************
D3DXVECTOR3 CBallast::ConclusionCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	//最終的な押し出し値
	D3DXVECTOR3 Add = pos;

	
	//接触確認用変数
	D3DXVECTOR3 HittingTargetPosMax[SURFACE_MAX], HittingTargetPosMin[SURFACE_MAX], HittingTargetOldPosMax[SURFACE_MAX], HittingTargetOldPosMin[SURFACE_MAX];

	//モデルの位置
	D3DXVECTOR3 ModelPos = GetPos();
	D3DXVECTOR3 ModelRot = GetRot();
	D3DXVECTOR3 ModelMax = GetVtxMax();
	D3DXVECTOR3 ModelMin = GetVtxMin();

	//マトリックス変換を掛ける
	D3DXVECTOR3 MatrixConversionMax, MatrixConversionMin;

	//代入
	MatrixConversionMax = ModelPos + ModelMax;
	MatrixConversionMin = ModelPos + ModelMin;

	//自身の回転のマトリックス
	D3DXMATRIX ThisMatrixRot = GetMatrix();

	//当たり判定用の面の４頂点
	D3DXVECTOR3 aTop[4];
	D3DXVECTOR3 Top,Down;


	Top = D3DXVECTOR3(0.0f, MatrixConversionMax.y, 0.0f);
	Down = D3DXVECTOR3(0.0f, MatrixConversionMin.y, 0.0f);
	
	D3DXVec3TransformCoord(&Top, &Top, &ThisMatrixRot);
	D3DXVec3TransformCoord(&Down, &Down, &ThisMatrixRot);

	//*****************************************************************************
	//上面
	//*****************************************************************************

	aTop[0] = D3DXVECTOR3(MatrixConversionMin.x, MatrixConversionMax.y, MatrixConversionMin.z);
	aTop[1] = D3DXVECTOR3(MatrixConversionMin.x, MatrixConversionMax.y, MatrixConversionMax.z);
	aTop[2] = D3DXVECTOR3(MatrixConversionMax.x, MatrixConversionMax.y, MatrixConversionMax.z);
	aTop[3] = D3DXVECTOR3(MatrixConversionMax.x, MatrixConversionMax.y, MatrixConversionMin.z);

	D3DXVec3TransformCoord(&aTop[0], &aTop[0], &ThisMatrixRot);
	D3DXVec3TransformCoord(&aTop[1], &aTop[1], &ThisMatrixRot);
	D3DXVec3TransformCoord(&aTop[2], &aTop[2], &ThisMatrixRot);
	D3DXVec3TransformCoord(&aTop[3], &aTop[3], &ThisMatrixRot);

	if (!CConvenience_Function::InnerProductCollisionBase(aTop, D3DXVECTOR3(pos.x, pos.y, pos.z)))
	{
		//押し出す値を返す
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (!(pos.y > Down.y
		&& pos.y < Top.y))
	{
		//押し出す値を返す
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	D3DXVECTOR3 TopPos;

	//瓦礫の上面用のPos
	D3DXVECTOR3 BallstTop;


	//側面

	//移動ベクトル
	D3DXVECTOR3 Vec = pos - oldpos;

	D3DXVECTOR3 vec1, vec2;
	float fInnerProductPos, fInnerProductOldPos;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//指定の辺の中にいるかどうか
		vec1 = aTop[(nCnt + 1) % 4] - aTop[nCnt];
		vec2 = pos - aTop[nCnt];

		fInnerProductPos = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = aTop[(nCnt + 1) % 4] - aTop[nCnt];
		vec2 = oldpos - aTop[nCnt];

		fInnerProductOldPos = vec1.x * vec2.z - vec1.z * vec2.x;

		//いる場合
		if (fInnerProductPos < 0.0f
			&& fInnerProductOldPos >= 0.0f
			&& oldpos.y > Down.y
			&& oldpos.y < Top.y
			)
		{
			//計算用変数宣言
			D3DXVECTOR3 V, V1, V2, Cross1, Cross2, Result;
			float fT2;

			//線分と線分の衝突
			V = aTop[nCnt] - oldpos;
			V1 = pos - oldpos;
			V2 = aTop[(nCnt + 1) % 4] - aTop[nCnt];

			V.y = 0.0f;
			V1.y = 0.0f;
			V2.y = 0.0f; 

			fT2 = (V.x * V1.z - V.z * V1.x) / (V1.x * V2.z - V1.z * V2.x);

			Result = aTop[nCnt] + fT2 * V2;

			Result.y = pos.y;

			V1 *= -1.0f;

			D3DXVec3Normalize(&V1, &V1);

			return Result + V1;

		}

	}
		
	//*****************************************************************************
	//上面
	//*****************************************************************************

	//法線
	D3DXVECTOR3 Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線をRotマトリックスで変換
	D3DXVec3TransformCoord(&Normal, &Normal, &GetMatrixRot());

	//ベクトルの長さを１にする
	D3DXVec3Normalize(&Normal,&Normal);

	//上面の１頂点
	 TopPos = GetPos() + D3DXVECTOR3(MatrixConversionMin.x, MatrixConversionMax.y, MatrixConversionMin.z);
	
	//瓦礫の上面用のPos
	 BallstTop;

	//上面用の当たり判定
	BallstTop = CConvenience_Function::InnerProductCollisionBaseExtrusion(TopPos, Normal, D3DXVECTOR3(pos.x, pos.y + ModelMin.y, pos.z));

	//上面の高さ判定
	if (pos.y < BallstTop.y
		&& oldpos.y >= BallstTop.y)
	{
		//押し出す値を返す
		return BallstTop;
	}

	//押し出す値を返す
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//カメラ描画の有無
//*****************************************************************************
bool CBallast::IsExistingDuring2Point(D3DXVECTOR3 posR, D3DXVECTOR3 posV)
{
	//マトリックス
	D3DXMATRIX mat, invmat;

	//マトリックスの取得
	mat = GetMatrix();

	//逆行列の計算
	D3DXMatrixInverse(&invmat, NULL, &mat);

	//変換
	D3DXVec3TransformCoord(&posR, &posR, &invmat);
	D3DXVec3TransformCoord(&posV, &posV, &invmat);

	//メッシュ情報
	LPD3DXMESH pMesh = GetMeshData();

	//注視点から視点のベクトル
	D3DXVECTOR3 Vec = posV - posR;

	//長さを１にする
	D3DXVec3Normalize(&Vec,&Vec);

	//当たったかどうか
	BOOL bHit = false;

	//レイを飛ばしてメッシュと当たってるかどうか
	if (FAILED(D3DXIntersect(pMesh, &posR, &Vec, &bHit,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)))
	{
		return false;
	}

	return bHit;
}
