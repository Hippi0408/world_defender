//=============================================================================
//
// convenience_function.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "convenience_function.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CConvenience_Function::CConvenience_Function()
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CConvenience_Function::~CConvenience_Function()
{
}

//*****************************************************************************
// Rotの正規化
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::NormalizationRot(D3DXVECTOR3 In)
{
	D3DXVECTOR3 rot = In;

	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	return rot;
}

//*****************************************************************************
// 円同士の当たり判定
//*****************************************************************************
bool CConvenience_Function::CircleCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//位置の保存
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//２この物体の半径同士の和
	float fDiff = fRadius1 + fRadius2;

	//計算用変数
	float fCalculationX, fCalculationZ;

	//Xの差分
	fCalculationX = Pos1.x - Pos2.x;
	//Yの差分
	fCalculationZ = Pos1.z - Pos2.z;

	//現在の２点の距離
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationZ * fCalculationZ);

	//２この物体の半径同士の和より現在の２点の距離が小さいかどうか
	if (fDiff >= fLength)
	{
		//当たった
		return true;
	}

	//当たってない
	return false;
}

//*****************************************************************************
// 球同士の当たり判定
//*****************************************************************************
bool CConvenience_Function::SphereCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//位置の保存
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//２この物体の半径同士の和
	float fDiff = fRadius1 + fRadius2;

	//計算用変数
	D3DXVECTOR3 Distance;

	//2点との距離
	Distance = Pos1 - Pos2;

	//現在の２点の距離
	float fLength = sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//２この物体の半径同士の和より現在の２点の距離が小さいかどうか
	if (fDiff >= fLength)
	{
		//当たった
		return true;
	}

	//当たってない
	return false;
}

//*****************************************************************************
// 2点間の距離
//*****************************************************************************
float CConvenience_Function::PointDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	//位置の保存
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//計算用変数
	D3DXVECTOR3 Distance;

	//2点との距離
	Distance = Pos1 - Pos2;

	//現在の２点の距離
	float fLength = sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//計算結果を返す
	return fLength;
}

//*****************************************************************************
// 内積の矩形当たり判定、中にいるかどうか
//*****************************************************************************
bool CConvenience_Function::InnerProductCollision(D3DXVECTOR3 * pTop1, D3DXVECTOR3 * pTop2, D3DXVECTOR3 TopTarget)
{
	//チェック用
	D3DXVECTOR3 aPos[4];

	if (!InnerProductCollisionBase(pTop1, TopTarget))
	{
		return false;
	}
	if (!InnerProductCollisionBase(pTop2, TopTarget))
	{
		return false;
	}

	aPos[0] = pTop1[0];
	aPos[1] = pTop1[1];
	aPos[2] = pTop2[1];
	aPos[3] = pTop1[0];

	if (!InnerProductCollisionSideX(aPos, TopTarget))
	{
		return false;
	}

	aPos[0] = pTop1[2];
	aPos[1] = pTop1[3];
	aPos[2] = pTop2[3];
	aPos[3] = pTop1[2];

	if (!InnerProductCollisionSideX(aPos, TopTarget))
	{
		return false;
	}


	aPos[0] = pTop1[0];
	aPos[1] = pTop1[3];
	aPos[2] = pTop2[3];
	aPos[3] = pTop1[0];

	if (!InnerProductCollisionSideZ(aPos, TopTarget))
	{
		return false;
	}

	aPos[0] = pTop1[1];
	aPos[1] = pTop1[2];
	aPos[2] = pTop2[2];
	aPos[3] = pTop1[1];

	if (!InnerProductCollisionSideX(aPos, TopTarget))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
// 内積の矩形当たり判定、中にいるかどうか（底面）
//*****************************************************************************
bool CConvenience_Function::InnerProductCollisionBase(D3DXVECTOR3* pTop1, D3DXVECTOR3 Top2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2, fInnerProduct3;


	vec1 = pTop1[1] - pTop1[0];
	vec2 = Top2 - pTop1[0];

	fInnerProduct0 = vec1.x * vec2.z - vec1.z * vec2.x;

	vec1 = pTop1[2] - pTop1[1];
	vec2 = Top2 - pTop1[1];

	fInnerProduct1 = vec1.x * vec2.z - vec1.z * vec2.x;

	vec1 = pTop1[3] - pTop1[2];
	vec2 = Top2 - pTop1[2];

	fInnerProduct2 = vec1.x * vec2.z - vec1.z * vec2.x;

	vec1 = pTop1[0] - pTop1[3];
	vec2 = Top2 - pTop1[3];

	fInnerProduct3 = vec1.x * vec2.z - vec1.z * vec2.x;

	if (
		(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f && fInnerProduct3 >= 0.0f)
		|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f && fInnerProduct3 <= 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// 内積の矩形当たり判定、中にいるかどうか（側面）
//*****************************************************************************
bool CConvenience_Function::InnerProductCollisionSideX(D3DXVECTOR3* pTop1, D3DXVECTOR3 Top2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2, fInnerProduct3;


	vec1 = pTop1[1] - pTop1[0];
	vec2 = Top2 - pTop1[0];

	fInnerProduct0 = vec1.x * vec2.y - vec1.y * vec2.x;

	vec1 = pTop1[2] - pTop1[1];
	vec2 = Top2 - pTop1[1];

	fInnerProduct1 = vec1.x * vec2.y - vec1.y * vec2.x;

	vec1 = pTop1[3] - pTop1[2];
	vec2 = Top2 - pTop1[2];

	fInnerProduct2 = vec1.x * vec2.y - vec1.y * vec2.x;

	vec1 = pTop1[0] - pTop1[3];
	vec2 = Top2 - pTop1[3];

	fInnerProduct3 = vec1.x * vec2.y - vec1.y * vec2.x;

	if (
		(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f && fInnerProduct3 >= 0.0f)
		|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f && fInnerProduct3 <= 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// 内積の矩形当たり判定、中にいるかどうか（側面）
//*****************************************************************************
bool CConvenience_Function::InnerProductCollisionSideZ(D3DXVECTOR3* pTop1, D3DXVECTOR3 Top2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2, fInnerProduct3;


	vec1 = pTop1[1] - pTop1[0];
	vec2 = Top2 - pTop1[0];

	fInnerProduct0 = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = pTop1[2] - pTop1[1];
	vec2 = Top2 - pTop1[1];

	fInnerProduct1 = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = pTop1[3] - pTop1[2];
	vec2 = Top2 - pTop1[2];

	fInnerProduct2 = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = pTop1[0] - pTop1[3];
	vec2 = Top2 - pTop1[3];

	fInnerProduct3 = vec1.z * vec2.y - vec1.y * vec2.z;

	if (
		(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f && fInnerProduct3 >= 0.0f)
		|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f && fInnerProduct3 <= 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// 内積の矩形当たり判定、押し出し（上面）
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::InnerProductCollisionBaseExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos)
{
	//面の１頂点
	D3DXVECTOR3 P1 = Top1;
	//面の法線
	D3DXVECTOR3 vec = Normal;
	//対象の位置
	D3DXVECTOR3 pos = D3DXVECTOR3(TopPos.x, TopPos.y, TopPos.z); 

	//対象のPosの面内のY
	float fPolygonY = P1.y - ((pos.x - P1.x) * vec.x + (pos.z - P1.z) * vec.z) / vec.y;

	//Yの保存
	pos.y = fPolygonY;

	//Posを返す
	return pos;
}

//*****************************************************************************
// 内積の矩形当たり判定、押し出し（X側面）
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::InnerProductCollisionSideXExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos)
{
	//面の１頂点
	D3DXVECTOR3 P1 = Top1;
	//面の法線
	D3DXVECTOR3 vec = Normal;
	//対象の位置
	D3DXVECTOR3 pos = D3DXVECTOR3(TopPos.x, TopPos.y, TopPos.z);

	//対象のPosの面内のX
	float fPolygonX = P1.x - ((pos.z - P1.z) * vec.z + (pos.y - P1.y) * vec.y) / vec.x;
	
	//Xの保存
	pos.x = fPolygonX;

	//Posを返す
	return pos;
}

//*****************************************************************************
// 内積の矩形当たり判定、押し出し（Z側面）
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::InnerProductCollisionSideZExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos)
{
	//面の１頂点
	D3DXVECTOR3 P1 = Top1;
	//面の法線
	D3DXVECTOR3 vec = Normal;
	//対象の位置
	D3DXVECTOR3 pos = D3DXVECTOR3(TopPos.x, TopPos.y, TopPos.z);

	//対象のPosの面内のZ
	float fPolygonZ = P1.z - ((pos.x - P1.x) * vec.x + (pos.y - P1.y) * vec.y) / vec.z;

	//Zの保存
	pos.z = fPolygonZ;

	//Posを返す
	return pos;
}

//*****************************************************************************
// 点の方向ベクトルの生成
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::PointOrientationVectorGeneration(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	//方向ベクトル
	D3DXVECTOR3 vec;

	//ベクトルの生成
	vec = pos1 - pos2;

	//長さを1にする
	D3DXVec3Normalize(&vec,&vec);

	return vec;
}
