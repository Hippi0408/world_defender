//=============================================================================
//
// ElasticityPolygon.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "elasticitypolygon.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CElasticityPolygon::CElasticityPolygon()
{
	m_nTextIndex = 0;
	m_pVtxBuff = nullptr;//���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_angleTop = 0.0f;
	m_angleUnder = 0.0f;
	m_lenTop = 0.0f;
	m_lenUnder = 0.0f;
	m_fHeightElasticity = 0.0f;
	m_fHeightMax = 0.0f;
	m_UVSizeMax = D3DXVECTOR2(1.0f, 1.0f);
	m_UVSizeMin = D3DXVECTOR2(0.0f, 0.0f);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CElasticityPolygon::~CElasticityPolygon()
{
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CElasticityPolygon::Init()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	// ���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMax.y);
	pVtx[1].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMax.y);
	pVtx[2].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMin.y);
	pVtx[3].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMin.y);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void CElasticityPolygon::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void CElasticityPolygon::Update()
{
	if (m_nFlashingCnt > 0)
	{
		m_nFlashingCnt--;
		if (!m_bFadeInOut)
		{
			m_Col.a += m_fFadeSpeed;
			if (m_Col.a > 1.0f)
			{
				m_Col.a = 1.0f;
				m_bFadeInOut = true;
			}
		}
		else if (m_bFadeInOut)
		{
			m_Col.a -= m_fFadeSpeed;
			if (m_Col.a < 0.2f)
			{
				m_Col.a = 0.2f;
				m_bFadeInOut = false;
			}
		}

		if (m_nFlashingCnt <= 0)
		{
			m_Col.a = 1.0f;
		}
	}

	SetPolygon();
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void CElasticityPolygon::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, tex);

	//�|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);		//�v���~�e�B�u�i�|���S���j��
}

void CElasticityPolygon::SetHeightElasticity(float fHeightElasticity)
{
	m_fHeightElasticity = fHeightElasticity;
	//�Ίp���̒������Z�o����
	m_lenTop = sqrtf(m_fWidth * m_fWidth + fHeightElasticity * fHeightElasticity);
	//�Ίp���̊p�x���Z�o����
	m_angleTop = atan2f(m_fWidth, fHeightElasticity);
}

void CElasticityPolygon::SetDiagonalLine(float fwidth, float fheight)
{
	m_fWidth = fwidth * 0.5f;
	m_fHeight = fheight * 0.5f;

	//�Ίp���̒������Z�o����
	m_lenTop = sqrtf(m_fWidth * m_fWidth + fheight * fheight);
	//�Ίp���̊p�x���Z�o����
	m_angleTop = atan2f(m_fWidth, fheight);

	//�Ίp���̒������Z�o����
	m_lenUnder = sqrtf(m_fWidth * m_fWidth);
	//�Ίp���̊p�x���Z�o����
	m_angleUnder = atan2f(m_fWidth, 0.0f);

}

void CElasticityPolygon::SetPolygon()
{
	float rot = m_Rot.z;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + (0.0f - D3DX_PI + m_angleTop)) * m_lenTop, m_Pos.y + cosf(rot + (0.0f - D3DX_PI + m_angleTop)) * m_lenTop, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + (0.0f + D3DX_PI - m_angleTop)) * m_lenTop, m_Pos.y + cosf(rot + (0.0f + D3DX_PI - m_angleTop)) * m_lenTop, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + sinf(rot - m_angleUnder) * m_lenUnder, m_Pos.y + cosf(rot - m_angleUnder) * m_lenUnder, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + m_angleUnder) * m_lenUnder, m_Pos.y + cosf(rot + m_angleUnder) * m_lenUnder, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMin.y);
	pVtx[1].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMin.y);
	pVtx[2].tex = D3DXVECTOR2(m_UVSizeMax.x, m_UVSizeMax.y);
	pVtx[3].tex = D3DXVECTOR2(m_UVSizeMin.x, m_UVSizeMax.y);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// �����蔻��
//*****************************************************************************
bool CElasticityPolygon::Collision(D3DXVECTOR3 pos, float radius)
{
	//�������ĂȂ�
	return false;

}