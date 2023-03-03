//=============================================================================
//
//�@�T�C�R�L�l�V�X�G���A���� [psychokinesis_area.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _PSYCHOKINESIS_AREA_H_			//���̃}�N����`������Ȃ�������
#define _PSYCHOKINESIS_AREA_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMesh_Cylinder;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPsychokinesis_Area : public CObject
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const float HEIGHT_LOCUS; 
	static const float RADIUS_INIT;
	static const float RADIUS_MAX;
	static const float RADIUS_MOVE;
	static const float SIZE_TOP;
public:
	CPsychokinesis_Area();
	~CPsychokinesis_Area() override;

	HRESULT Init() override;//������
	HRESULT Init(D3DXVECTOR3 Pos) ;//������(Pos����)
	void Uninit() override;//�I������
	void Update() override;//�X�V����
	void Update(D3DXVECTOR3 Pos);//Pos�X�V����
	void Draw() override;//�`�揈��

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius() { return m_fRadius + RADIUS_INIT; }
	float GetSizeTop() { return m_fSizeTop; }
	
	void RateCalculation(float fRate);

private:
	D3DXVECTOR3 m_PLPos;
	CMesh_Cylinder* m_pMesh_Cylinder;
	float m_fRadius;
	float m_fSizeTop;
};

#endif
