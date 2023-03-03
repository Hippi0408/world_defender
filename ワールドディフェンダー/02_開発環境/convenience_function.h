//=============================================================================
//
// convenience_function.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _CONVENIENCE_FUNCTION_H_			//このマクロ定義がされなかったら
#define _CONVENIENCE_FUNCTION_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CConvenience_Function
{
private:
	
public:
	CConvenience_Function();
	~CConvenience_Function();

	static D3DXVECTOR3 NormalizationRot(D3DXVECTOR3 In);
	static bool CircleCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2);
	static bool SphereCollision(D3DXVECTOR3 pos1,float fRadius1, D3DXVECTOR3 pos2, float fRadius2);
	static float PointDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	static bool InnerProductCollision(D3DXVECTOR3* pTop1, D3DXVECTOR3* pTop2, D3DXVECTOR3 TopTarget);
	static bool InnerProductCollisionBase(D3DXVECTOR3* Top1, D3DXVECTOR3 Top2);
	static bool InnerProductCollisionSideX(D3DXVECTOR3* Top1, D3DXVECTOR3 Top2);
	static bool InnerProductCollisionSideZ(D3DXVECTOR3* Top1, D3DXVECTOR3 Top2);
	static D3DXVECTOR3 InnerProductCollisionBaseExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos);
	static D3DXVECTOR3 InnerProductCollisionSideXExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos);
	static D3DXVECTOR3 InnerProductCollisionSideZExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos);
	static D3DXVECTOR3 PointOrientationVectorGeneration(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
private:
};

#endif