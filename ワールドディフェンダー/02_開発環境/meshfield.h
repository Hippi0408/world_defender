//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _MESHFIELD_H_			//���̃}�N����`������Ȃ�������
#define _MESHFIELD_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "3dpolygon.h"

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
//���b�V���\����
struct MeshfieldStructure
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXCOLOR col;						//�J���[
	D3DXVECTOR3* pNormalTop;			//���_�@��
	D3DXVECTOR3* pNormalPolygon;		//�ʖ@��
	float fRadiusX; float fRadiusZ;		//�e���c��
	int nMeshX; int nMeshZ;				//�c���̃��b�V�����̐�
	int nPolygon;						//�|���S����
	int nTop;							//���_
	int nIdx;							//�C���f�b�N�X��
	int nTextIndex;
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshfield : public C3DPolygon
{
private:
	static const D3DXVECTOR3 INIT_POS;
public:
	CMeshfield();
	~CMeshfield() override;

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	virtual void Draw() override;//�`�揈��
	virtual void SetMeshfield(MeshfieldStructure meshfieldStructure);
	virtual void SetPolygon();//�X�V�����ȊO�ł̃|���S���̃Z�b�g
	virtual void SetMeshTopPos();//�e���_�̈ʒu�̐ݒ�
	virtual void SetNormal();			//�@���ݒ�
	void SetMeshfieldData(MeshfieldStructure meshfieldStructure);
	void SetIdxBuff();
	int CheckPosLocation(D3DXVECTOR3 pos); //��������Pos���ǂ̃}�X�ɋ��邩��Ԃ�

	LPDIRECT3DINDEXBUFFER9 GetIdxBuff() { return m_pIdxBuff; }//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^�̎擾
	MeshfieldStructure GetMeshfieldData() { return m_MeshfieldData; }

	D3DXVECTOR3 Collision(D3DXVECTOR3 pos);

	int GetMeshX() { return m_MeshfieldData.nMeshX; }
	int GetMeshZ() { return m_MeshfieldData.nMeshZ; }

	int GetMeshfieldNumMax() { return m_nMeshfieldNumMax; }

private:
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	MeshfieldStructure m_MeshfieldData;
	int m_nCntMeshfield;
	int m_nCoolTimeMeshfield;
	int m_nMeshfieldNumMax;

};

#endif
