//=============================================================================
//
// �O�Տ��� [psychokinesis_area.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "psychokinesis_area.h" 
#include "texture.h"
#include <assert.h>
#include "convenience_function.h"
#include "mesh_cylinder.h"
#include "input.h"

const D3DXVECTOR3 CPsychokinesis_Area::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const float CPsychokinesis_Area::HEIGHT_LOCUS = 80.0f;
const float CPsychokinesis_Area::RADIUS_INIT = 200.0f;
const float CPsychokinesis_Area::RADIUS_MAX = 1600.0f;
const float CPsychokinesis_Area::SIZE_TOP = 50.0f;
const float CPsychokinesis_Area::RADIUS_MOVE = D3DXToRadian(5);
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPsychokinesis_Area::CPsychokinesis_Area()
{
	m_PLPos = INIT_POS;
	m_fRadius = 0.0f;
	m_fSizeTop = SIZE_TOP;
	m_pMesh_Cylinder = nullptr;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPsychokinesis_Area::~CPsychokinesis_Area()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CPsychokinesis_Area::Init()
{
	//Num�`�F�b�N
	if (m_pMesh_Cylinder != nullptr)
	{
		assert(false);
	}

	//���̊m��
	m_pMesh_Cylinder = new CMesh_Cylinder;

	//������
	if (FAILED(m_pMesh_Cylinder->Init()))
	{
		return -1;
	}

	Mesh_Cylinder_Structure Mesh_Cylinder_Structure;

	Mesh_Cylinder_Structure.fRadius = RADIUS_INIT;
	Mesh_Cylinder_Structure.fSizeYTop = m_fSizeTop;
	Mesh_Cylinder_Structure.nPolygonX = 30;
	Mesh_Cylinder_Structure.nPolygonY = 1;
	Mesh_Cylinder_Structure.nTextureNum = CTexture::LoadTexture("data/TEXTURE/�O��.png");
	Mesh_Cylinder_Structure.ParentPos = m_PLPos;
	Mesh_Cylinder_Structure.ColorMax = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
	Mesh_Cylinder_Structure.ColorLowest = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);
	Mesh_Cylinder_Structure.nAttenuationFrame = 120;
	Mesh_Cylinder_Structure.bFade = true;
	Mesh_Cylinder_Structure.fRotMove = D3DXToRadian(1);

	m_pMesh_Cylinder->SetMesh_Cylinder(Mesh_Cylinder_Structure);


	return S_OK;
}

//*****************************************************************************
// ������(Pos����)
//*****************************************************************************
HRESULT CPsychokinesis_Area::Init(D3DXVECTOR3 Pos)
{
	//�v���C���[�̈ʒu�̕ۊ�
	m_PLPos = Pos;

	//������
	if (FAILED(Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CPsychokinesis_Area::Uninit()
{
	//�����O�̏��̉��
	if (m_pMesh_Cylinder != nullptr)
	{
		//�I������
		m_pMesh_Cylinder->Uninit();
		delete m_pMesh_Cylinder;
		m_pMesh_Cylinder = nullptr;
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CPsychokinesis_Area::Update()
{
	m_pMesh_Cylinder->SetRadius(RADIUS_INIT + m_fRadius);
	m_pMesh_Cylinder->SetSizeTop(m_fSizeTop);

	m_pMesh_Cylinder->Update(m_PLPos);
}

//*****************************************************************************
// �X�V����(�I�[�o�[���[�hPos�X�V����)
//*****************************************************************************
void CPsychokinesis_Area::Update(D3DXVECTOR3 Pos)
{
	//�v���C���[�̈ʒu�̍X�V
	m_PLPos = Pos;

	//�X�V����
	Update();
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CPsychokinesis_Area::Draw()
{
	//�`�揈��
	m_pMesh_Cylinder->Draw();
}

//*****************************************************************************
// �����v�Z����
//*****************************************************************************
void CPsychokinesis_Area::RateCalculation(float fRate)
{
	m_fRadius = RADIUS_MAX * fRate;
}

