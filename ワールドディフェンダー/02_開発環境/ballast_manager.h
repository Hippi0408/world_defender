//=============================================================================
//
//　瓦礫の管理者処理 [ballast_manager.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _BALLAST_MANAGER_H_			//このマクロ定義がされなかったら
#define _BALLAST_MANAGER_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "object.h"
#include <map>
#include <list>
#include "3dobject.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBallast;
class CMeshfield;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBallast_Manager : public CObject
{
private:
	static const float MAP_MAX;
	static const D3DXVECTOR3 INIT_POS;
	static const float BASE_RADIUS_PER_RUBBLE_ONE;
	static const int BALLAST_PATTERN = 5;
public:
	CBallast_Manager();
	~CBallast_Manager() override;

	HRESULT Init() override;//初期化
	void Uninit() override;//終了処理
	void Update() override;//更新処理
	void Draw() override;//描画処理

	//プレイヤーの位置のSet、Get
	void SetPlListNumber(int nNumber) { m_nPlListNumber = nNumber; }
	int GetPlListNumber() { return m_nPlListNumber; }

	//瓦礫の設定
	void SetBallast(int nNumber, Object_Data Data);

	//メッシュフィールドの設定
	void MeshfieldSet(CMeshfield* pMeshfield);

	//サイコキネシスエリアにあったらそのオブジェクトのポインタを返す
	CBallast* CheckCircleCollision(D3DXVECTOR3 pos, float fRadius, int nAdrenaline);

	//サイコキネシスエリアにあったらそのオブジェクトの色を変える
	void WithinRangeColor(int nMapGrid, D3DXVECTOR3 pos, float fRadius, int nAdrenaline);

	//障害物の当たり判定
	D3DXVECTOR3 CollisionBallast(int nMapGrid, D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min);

	//エネミーとの当たり判定
	void CollisionEnemy();

	//浮遊状態のリスト追加
	void SetFloatingBallst(CBallast* pBallast);

	//後天的瓦礫の生成(引数は飛ばしたい方向ベクトル,あとは基本情報)
	void SetBallastAcquired(D3DXVECTOR3 vec,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius);

	//リスト内の入れ替え
	void ReplacementList(CBallast* pBallast, int nNext);

	//ライトの方向VecのSet,Get
	void SetLight(D3DXVECTOR3 vec) { m_Light = vec; }
	D3DXVECTOR3 GetLight() { return m_Light; }

	//リストの個数分確保
	void CreateBallastListData(int nDataNum);

private:
	std::list<CBallast*>* m_pBallastListData;		//マップ上のオブジェクトをリストで管理する変数
	int m_nBallastListDataMax;						//上記の変数のMax
	std::list<CBallast*> m_FloatingBallstList;					//浮遊状態の瓦礫
	int m_nPlListNumber;
	CMeshfield *m_pMeshfieldCopy;								//メッシュフィールドポインタのコピー（このコピーは解放をしない）
	int m_nMeshfieldNumMax;										//メッシュのマスの数の最大
	int m_nBallast_Acquired_Model[BALLAST_PATTERN];
	D3DXVECTOR3 m_Light;		//ライトの向き
};

#endif
