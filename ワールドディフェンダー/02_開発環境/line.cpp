//=============================================================================
//
// ���C������ [line.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "line.h"
#include "manager.h"
#include <assert.h>

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CLine::CLine()
{
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CLine::~CLine()
{
}

//*****************************************************************************
//���C��������
//*****************************************************************************
HRESULT CLine::Init(void)
{
	//�}�l�[�W���[���̎擾
	CManager *pManager = GetManager();

	//�����_�[���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	pD3DDevice = pManager->GetDeviceManager();

	//���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_LINE) * LINE_BUFF_MAX,//�m�ۂ��钸�_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuffLine,
		NULL);

	return S_OK;
}

//*****************************************************************************
//���C���̏I������
//*****************************************************************************
void CLine::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuffLine != NULL)
	{
		m_pVtxBuffLine->Release();
		m_pVtxBuffLine = NULL;
	}
}

//*****************************************************************************
//���C���̍X�V����
//*****************************************************************************
void CLine::Update(void)
{

}

//*****************************************************************************
//���C���̕`�揈��
//*****************************************************************************
void CLine::Draw(void)
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//���C�g����
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuffLine, 0, sizeof(VERTEX_LINE));

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_LINE);


	//�|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_LINELIST,		//�v���~�e�B�u�̎��
		0,					//�`�悷��ŏ��̒��_�C���f�b�N�X
		LINE_BUFF_MAX);		//�v���~�e�B�u�i�|���S���j��


	//���C�g�L��
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);



}

//-----------------------------------------------------------------------------
//�Z�b�g���C������
//-----------------------------------------------------------------------------
void CLine::SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXCOLOR col)
{
	VERTEX_LINE *pVtx = NULL;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);


	//��{���̕ۑ�
	m_Pos = pos;
	m_SizeMax = vtxMax;
	m_SizeMin = vtxMin;
	m_Color = col;

	//���_���W�̐ݒ�//���[�J�����W��
	pVtx[0].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	pVtx[1].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	pVtx[2].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	pVtx[3].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);
	pVtx[4].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	pVtx[5].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);
	pVtx[6].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);
	pVtx[7].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);
	pVtx[8].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);
	pVtx[9].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);
	pVtx[10].pos = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);
	pVtx[11].pos = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	pVtx[12].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	pVtx[13].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);
	pVtx[14].pos = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);
	pVtx[15].pos = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);

	//���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < LINE_BUFF_MAX; nCnt++)
	{
		pVtx[nCnt].col = m_Color;
	}
	
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuffLine->Unlock();
}
