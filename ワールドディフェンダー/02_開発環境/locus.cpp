//=============================================================================
//
// �O�Տ��� [locus.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "locus.h" 
#include "manager.h"


const D3DXVECTOR3 CLocus::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CLocus::CLocus()
{
	//�e�|�C���^�̏�����
	m_pTopPosOld = nullptr;
	m_pDownPosOld = nullptr;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CLocus::~CLocus()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CLocus::Init()
{
	//�O�Ս\���̏�����
	ZeroMemory(&m_LocusData, sizeof(m_LocusData));
	m_nCarrent = 0;

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
void CLocus::Uninit()
{
	//TopPos�̉��
	if (m_pTopPosOld != nullptr)
	{
		delete[] m_pTopPosOld;
		m_pTopPosOld = nullptr;
	}

	//DownPos�̉��
	if (m_pDownPosOld != nullptr)
	{
		delete[] m_pDownPosOld;
		m_pDownPosOld = nullptr;
	}

	//�e�N���X�̏I������
	CMeshfield::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CLocus::Update()
{
	
}

//*****************************************************************************
// �X�V����(�I�[�o�[���[�hPos�X�V����)
//*****************************************************************************
void CLocus::Update(D3DXVECTOR3 TopPos, D3DXVECTOR3 DownPos)
{
	
	//���݂̈ʒu�̍X�V
	m_LocusData.TopPos = TopPos;
	m_LocusData.DownPos = DownPos;

	//Old�̔z��X�V
	m_pTopPosOld[m_nCarrent] = TopPos;
	m_pDownPosOld[m_nCarrent] = DownPos;

	//�e���_�̍X�V
	SetMeshTopPos();

	//�ۑ����ύX
	m_nCarrent++;

	//�ۑ���̔ԍ����l�p�̐��𒴂�����
	if (m_nCarrent >= m_nCarrentMax)
	{
		m_nCarrent = 0;
	}

	
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CLocus::Draw()
{
	//�}�l�[�W���[���̎擾
	CManager *pManager = GetManager();

	//�����_�[���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	pD3DDevice = pManager->GetDeviceManager();

	//�J�����O�̃I�t
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���u�����f�B���O�����Z�����ɐݒ�
	if (m_LocusData.bAddColor)
	{
		//���u�����f�B���O�����Z�����ɐݒ�
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//�`��
	CMeshfield::Draw();

	if (m_LocusData.bAddColor)
	{
		//���u�����f�B���O�����̖߂�
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	//�J�����O�̃I��
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//*****************************************************************************
// �O�Ղ̐ݒ�
//*****************************************************************************
void CLocus::SetLocus(LocusStructure locusStructure)
{
	//�㒸�_
	m_LocusData.TopPos = locusStructure.TopPos;

	//�����_
	m_LocusData.DownPos = locusStructure.DownPos;

	//�n�߂̃J���[
	m_LocusData.BeginningCol = locusStructure.BeginningCol;

	//�I���̃J���[
	m_LocusData.EndCol = locusStructure.EndCol;

	//���Z�������s�����ǂ���
	m_LocusData.bAddColor = locusStructure.bAddColor;

	//�����ŕ`�悷�邩
	m_LocusData.nPolygon = locusStructure.nPolygon;

	//�e�N�X�`���ԍ�
	m_LocusData.nTextureNum = locusStructure.nTextureNum;

	//�ۑ�����Ԋu
	m_LocusData.nSaveInterval = locusStructure.nSaveInterval;

	//OldPos�̕ۑ��\��
	m_nCarrentMax = LOWEST_VALUE + locusStructure.nSaveInterval * locusStructure.nPolygon;

	//�㒸�_�̃������[�̊m��
	m_pTopPosOld = new D3DXVECTOR3[m_nCarrentMax];
	//�����_�̃������[�̊m��
	m_pDownPosOld = new D3DXVECTOR3[m_nCarrentMax];

	//���̏�����
	for (int nCnt = 0; nCnt < m_nCarrentMax; nCnt++)
	{
		m_pTopPosOld[nCnt] = locusStructure.TopPos;
		m_pDownPosOld[nCnt] = locusStructure.DownPos;
	}

	MeshfieldStructure MeshData;
	ZeroMemory(&MeshData, sizeof(MeshData));
	MeshData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MeshData.rot = D3DXVECTOR3(D3DXToRadian(0), 0.0f, 0.0f);
	MeshData.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	MeshData.fRadiusX = 0.0f;
	MeshData.fRadiusZ = 0.0f;
	MeshData.nMeshX = 1;
	MeshData.nMeshZ = locusStructure.nPolygon;
	MeshData.nTextIndex = locusStructure.nTextureNum;

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
void CLocus::SetMeshTopPos()
{
	int nMeshX = GetMeshfieldData().nMeshX;
	int nMeshZ = GetMeshfieldData().nMeshZ;

	//�J���[�̐ݒ�
	D3DXCOLOR col = m_LocusData.BeginningCol;

	D3DXCOLOR addcol = m_LocusData.EndCol - m_LocusData.BeginningCol;

	float fRatio = (float)(nMeshZ + 1);

	addcol = addcol / fRatio;

	VERTEX_3D *pVtx = nullptr;		//���_���ւ̃|�C���^

	int nCarrent = m_nCarrent;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt <= nMeshZ; nCnt++)
	{
		

		D3DXVECTOR3 TopPosOld = m_pTopPosOld[nCarrent];
		D3DXVECTOR3 DownPosOld = m_pDownPosOld[nCarrent];

		//���_���W�̐ݒ�//���[�J�����W��
		pVtx[0].pos = TopPosOld;
		pVtx[1].pos = DownPosOld;

		col += addcol;

		//���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(1.0f / nMeshX  * nCnt, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / nMeshX  * nCnt, 1.0f);


		nCarrent = m_nCarrent - (m_LocusData.nSaveInterval + m_LocusData.nSaveInterval * nCnt);
		if (nCarrent < 0)
		{
			nCarrent = m_nCarrentMax + nCarrent;
		}

		pVtx += 2; //�f�[�^��2���i�߂�

	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//*****************************************************************************
// �@���̐ݒ�
//*****************************************************************************
void CLocus::SetNormal()
{
	VERTEX_3D *pVtx = nullptr;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̎擾
	int nTopMax = GetMeshfieldData().nTop;

	//�e���_�̖@���̐ݒ�
	for (int nTop = 0; nTop < nTopMax; nTop++)
	{
		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}


