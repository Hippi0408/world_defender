//=============================================================================
//
//　メッシュ円柱処理 [mesh_cylinder.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _MESH_CYLINDER_H_			//このマクロ定義がされなかったら
#define _MESH_CYLINDER_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "meshfield.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//軌跡構造体
struct Mesh_Cylinder_Structure
{
	D3DXVECTOR3 ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR ColorMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR ColorLowest = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	int nTextureNum = 0;
	int nPolygonX = 1;
	int nPolygonY = 1;
	float fRadius = 0.0f;
	float fSizeYTop = 0.0f;
	int nAttenuationFrame = 1;
	float fRotMove = 0.0f;
	bool bFade = false;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMesh_Cylinder : public CMeshfield
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int MESH_CYLINDER_VALUE = 50;
public:
	CMesh_Cylinder();
	~CMesh_Cylinder() override;

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	void Update(D3DXVECTOR3 Pos);//Pos更新処理
	virtual void Draw() override;//描画処理
	void SetMesh_Cylinder(Mesh_Cylinder_Structure mesh_cylinder_structure);
	void SetMeshTopPos() override;//各頂点の位置の設定
	void SetNormal() override;			//法線設定
	void ChangeColor();
	void SetRadius(float fRadius) { m_Mesh_Cylinder_Structure.fRadius = fRadius; }
	float GetRadius() { return  m_Mesh_Cylinder_Structure.fRadius; }
	void SetSizeTop(float fSizeTop) { m_Mesh_Cylinder_Structure.fSizeYTop = fSizeTop; }
	float GetSizeTop() { return m_Mesh_Cylinder_Structure.fSizeYTop; }

private:
	Mesh_Cylinder_Structure m_Mesh_Cylinder_Structure;
	D3DXVECTOR3 m_MeshRot;
	bool m_bColorChange;
	D3DXCOLOR m_AddColor;
	int m_nAttenuationFrameCnt;
};

#endif
