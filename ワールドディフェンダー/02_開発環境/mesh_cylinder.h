//=============================================================================
//
//�@���b�V���~������ [mesh_cylinder.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _MESH_CYLINDER_H_			//���̃}�N����`������Ȃ�������
#define _MESH_CYLINDER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "meshfield.h"

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
//�O�Ս\����
struct Mesh_Cylinder_Structure
{
	D3DXVECTOR3 ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR ColorMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR ColorLowest = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	int nTextureNum = 0;
	int nPolygonX = 1;
	int nPolygonY = 1;
	float fRadius = 0.0f;
	float fSizeYTop = 0.0f;
	int nAttenuationFrame = 1;
	float fRotMove = 0.0f;
	bool bFade = false;
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMesh_Cylinder : public CMeshfield
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int MESH_CYLINDER_VALUE = 50;
public:
	CMesh_Cylinder();
	~CMesh_Cylinder() override;

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	void Update(D3DXVECTOR3 Pos);//Pos�X�V����
	virtual void Draw() override;//�`�揈��
	void SetMesh_Cylinder(Mesh_Cylinder_Structure mesh_cylinder_structure);
	void SetMeshTopPos() override;//�e���_�̈ʒu�̐ݒ�
	void SetNormal() override;			//�@���ݒ�
	void ChangeColor();
	void SetRadius(float fRadius) { m_Mesh_Cylinder_Structure.fRadius = fRadius; }
	float GetRadius() { return  m_Mesh_Cylinder_Structure.fRadius; }
	void SetSizeTop(float fSizeTop) { m_Mesh_Cylinder_Structure.fSizeYTop = fSizeTop; }
	float GetSizeTop() { return m_Mesh_Cylinder_Structure.fSizeYTop; }

private:
	Mesh_Cylinder_Structure m_Mesh_Cylinder_Structure;
	D3DXVECTOR3 m_MeshRot;
	bool m_bColorChange;
	D3DXCOLOR m_AddColor;
	int m_nAttenuationFrameCnt;
};

#endif
