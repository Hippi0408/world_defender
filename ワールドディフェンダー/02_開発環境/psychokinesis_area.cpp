//=============================================================================
//
// 軌跡処理 [psychokinesis_area.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "psychokinesis_area.h" 
#include "texture.h"
#include <assert.h>
#include "convenience_function.h"
#include "mesh_cylinder.h"
#include "input.h"

const D3DXVECTOR3 CPsychokinesis_Area::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const float CPsychokinesis_Area::HEIGHT_LOCUS = 80.0f;
const float CPsychokinesis_Area::RADIUS_INIT = 200.0f;
const float CPsychokinesis_Area::RADIUS_MAX = 1600.0f;
const float CPsychokinesis_Area::SIZE_TOP = 50.0f;
const float CPsychokinesis_Area::RADIUS_MOVE = D3DXToRadian(5);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPsychokinesis_Area::CPsychokinesis_Area()
{
	m_PLPos = INIT_POS;
	m_fRadius = 0.0f;
	m_fSizeTop = SIZE_TOP;
	m_pMesh_Cylinder = nullptr;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPsychokinesis_Area::~CPsychokinesis_Area()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CPsychokinesis_Area::Init()
{
	//Numチェック
	if (m_pMesh_Cylinder != nullptr)
	{
		assert(false);
	}

	//情報の確保
	m_pMesh_Cylinder = new CMesh_Cylinder;

	//初期化
	if (FAILED(m_pMesh_Cylinder->Init()))
	{
		return -1;
	}

	Mesh_Cylinder_Structure Mesh_Cylinder_Structure;

	Mesh_Cylinder_Structure.fRadius = RADIUS_INIT;
	Mesh_Cylinder_Structure.fSizeYTop = m_fSizeTop;
	Mesh_Cylinder_Structure.nPolygonX = 30;
	Mesh_Cylinder_Structure.nPolygonY = 1;
	Mesh_Cylinder_Structure.nTextureNum = CTexture::LoadTexture("data/TEXTURE/軌跡.png");
	Mesh_Cylinder_Structure.ParentPos = m_PLPos;
	Mesh_Cylinder_Structure.ColorMax = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
	Mesh_Cylinder_Structure.ColorLowest = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);
	Mesh_Cylinder_Structure.nAttenuationFrame = 120;
	Mesh_Cylinder_Structure.bFade = true;
	Mesh_Cylinder_Structure.fRotMove = D3DXToRadian(1);

	m_pMesh_Cylinder->SetMesh_Cylinder(Mesh_Cylinder_Structure);


	return S_OK;
}

//*****************************************************************************
// 初期化(Posあり)
//*****************************************************************************
HRESULT CPsychokinesis_Area::Init(D3DXVECTOR3 Pos)
{
	//プレイヤーの位置の保管
	m_PLPos = Pos;

	//初期化
	if (FAILED(Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CPsychokinesis_Area::Uninit()
{
	//リングの情報の解放
	if (m_pMesh_Cylinder != nullptr)
	{
		//終了処理
		m_pMesh_Cylinder->Uninit();
		delete m_pMesh_Cylinder;
		m_pMesh_Cylinder = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CPsychokinesis_Area::Update()
{
	m_pMesh_Cylinder->SetRadius(RADIUS_INIT + m_fRadius);
	m_pMesh_Cylinder->SetSizeTop(m_fSizeTop);

	m_pMesh_Cylinder->Update(m_PLPos);
}

//*****************************************************************************
// 更新処理(オーバーロードPos更新あり)
//*****************************************************************************
void CPsychokinesis_Area::Update(D3DXVECTOR3 Pos)
{
	//プレイヤーの位置の更新
	m_PLPos = Pos;

	//更新処理
	Update();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CPsychokinesis_Area::Draw()
{
	//描画処理
	m_pMesh_Cylinder->Draw();
}

//*****************************************************************************
// 割合計算処理
//*****************************************************************************
void CPsychokinesis_Area::RateCalculation(float fRate)
{
	m_fRadius = RADIUS_MAX * fRate;
}

