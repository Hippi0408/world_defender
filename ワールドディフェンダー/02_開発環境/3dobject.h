//=============================================================================
//
// 3dobject.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _3DOBJECT_H_			//このマクロ定義がされなかったら
#define _3DOBJECT_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//モデルの情報の構造体
typedef struct
{
	int nPattn;							//モデル番号
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
}Object_Data;

//*****************************************************************************
// 前方宣言
//*****************************************************************************
enum Object_Type_List;
#ifdef _DEBUG
class CLine;
#endif // _DEBUG

//*****************************************************************************
// クラス定義
//*****************************************************************************
class C3DObject : public CObject
{
public:
	static const int MODEL_MAX_TEXTURE = 30;
	static const int MODEL_PATTERN_MAX = 120;

	//モデルパターン構造体
	typedef struct
	{
		LPD3DXMESH pMeshModel;		//メッシュ情報へのポインタ
		LPD3DXBUFFER pBuffMatModel;	//マテリアル情報へのポインタ
		DWORD nNumMatModel;				//マテリアル情報の数
		LPDIRECT3DTEXTURE9 pTexture[MODEL_MAX_TEXTURE];		// テクスチャへのポインタ
		bool bUse;
	}ModelPattern;

	//モデル構造体
	typedef struct
	{
		D3DXVECTOR3* pNormalPolygon;						//面法線
		D3DXVECTOR3* pTopPos;								//頂点の位置
		D3DXMATRIX mtxWorld;								//ワールドマトリックス
		D3DXMATRIX mtxWorldRot;								//ワールドマトリックス(向きだけ)
		D3DXVECTOR3 posParent;								//位置親
		D3DXVECTOR3 rotParent;								//向き親
		D3DXVECTOR3 rot;									//向き
		D3DXVECTOR3 pos;									//位置
		D3DXVECTOR3 move;									//移動
		D3DXVECTOR3 vtxMin, vtxMax;							//モデルのサイズ(元)
		D3DXVECTOR3 vtxMin2, vtxMax2;						//モデルのサイズ(ワールドマトリックスで変換したもの)
		D3DXVECTOR3 posMove = D3DXVECTOR3(0.0f,0.0f,0.0f);	//位置動く時用
		int nPattn;											//モデルのパターン
	}Model;

public:
	C3DObject();
	virtual ~C3DObject() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	void Set3DObject(int nPattn, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPos() { return m_Model.pos; }
	D3DXVECTOR3 GetRot() { return m_Model.rot; }
	void SetPos(D3DXVECTOR3 pos) { m_Model.pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Model.rot = rot; }
	D3DXVECTOR3 GetParentPos() { return m_Model.posParent; }
	D3DXVECTOR3 GetParentRot() { return m_Model.rotParent; }
	void SetParentPos(D3DXVECTOR3 pos) { m_Model.posParent = pos; }
	void SetParentRot(D3DXVECTOR3 rot) { m_Model.rotParent = rot; }
	void AddParentPos(D3DXVECTOR3 add) { m_Model.posParent += add; }
	void SetPosMove(D3DXVECTOR3 pos) { m_Model.posMove = pos; }
	D3DXVECTOR3 GetPosMove() { return m_Model.posMove; }
	void AddPosMove(D3DXVECTOR3 add) { m_Model.posMove += add; }
	void AddPos(D3DXVECTOR3 add) { m_Model.pos += add; }
	void AddRot(D3DXVECTOR3 add) { m_Model.rot += add; }
	void SetSize(float fSize) { m_fSize = fSize; }
	D3DXVECTOR3 GetLightVec() { return m_LightVec; }
	void SetLightVec(D3DXVECTOR3 LightVec) { m_LightVec = LightVec; }
	void SetShadowPos(D3DXVECTOR3 shadowpos) { m_ShadowPos = shadowpos; }
	D3DXVECTOR3 GetWorldPos();
	D3DXVECTOR3 GetWorlMatrixRot();
	D3DXVECTOR3 GetWorldRot();
	D3DXMATRIX GetMatrix() { return m_Model.mtxWorld; }
	D3DXMATRIX GetMatrixRot() { return m_Model.mtxWorldRot; }
	void SetMatrix(D3DXMATRIX mtx) { m_Model.mtxWorld = mtx; }
	D3DXVECTOR3 GetSize();
	void SetModelPattnNum(int nPattnNum) { m_Model.nPattn = nPattnNum; }
	int GetModelPattnNum() { return m_Model.nPattn; }
	void CalculationMatrix();
	void SetNormal();			//法線設定
	void UpdateNormal();		//法線の向きの更新
	virtual D3DXVECTOR3 Collision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos);//当たり判定
	virtual bool NormalCollision(D3DXVECTOR3 pos);//当たり判定
	bool TriangleInOut(D3DXVECTOR3 pos, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2);//三角形の内にいるかどうか
	bool SquareInOut(D3DXVECTOR3 pos , D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3);//四角形の内にいるかどうか
	D3DXVECTOR3 GetVtxMin() { return m_Model.vtxMin; }
	D3DXVECTOR3 GetVtxMax() { return m_Model.vtxMax; }

	void SetWithinRangeColor(bool bUes) { m_bColor = bUes; }
	void SetColor(D3DXCOLOR colorMax, D3DXCOLOR colorMin, float fAttenuation);
	

	void SetObject_Type_List(Object_Type_List object_type_list) { m_Object_Type_List = object_type_list; }
	Object_Type_List GetObject_Type_List() { return m_Object_Type_List; }

	LPD3DXMESH GetMeshData() { return m_ModelPattern[m_Model.nPattn].pMeshModel; }

	//面法線
	D3DXVECTOR3* GetNormalPolygon() { return m_Model.pNormalPolygon; }
	D3DXVECTOR3* GetTop() { return m_Model.pTopPos; }

	static int SetModel(ModelPattern *pModel);
	static void UninitAllModel();

private:
	static ModelPattern m_ModelPattern[MODEL_PATTERN_MAX];
	Model m_Model;
	float m_fSize;//サイズ
	D3DXVECTOR3 m_LightVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
	D3DXVECTOR3 m_ShadowPos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Object_Type_List m_Object_Type_List;
	D3DXCOLOR m_Color;
	D3DXCOLOR m_ColorChange;
	D3DXCOLOR m_ColorMax;
	D3DXCOLOR m_ColorMin;
	float m_fAttenuationColor;
	bool m_bColor;
	D3DCOLORVALUE* m_pDefaultColor;

#ifdef _DEBUG
	CLine* m_pLine;
#endif // _DEBUG
};

#endif