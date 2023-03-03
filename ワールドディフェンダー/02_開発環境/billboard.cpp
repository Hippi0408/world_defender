//=============================================================================
//
// �r���{�[�h���� [billcoard.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "billboard.h"
#include "manager.h"
#include "texture.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CBillcoard::CBillcoard()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CBillcoard::~CBillcoard()
{
}

//*****************************************************************************
// �`��
//*****************************************************************************
void CBillcoard::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���C�g����
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	//�r���[�}�g���b�N�X�̎擾
	D3DXMATRIX mtxView;
	pD3DDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�J�����̃x�N�g���Ɛ����΂ɂ���
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	//�ʒu�̔��f

	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_3D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, tex);

	//�|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);		//�v���~�e�B�u�i�|���S���j��

				//���C�g�L��
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

void CBillcoard::SetPolygon()
{
	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�//���[�J�����W��
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[1].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[2].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y + m_UVSize.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y + m_UVSize.y);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
