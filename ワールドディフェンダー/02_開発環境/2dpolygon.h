//=============================================================================
//
// 2DPolygon.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _2DPOLGON_H_			//このマクロ定義がされなかったら
#define _2DPOLGON_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class C2DPolygon : public CObject
{
private:
	// 頂点フォーマット
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	static const float FLASHING_AMOUNT;

public:
	C2DPolygon();//コンストラクタFlashing
	virtual ~C2DPolygon() override;//デストラクタ

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	virtual void Draw() override;//描画処理

	virtual bool IsUnused() { return true; }//終了条件処理
	virtual void SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);//セットアップ

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void AddPos(D3DXVECTOR3 pos) { m_Pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	D3DXVECTOR3 GetMove() { return m_Move; }
	void SetUV(D3DXVECTOR2 uv) { m_UV = uv; }
	void AddUVPos(D3DXVECTOR2 uvmove) { m_UV += uvmove; }
	void SetUVMove(D3DXVECTOR2 uvmove) { m_UVMove = uvmove; }
	void SetUVSize(D3DXVECTOR2 uvsize) { m_UVSize = uvsize; }
	void SetColor(D3DXCOLOR col) { m_Col = col; }
	void AddColor(D3DXCOLOR col) { m_Col += col; }
	D3DXCOLOR GetColor() { return m_Col; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void SetFlashing(int nflashing) { m_nFlashingCnt = nflashing; }
	void SetFadeSpeed(float fspeed) { m_fFadeSpeed = fspeed; }
	void PolygonAppear(float fAppear);
	void SetAlphaLimitValue(float fAlphaMax, float fAlphaLeast) { m_fAlphaMax = fAlphaMax; m_fAlphaLeast = fAlphaLeast; }

	virtual void SetDiagonalLine(float fwidth, float fheight);//表示するポリゴンの縦横幅設定
	void AddDiagonalLine(float faddwidth, float faddheight);//表示するポリゴンの縦横幅Add
	float GetActualRadius() { return m_ActualRadius; }//実際の半径

	virtual void SetPolygon();//更新処理以外でのポリゴンのセット
	void SetTextIndex(int nIndex) { m_nTextIndex = nIndex; }//このポリゴンが使用するテクスチャ番号のセット
	int GetTextIndex() { return m_nTextIndex; }//このポリゴンが使用するテクスチャ番号の取得
	virtual bool Collision(D3DXVECTOR3 pos,float radius);//当たり判定

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//頂点バッファへのポインタ
	D3DXVECTOR3 m_Pos;//位置
	D3DXVECTOR3 m_Rot;//向き
	D3DXVECTOR3 m_Move;//移動量
	D3DXVECTOR2 m_UVMove;//UVの移動量
	D3DXVECTOR2 m_UV;//UVの座標
	D3DXVECTOR2 m_UVSize;//UVのサイズ

	D3DXCOLOR m_Col;//色
	float m_fAlphaMax;
	float m_fAlphaLeast;
	float m_fWidth;//横幅
	float m_fHeight;//縦幅
	//対角線の長さ
	float m_len;
	//対角線の角度
	float m_angle;
	float m_ActualRadius;//実際の半径
	int m_nTextIndex;//テクスチャ番号
	int m_nFlashingCnt;//点滅カウント
	bool m_bFadeInOut;//フェード状態(true = in,false = out)
	float m_fFadeSpeed;//点滅の速さ
	
};

#endif