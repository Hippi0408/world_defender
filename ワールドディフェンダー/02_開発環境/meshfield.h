//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _MESHFIELD_H_			//このマクロ定義がされなかったら
#define _MESHFIELD_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "3dpolygon.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//メッシュ構造体
struct MeshfieldStructure
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXCOLOR col;						//カラー
	D3DXVECTOR3* pNormalTop;			//頂点法線
	D3DXVECTOR3* pNormalPolygon;		//面法線
	float fRadiusX; float fRadiusZ;		//各横縦幅
	int nMeshX; int nMeshZ;				//縦横のメッシュ化の数
	int nPolygon;						//ポリゴン数
	int nTop;							//頂点
	int nIdx;							//インデックス数
	int nTextIndex;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshfield : public C3DPolygon
{
private:
	static const D3DXVECTOR3 INIT_POS;
public:
	CMeshfield();
	~CMeshfield() override;

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	virtual void Draw() override;//描画処理
	virtual void SetMeshfield(MeshfieldStructure meshfieldStructure);
	virtual void SetPolygon();//更新処理以外でのポリゴンのセット
	virtual void SetMeshTopPos();//各頂点の位置の設定
	virtual void SetNormal();			//法線設定
	void SetMeshfieldData(MeshfieldStructure meshfieldStructure);
	void SetIdxBuff();
	int CheckPosLocation(D3DXVECTOR3 pos); //引く数のPosがどのマスに居るかを返す

	LPDIRECT3DINDEXBUFFER9 GetIdxBuff() { return m_pIdxBuff; }//インデックスバッファへのポインタの取得
	MeshfieldStructure GetMeshfieldData() { return m_MeshfieldData; }

	D3DXVECTOR3 Collision(D3DXVECTOR3 pos);

	int GetMeshX() { return m_MeshfieldData.nMeshX; }
	int GetMeshZ() { return m_MeshfieldData.nMeshZ; }

	int GetMeshfieldNumMax() { return m_nMeshfieldNumMax; }

private:
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
	MeshfieldStructure m_MeshfieldData;
	int m_nCntMeshfield;
	int m_nCoolTimeMeshfield;
	int m_nMeshfieldNumMax;

};

#endif
