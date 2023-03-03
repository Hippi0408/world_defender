//=============================================================================
//
// ElasticityPolygon.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ELASTICITYPOLYGON_H_			//このマクロ定義がされなかったら
#define _ELASTICITYPOLYGON_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CElasticityPolygon : public CObject
{
	// 頂点フォーマット
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

public:
	CElasticityPolygon();//コンストラクタ
	virtual ~CElasticityPolygon() override;//デストラクタ

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	virtual void Draw() override;//描画処理

	virtual bool IsUnused() { return true; }//終了条件

	void SetPolygon();//更新処理外でのセットポリゴン
	void SetDiagonalLine(float fwidth, float fheight);//縦横幅の設定
	bool Collision(D3DXVECTOR3 pos, float radius);//当たり判定

	void SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot) { m_Pos = pos; m_Rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void AddPos(D3DXVECTOR3 pos) { m_Pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void SetColor(D3DXCOLOR col) { m_Col = col; }
	void SetTextIndex(int nIndex) { m_nTextIndex = nIndex; }
	void SetFlashing(int nflashing) { m_nFlashingCnt = nflashing; }
	void SetFadeSpeed(float fspeed) { m_fFadeSpeed = fspeed; }

	void SetHeightElasticity(float fHeightElasticity);//伸びるポリゴン縦幅のセット
	void SetHeightMax(float fHeightMax) { m_fHeightMax = fHeightMax; }//伸びるポリゴン縦幅Maxのセット
	float GetHeightMax() { return m_fHeightMax; }
	float GetHeight() { return m_fHeight; }
	void AddHeightElasticity(float fHeightElasticity) { m_fHeightElasticity += fHeightElasticity; }//伸びるポリゴンの延長
	float GetHeightElasticity() { return m_fHeightElasticity; }//現在の長さ

	D3DXVECTOR2 GetUVSizeMax() { return m_UVSizeMax; }
	D3DXVECTOR2 GetUVSizeMin() { return m_UVSizeMin; }

	void SetUVSizeMax(D3DXVECTOR2 UVSize) { m_UVSizeMax = UVSize; }
	void SetUVSizeMin(D3DXVECTOR2 UVSize) { m_UVSizeMin = UVSize; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//頂点バッファへのポインタ
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	D3DXCOLOR m_Col;
	D3DXVECTOR2 m_UVSizeMax;
	D3DXVECTOR2 m_UVSizeMin;

	float m_fWidth;//横幅
	float m_fHeight;//縦幅

	float m_fHeightMax;//縦幅

	float m_fHeightElasticity;//伸縮するサイズ（ｙの伸縮）
	
	float m_lenTop;//対角線の長さTop
	float m_lenUnder;//対角線の長さUnder
	
	float m_angleTop;//対角線の角度Top
	float m_angleUnder;//対角線の角度Under
	bool m_bFadeInOut;//フェード状態(true = in,false = out)
	int m_nFlashingCnt;
	float m_fFadeSpeed;
	int m_nTextIndex;

};

#endif