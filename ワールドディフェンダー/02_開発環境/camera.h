//=============================================================================
//
// camera.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _CAMERA_H_			//このマクロ定義がされなかったら
#define _CAMERA_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera : public CObject
{
private:
	
public:
	CCamera();
	virtual ~CCamera() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	void SetCamera();

	//基本情報
	D3DXVECTOR3 GetPosV() { return m_posV; }
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void AddPosV(D3DXVECTOR3 addV) { m_posV += addV; }

	D3DXVECTOR3 GetPosR() { return m_posR; }
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	void AddPosR(D3DXVECTOR3 addR) { m_posR += addR; }

	void SetVecU(D3DXVECTOR3 vecU) { m_vecU = vecU; }
	float GetRot();

	void SetVibration(int nVibrationCntMax, int nVibration);
	bool CheckVibration();

private:
	D3DXVECTOR3 m_posV;		//視点
	D3DXVECTOR3 m_posVDest;		//視点
	D3DXVECTOR3 m_posR;		//注視点
	D3DXVECTOR3 m_posRDest;		//注視点
	D3DXVECTOR3 m_vecU;		//上方向ベクトル
	D3DXMATRIX m_mtxProjection; //プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		  //ビューマトリックス
	float fDistance;			//視点か注視点のまでの距離
	D3DXVECTOR3 m_AddVibration;
	int m_nVibration;
	int m_nVibrationCnt;
	int m_nVibrationCntMax;
};

#endif