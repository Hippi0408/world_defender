//=============================================================================
//
//　軌跡処理 [locus.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _LOCUS_H_			//このマクロ定義がされなかったら
#define _LOCUS_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "meshfield.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//軌跡構造体
struct LocusStructure
{
	D3DXVECTOR3 TopPos;
	D3DXVECTOR3 DownPos;
	int nPolygon;
	int nTextureNum;
	int nSaveInterval;
	D3DXCOLOR BeginningCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR EndCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bool bAddColor = false;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLocus : public CMeshfield
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int LOWEST_VALUE = 50;
public:
	CLocus();
	~CLocus() override;

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	void Update(D3DXVECTOR3 TopPos, D3DXVECTOR3 DownPos);//Pos更新処理
	virtual void Draw() override;//描画処理
	void SetLocus(LocusStructure locusStructure);
	void SetMeshTopPos() override;//各頂点の位置の設定
	void SetNormal() override;			//法線設定

private:
	LocusStructure m_LocusData;
	int m_nCarrent;
	int m_nCarrentMax;
	D3DXVECTOR3* m_pTopPosOld;
	D3DXVECTOR3* m_pDownPosOld;
};

#endif
