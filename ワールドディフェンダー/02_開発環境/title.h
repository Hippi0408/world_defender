//=============================================================================
//
// title.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _TITLE_H_			//���̃}�N����`������Ȃ�������
#define _TITLE_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;
class C2DPolygon;
class CCamera;
class CLight;
class C3DPolygon;
class CMeshfield;
class CBallast_Manager;
class CMesh_Cylinder;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle : public CObject
{
private:
	enum NEXT_MODE
	{
		MODE_GAME = 0,
		MODE_TUTORIAL,
		MODE_MAX
	};

public:
	CTitle();
	~CTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	CUI *m_pUi; 
	C2DPolygon* m_pGameStart;
	C2DPolygon* m_pTutorial;
	C2DPolygon* m_pTextBackground;
	NEXT_MODE m_Mode;
	bool m_bPress;

	CCamera* m_pCameraRound;
	CLight* m_pLight;
	CMeshfield* m_pMeshfieldBG;
	CBallast_Manager* m_pBallastManager;
	CMesh_Cylinder* m_pMesh_Cylinder;
	D3DXVECTOR3 m_LightVec;

};

#endif