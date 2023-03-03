//=============================================================================
//
// �O�Տ��� [mesh_cylinder.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "mesh_cylinder.h" 
#include "manager.h"

const D3DXVECTOR3 CMesh_Cylinder::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CMesh_Cylinder::CMesh_Cylinder()
{
	
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CMesh_Cylinder::~CMesh_Cylinder()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CMesh_Cylinder::Init()
{
	m_MeshRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//�\���̏�����
	ZeroMemory(&m_Mesh_Cylinder_Structure, sizeof(m_Mesh_Cylinder_Structure));
	
	//�e�N���X�̏�����
	if (FAILED(CMeshfield::Init()))
	{
		return -1;
	}


	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CMesh_Cylinder::Uninit()
{
	//�e�N���X�̏I������
	CMeshfield::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CMesh_Cylinder::Update()
{

}

//*****************************************************************************
// �X�V����(�I�[�o�[���[�hPos�X�V����)
//*****************************************************************************
void CMesh_Cylinder::Update(D3DXVECTOR3 Pos)
{
	//���݂̈ʒu�̍X�V
	SetPos(Pos);

	//�e���_�̍X�V
	SetMeshTopPos();
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CMesh_Cylinder::Draw()
{
	//�}�l�[�W���[���̎擾
	CManager *pManager = GetManager();

	//�����_�[���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	pD3DDevice = pManager->GetDeviceManager();

	//�J�����O�̃I�t
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�`��
	CMeshfield::Draw();

	//�J�����O�̃I��
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//*****************************************************************************
// �O�Ղ̐ݒ�
//*****************************************************************************
void CMesh_Cylinder::SetMesh_Cylinder(Mesh_Cylinder_Structure mesh_cylinder_structure)
{
	//RotMove
	m_Mesh_Cylinder_Structure.fRotMove = mesh_cylinder_structure.fRotMove;

	//���a
	m_Mesh_Cylinder_Structure.fRadius = mesh_cylinder_structure.fRadius;

	//Y�̃T�C�Y�ő�
	m_Mesh_Cylinder_Structure.fSizeYTop = mesh_cylinder_structure.fSizeYTop;

	//�e���_
	m_Mesh_Cylinder_Structure.ParentPos = mesh_cylinder_structure.ParentPos;

	//�J���[������
	m_Mesh_Cylinder_Structure.nAttenuationFrame = mesh_cylinder_structure.nAttenuationFrame;

	//�J���[
	m_Mesh_Cylinder_Structure.ColorMax = mesh_cylinder_structure.ColorMax;
	m_Mesh_Cylinder_Structure.ColorLowest = mesh_cylinder_structure.ColorLowest;

	m_Mesh_Cylinder_Structure.bFade = mesh_cylinder_structure.bFade;
	if (m_Mesh_Cylinder_Structure.bFade)
	{
		m_AddColor = (m_Mesh_Cylinder_Structure.ColorLowest - m_Mesh_Cylinder_Structure.ColorMax) / (float)m_Mesh_Cylinder_Structure.nAttenuationFrame;
	}
	
	m_Color = m_Mesh_Cylinder_Structure.ColorMax;

	//�����ŕ`�悷�邩
	m_Mesh_Cylinder_Structure.nPolygonX = mesh_cylinder_structure.nPolygonY;
	m_Mesh_Cylinder_Structure.nPolygonY = mesh_cylinder_structure.nPolygonX;

	//�e�N�X�`���ԍ�
	m_Mesh_Cylinder_Structure.nTextureNum = mesh_cylinder_structure.nTextureNum;

	
	


	MeshfieldStructure MeshData;
	ZeroMemory(&MeshData, sizeof(MeshData));
	MeshData.pos = mesh_cylinder_structure.ParentPos;
	MeshData.rot = D3DXVECTOR3(D3DXToRadian(0), 0.0f, 0.0f);
	MeshData.col = mesh_cylinder_structure.ColorMax;
	MeshData.fRadiusX = 0.0f;
	MeshData.fRadiusZ = 0.0f;
	MeshData.nMeshX = m_Mesh_Cylinder_Structure.nPolygonX;
	MeshData.nMeshZ = m_Mesh_Cylinder_Structure.nPolygonY;
	MeshData.nTextIndex = m_Mesh_Cylinder_Structure.nTextureNum;


	//�f�[�^�̐ݒ�
	SetMeshfieldData(MeshData);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	SetIdxBuff();

	//�@��
	SetNormal();
}

//*****************************************************************************
// ���b�V���̊e���_�̈ʒu�̐ݒ�
//*****************************************************************************
void CMesh_Cylinder::SetMeshTopPos()
{
	//X�̒��_��
	int nTopX = m_Mesh_Cylinder_Structure.nPolygonY;

	//�p�x�ϐ��錾
	
	float fRot, fRotAdd;
	fRot = m_MeshRot.z;
	m_MeshRot.z += m_Mesh_Cylinder_Structure.fRotMove;
	fRotAdd = D3DXToRadian(360) / nTopX;
	
	//SizeY�ϐ��錾
	float fSizeY, fSizeYAdd;
	//���݂̍���
	fSizeY = m_Mesh_Cylinder_Structure.ParentPos.y;
	//���Z���Ă����l
	fSizeYAdd = (m_Mesh_Cylinder_Structure.fSizeYTop) / (m_Mesh_Cylinder_Structure.nPolygonY);


	// �J���[�̐ݒ�
	if (m_Mesh_Cylinder_Structure.bFade)
	{
		ChangeColor();
	}

	//�e���b�V���̐�
	int nMeshZ = GetMeshfieldData().nMeshZ;
	int nMeshX = GetMeshfieldData().nMeshX;

	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
	{
		//���݂̍���
		fSizeY = m_Mesh_Cylinder_Structure.ParentPos.y;
		//���Z���Ă����l
		fSizeYAdd = (m_Mesh_Cylinder_Structure.fSizeYTop) / (m_Mesh_Cylinder_Structure.nPolygonX);


		D3DXVECTOR3 pos;

		pos.x = m_Mesh_Cylinder_Structure.fRadius * cosf(fRot);
		pos.z = m_Mesh_Cylinder_Structure.fRadius * sinf(fRot);

		
		
		for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
		{

			pos.y = fSizeY;

			//���_���W�̐ݒ�//���[�J�����W��
			pVtx[0].pos = pos;

			//�e���_�̖@���̐ݒ�i�x�N�g���̑傫���͂P�ɂ���K�v������j
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = m_Color;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f / nMeshZ  * nCntZ, -1.0f / nMeshX * nCntX);

			pVtx++; //�f�[�^��1���i�߂�


			fSizeY += fSizeYAdd;

			

		}

		fRot += fRotAdd;

	}
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// �@���̐ݒ�
//*****************************************************************************
void CMesh_Cylinder::SetNormal()
{
	VERTEX_3D *pVtx = nullptr;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̎擾
	int nTopMax = GetMeshfieldData().nTop;

	//�e���_�̖@���̐ݒ�
	for (int nTop = 0; nTop < nTopMax; nTop++)
	{
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// �J���[�̐ݒ�
//*****************************************************************************
void CMesh_Cylinder::ChangeColor()
{
	m_nAttenuationFrameCnt++;
	m_Color += m_AddColor;

	if (m_Color.r >= 1.0f)
	{
		m_Color.r = 1.0f;
	}
	else if (m_Color.r <= 0.0f)
	{
		m_Color.r = 0.0f;
	}

	if (m_Color.g >= 1.0f)
	{
		m_Color.g = 1.0f;
	}
	else if (m_Color.g <= 0.0f)
	{
		m_Color.g = 0.0f;
	}

	if (m_Color.b >= 1.0f)
	{
		m_Color.b = 1.0f;
	}
	else if (m_Color.b <= 0.0f)
	{
		m_Color.b = 0.0f;
	}

	if (m_Color.a >= 1.0f)
	{
		m_Color.a = 1.0f;
	}
	else if (m_Color.a <= 0.0f)
	{
		m_Color.a = 0.0f;
	}

	if (m_nAttenuationFrameCnt >= m_Mesh_Cylinder_Structure.nAttenuationFrame)
	{
		m_AddColor = -1.0f * m_AddColor;
		m_nAttenuationFrameCnt = 0;
	}

}


