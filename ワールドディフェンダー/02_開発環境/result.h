//=============================================================================
//
// Result.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _RESULT_H_			//���̃}�N����`������Ȃ�������
#define _RESULT_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CCamera;
class CLight;
class C2DPolygon;
class CMeshfield;
class CBallast_Manager;
class CMesh_Cylinder;
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CObject
{
private:
	static const float SCORE_SIZE;
public:
	CResult();
	~CResult() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	CCamera* m_pCameraRound;
	CLight* m_pLight;
	CMeshfield* m_pMeshfieldBG;
	CBallast_Manager* m_pBallastManager;
	CMesh_Cylinder* m_pMesh_Cylinder;
	D3DXVECTOR3 m_LightVec;

	CNumber* m_pNewScore;
	CNumber* m_pOldScore;

	C2DPolygon* m_pNew;
	C2DPolygon* m_pOld;

	C2DPolygon* m_pRankingName;

};

#endif