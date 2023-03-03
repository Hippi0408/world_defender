//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "meshfield.h" 
#include "manager.h"
#include "texture.h"
#include <assert.h>

const D3DXVECTOR3 CMeshfield::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CMeshfield::CMeshfield()
{
	m_nMeshfieldNumMax = 0;
	m_nCntMeshfield = 0;
	m_nCoolTimeMeshfield = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CMeshfield::~CMeshfield()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CMeshfield::Init()
{
	m_pIdxBuff = nullptr;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	ZeroMemory(&m_MeshfieldData,sizeof(m_MeshfieldData));

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CMeshfield::Uninit()
{
	C3DPolygon::Uninit();

	if (m_MeshfieldData.pNormalPolygon != nullptr)
	{
		delete[] m_MeshfieldData.pNormalPolygon;
		m_MeshfieldData.pNormalPolygon = nullptr;
	}

	if (m_MeshfieldData.pNormalTop != nullptr)
	{
		delete[] m_MeshfieldData.pNormalTop;
		m_MeshfieldData.pNormalTop = nullptr;
	}

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CMeshfield::Update()
{
	m_nCoolTimeMeshfield++;

	if (m_nCoolTimeMeshfield < 0)
	{
		return;
	}

	m_nCoolTimeMeshfield = 0;

	m_nCntMeshfield++;

	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntZ = 0; nCntZ <= m_MeshfieldData.nMeshZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_MeshfieldData.nMeshX; nCntX++)
		{
			float fY = sinf(D3DXToRadian(0.5f) * (nCntX + m_nCntMeshfield)) * 10.0f;

			//���_���W�̐ݒ�//���[�J�����W��
			pVtx[0].pos = D3DXVECTOR3(m_MeshfieldData.fRadiusX * nCntX, fY, -m_MeshfieldData.fRadiusZ * nCntZ);

			pVtx++; //�f�[�^��1���i�߂�
		}
	}
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�@���ݒ�
	SetNormal();

	C3DPolygon::Update();
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CMeshfield::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans,pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_3D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, tex);

	//�|���S���̕`��
	pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,														//�`�悷��ŏ��̒��_�C���f�b�N�X
		0,														//�C���f�b�N�X�̍ŏ��l
		m_MeshfieldData.nTop,									//���_�̐�
		0,														//�C���f�b�N�X�̍ŏ��̐�
		m_MeshfieldData.nPolygon);								//�v���~�e�B�u�i�|���S���j��


}

//*****************************************************************************
// �Z�b�g���b�V���t�B�[���h
//*****************************************************************************
void CMeshfield::SetMeshfield(MeshfieldStructure meshfieldStructure)
{
	//�f�[�^�̐ݒ�
	SetMeshfieldData(meshfieldStructure);

	//�e���_�̈ʒu�̐ݒ�
	SetMeshTopPos();

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	SetIdxBuff();
	

	//�@���ݒ�
	SetNormal();
}

//�Z�b�g�|���S���i���g�����j
void CMeshfield::SetPolygon()
{
	
}

//�e���_�̈ʒu�̐ݒ�
void CMeshfield::SetMeshTopPos()
{
	int nMeshX = m_MeshfieldData.nMeshX;
	int nMeshZ = m_MeshfieldData.nMeshZ;
	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^

								// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
	{
		float fY = 0.0f;
		for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
		{

			//float fY = sinf(D3DXToRadian(0.5f) * (nCntX + m_nCntMeshfield)) * 10.0f;
			//float fY = rand() % 50;

			//���_���W�̐ݒ�//���[�J�����W��
			pVtx[0].pos = D3DXVECTOR3(m_MeshfieldData.fRadiusX * nCntX, fY, -m_MeshfieldData.fRadiusZ * nCntZ);

			//�e���_�̖@���̐ݒ�i�x�N�g���̑傫���͂P�ɂ���K�v������j
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = m_Color;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f / nMeshX  * nCntX, 1.0f / nMeshZ * nCntZ);

			pVtx++; //�f�[�^��1���i�߂�
		}
	}
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//�@���ݒ�
void CMeshfield::SetNormal()
{
	VERTEX_3D *pVtx = nullptr;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nPolygon = 0; nPolygon < m_MeshfieldData.nPolygon; nPolygon++)
	{
		
			D3DXVECTOR3 vec1, vec2,vecResult;
			D3DXVECTOR3 top;

			if (pIdx[0] == pIdx[1]
				|| pIdx[0] == pIdx[2]
				|| pIdx[1] == pIdx[2])
			{
				pIdx++;
				continue;
			}

			vec1 = pVtx[pIdx[1]].pos - pVtx[pIdx[0]].pos;
			vec2 = pVtx[pIdx[2]].pos - pVtx[pIdx[0]].pos;

			//�O��
			if (nPolygon % 2 == 1)
			{
				D3DXVec3Cross(&vecResult, &vec2, &vec1);
			}
			else
			{
				D3DXVec3Cross(&vecResult, &vec1, &vec2);
			}
			//���K��
			D3DXVec3Normalize(&vecResult,&vecResult);
			//���_�̖@���x�N�g���̕ۑ�
			top = vecResult;
			//�ʂ̖@���x�N�g���̕ۑ�
			m_MeshfieldData.pNormalPolygon[nPolygon] = vecResult;
			//�e���_�̖@���x�N�g���̌v�Z
			for (int nTop = 0;nTop < 3;nTop++)
			{
				D3DXVECTOR3 vec = m_MeshfieldData.pNormalTop[pIdx[0 + nTop]];
				vec += top;
				//���K��
				D3DXVec3Normalize(&vec, &vec);
				m_MeshfieldData.pNormalTop[pIdx[0 + nTop]] = vec;
			}
			
			pIdx++;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	for (int nTop = 0; nTop < m_MeshfieldData.nTop; nTop++)
	{
		//�e���_�̖@���̐ݒ�
		pVtx[0].nor = m_MeshfieldData.pNormalTop[nTop];
		pVtx++;
	}


	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//�f�[�^�̐ݒ�
void CMeshfield::SetMeshfieldData(MeshfieldStructure meshfieldStructure)
{
	if (meshfieldStructure.nMeshX <= 0
		|| meshfieldStructure.nMeshZ <= 0)
	{
		assert(false);
	}

	SetPos(meshfieldStructure.pos);
	SetRot(meshfieldStructure.rot);
	SetColor(meshfieldStructure.col);
	m_MeshfieldData.fRadiusX = meshfieldStructure.fRadiusX;
	m_MeshfieldData.fRadiusZ = meshfieldStructure.fRadiusZ;

	int nMeshX = meshfieldStructure.nMeshX;
	int nMeshZ = meshfieldStructure.nMeshZ;

	m_MeshfieldData.nMeshX = nMeshX;
	m_MeshfieldData.nMeshZ = nMeshZ;

	m_nMeshfieldNumMax = nMeshX * nMeshZ;

	m_nTextIndex = meshfieldStructure.nTextIndex;

	m_MeshfieldData.nTop = (nMeshX + 1) * (nMeshZ + 1);
	m_MeshfieldData.nIdx = (nMeshX + 1) * 2 * nMeshZ + (nMeshZ - 1) * 2;
	m_MeshfieldData.nPolygon = nMeshX * nMeshZ * 2 + (nMeshZ - 1) * 4;

	m_MeshfieldData.pNormalTop = new D3DXVECTOR3[m_MeshfieldData.nTop];

	ZeroMemory(m_MeshfieldData.pNormalTop, sizeof(D3DXVECTOR3) * m_MeshfieldData.nTop);
	m_MeshfieldData.pNormalPolygon = new D3DXVECTOR3[m_MeshfieldData.nPolygon];

	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	// ���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshfieldData.nTop,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���_�̃C���f�b�N�X�o�b�t�@�̐���
	pD3DDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshfieldData.nIdx,//�m�ۂ��钸�_�̐�
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

}

//�C���f�b�N�X�o�b�t�@�̐ݒ�
void CMeshfield::SetIdxBuff()
{
	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntZ = 0; nCntZ < m_MeshfieldData.nMeshZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_MeshfieldData.nMeshX; nCntX++)
		{
			//�C���f�b�N�X�̐ݒ�
			pIdx[0] = (WORD)(nCntX + (m_MeshfieldData.nMeshX + 1) * (nCntZ + 1));
			pIdx[1] = (WORD)(nCntX + (m_MeshfieldData.nMeshX + 1) * nCntZ);
			pIdx += 2;//�C���f�b�N�X�̃f�[�^���Q�����߂�
		}

		if (nCntZ < m_MeshfieldData.nMeshZ - 1)
		{
			//�C���f�b�N�X�̐ݒ�
			pIdx[0] = pIdx[-1];//�ЂƂO��pIdx�̒��g������i�ЂƂO�͕K������j
			pIdx[1] = (WORD)((m_MeshfieldData.nMeshX + 1) * (nCntZ + 2));
			pIdx += 2;//�C���f�b�N�X�̃f�[�^���Q�����߂�
		}
	}
	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

}

//��������Pos���ǂ̃}�X�ɋ��邩��Ԃ�
int CMeshfield::CheckPosLocation(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx = nullptr;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nLocation = 0;
	int retval = -1;
	D3DXVECTOR3 vec1, vec2,top0,top1,top2, MeshfieldPos;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2;

	//���b�V���̈ʒu
	MeshfieldPos = GetPos();

	for (int nPolygon = 0; nPolygon < m_MeshfieldData.nPolygon; nPolygon++)
	{
		if (pIdx[0] == pIdx[1]
			|| pIdx[0] == pIdx[2]
			|| pIdx[1] == pIdx[2])
		{
			pIdx++;
			continue;
		}

		top0 = pVtx[pIdx[0]].pos + MeshfieldPos;

		{
			D3DXVECTOR3 CheckDistance = top0 - pos;

			if (D3DXVec3LengthSq(&CheckDistance) > 1500.0f * 1500.0f)
			{
				pIdx++;

				//�}�X�̐��̓|���S��2���łP���i��
				if ((nPolygon + 1) % 2 == 0)
				{
					nLocation++;
				}

				continue;
			}
		}

		top1 = pVtx[pIdx[1]].pos + MeshfieldPos;
		top2 = pVtx[pIdx[2]].pos + MeshfieldPos;

		vec1 = top1 - top0;
		vec2 = pos - top0;

		fInnerProduct0 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = top2 - top1;
		vec2 = pos - top1;

		fInnerProduct1 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = top0 - top2;
		vec2 = pos - top2;

		fInnerProduct2 = vec1.x * vec2.z - vec1.z * vec2.x;

		if (
			(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f) ||
			(fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f)
			)
		{
			retval = nLocation;
			break;
		}

		pIdx++;

		//�}�X�̐��̓|���S��2���łP���i��
		if ((nPolygon + 1) % 2 == 0)
		{
			nLocation++;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�G���[���� -1
	return retval;
}

D3DXVECTOR3 CMeshfield::Collision(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx = nullptr;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nPolygon = 0; nPolygon < m_MeshfieldData.nPolygon; nPolygon++)
	{
		if (pIdx[0] == pIdx[1]
			|| pIdx[0] == pIdx[2]
			|| pIdx[1] == pIdx[2])
		{
			pIdx++;
			continue;
		}

		D3DXVECTOR3 vec1, vec2;
		float fInnerProduct0 , fInnerProduct1, fInnerProduct2;

		vec1 = pVtx[pIdx[1]].pos - pVtx[pIdx[0]].pos;
		vec2 = pos - pVtx[pIdx[0]].pos;

		fInnerProduct0 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = pVtx[pIdx[2]].pos - pVtx[pIdx[1]].pos;
		vec2 = pos - pVtx[pIdx[1]].pos;

		fInnerProduct1 = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = pVtx[pIdx[0]].pos - pVtx[pIdx[2]].pos;
		vec2 = pos - pVtx[pIdx[2]].pos;

		fInnerProduct2 = vec1.x * vec2.z - vec1.z * vec2.x;

		if (
			(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f)
			|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f)
			)
		{
			D3DXVECTOR3 P1 = pVtx[pIdx[0]].pos;
			D3DXVECTOR3 vec = m_MeshfieldData.pNormalPolygon[nPolygon];

			float fPolygonY = P1.y - ((pos.x - P1.x) * vec.x + (pos.z - P1.z) * vec.z) / vec.y;

			//�C���f�b�N�X�o�b�t�@���A�����b�N
			m_pIdxBuff->Unlock();
			//���_�o�b�t�@���A�����b�N
			m_pVtxBuff->Unlock();

			return D3DXVECTOR3(pos.x, fPolygonY, pos.z);
		}

		pIdx++;

	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return D3DXVECTOR3(0.0f,0.0f,0.0f);
}
