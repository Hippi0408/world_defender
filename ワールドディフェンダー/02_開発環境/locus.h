//=============================================================================
//
//�@�O�Տ��� [locus.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _LOCUS_H_			//���̃}�N����`������Ȃ�������
#define _LOCUS_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "meshfield.h"

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
//�O�Ս\����
struct LocusStructure
{
	D3DXVECTOR3 TopPos;
	D3DXVECTOR3 DownPos;
	int nPolygon;
	int nTextureNum;
	int nSaveInterval;
	D3DXCOLOR BeginningCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR EndCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bool bAddColor = false;
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLocus : public CMeshfield
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int LOWEST_VALUE = 50;
public:
	CLocus();
	~CLocus() override;

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	void Update(D3DXVECTOR3 TopPos, D3DXVECTOR3 DownPos);//Pos�X�V����
	virtual void Draw() override;//�`�揈��
	void SetLocus(LocusStructure locusStructure);
	void SetMeshTopPos() override;//�e���_�̈ʒu�̐ݒ�
	void SetNormal() override;			//�@���ݒ�

private:
	LocusStructure m_LocusData;
	int m_nCarrent;
	int m_nCarrentMax;
	D3DXVECTOR3* m_pTopPosOld;
	D3DXVECTOR3* m_pDownPosOld;
};

#endif
