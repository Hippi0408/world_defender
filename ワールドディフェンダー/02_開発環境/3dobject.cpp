//=============================================================================
//
// 3dobject.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "3dobject.h"
#include "manager.h"
#include "object_type_list.h"
#include <assert.h>

#ifdef _DEBUG
#include "line.h"
#endif // _DEBUG

C3DObject::ModelPattern C3DObject::m_ModelPattern[MODEL_PATTERN_MAX] = {};
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
C3DObject::C3DObject()
{
	ZeroMemory(&m_Model, sizeof(m_Model));
	m_fSize = 1.0f;
	m_Object_Type_List = OBJ_OTHER;
	m_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_ColorChange = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ColorMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ColorMin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fAttenuationColor = 0.0f;
	m_bColor = false;
	m_pDefaultColor = nullptr;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
C3DObject::~C3DObject()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT C3DObject::Init()
{
	ZeroMemory(&m_Model, sizeof(m_Model));
	m_fSize = 1.0f;

#ifdef _DEBUG
	m_pLine = nullptr;
#endif // _DEBUG

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void C3DObject::Uninit()
{
	if (m_Model.pTopPos != nullptr)
	{
		delete[] m_Model.pTopPos;
		m_Model.pTopPos = nullptr;
	}

	if(m_Model.pNormalPolygon != nullptr)
	{
		delete[] m_Model.pNormalPolygon;
		m_Model.pNormalPolygon = nullptr;
	}

	if (m_pDefaultColor != nullptr)
	{
		delete[] m_pDefaultColor;
		m_pDefaultColor = nullptr;
	}

#ifdef _DEBUG
	if (m_pLine != nullptr)
	{
		delete m_pLine;
		m_pLine = nullptr;
	}
#endif // _DEBUG

}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void C3DObject::Update()
{
	m_Color += m_ColorChange * m_fAttenuationColor;

	//r
	if (m_Color.r >= m_ColorMax.r)
	{
		m_Color.r = m_ColorMax.r;
		m_ColorChange.r *= -1.0f;
	}
	else if (m_Color.r <= m_ColorMin.r)
	{
		m_Color.r = m_ColorMin.r;
		m_ColorChange.r *= -1.0f;
	}

	//g
	if (m_Color.g >= m_ColorMax.g)
	{
		m_Color.g = m_ColorMax.g;
		m_ColorChange.g *= -1.0f;
	}
	else if (m_Color.g <= m_ColorMin.g)
	{
		m_Color.g = m_ColorMin.g;
		m_ColorChange.g *= -1.0f;
	}

	//b
	if (m_Color.b >= m_ColorMax.b)
	{
		m_Color.b = m_ColorMax.b;
		m_ColorChange.b *= -1.0f;
	}
	else if (m_Color.b <= m_ColorMin.b)
	{
		m_Color.b = m_ColorMin.b;
		m_ColorChange.b *= -1.0f;
	}

	//a
	if (m_Color.a >= m_ColorMax.a)
	{
		m_Color.a = m_ColorMax.a;
		m_ColorChange.a *= -1.0f;
	}
	else if (m_Color.a <= m_ColorMin.a)
	{
		m_Color.a = m_ColorMin.a;
		m_ColorChange.a *= -1.0f;
	}


	
	
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void C3DObject::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	bool bShadow = false;
	//�e������
	if (m_LightVec != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		bShadow = true;
		D3DXMATRIX mtxShadow;
		D3DXPLANE pIaneField;
		D3DXVECTOR4 vecLight;
		D3DXVECTOR3 pos, normal;

		pos = m_ShadowPos;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		vecLight = D3DXVECTOR4(m_LightVec.x, m_LightVec.y, m_LightVec.z,0.0f) * -1.0f;

		D3DXPlaneFromPointNormal(&pIaneField, &pos, &normal);

		D3DXMatrixShadow(&mtxShadow, &vecLight, &pIaneField);

		D3DXMatrixMultiply(&mtxShadow, &m_Model.mtxWorld, &mtxShadow);

		//���[���h�}�g���b�N�X�̐ݒ�
		pD3DDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//���݂̃}�e���A����ۑ�
		pD3DDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

		//�e�N�X�`���̐ݒ�
		pD3DDevice->SetTexture(0, NULL);

		for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
		{

			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			//�}�e���A���̐ݒ�
			pD3DDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pD3DDevice->SetTexture(0, m_ModelPattern[m_Model.nPattn].pTexture[nCntMat]);

			//���f���p�[�c�̕`��
			m_ModelPattern[m_Model.nPattn].pMeshModel->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pD3DDevice->SetMaterial(&matDef);

	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_Model.mtxWorld);

	//���݂̃}�e���A����ۑ�
	pD3DDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, NULL);

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
	{
		
		if (m_bColor)
		{
			pMat[nCntMat].MatD3D.Diffuse = m_Color;
		}
		else
		{
			pMat[nCntMat].MatD3D.Diffuse = m_pDefaultColor[nCntMat];
		}

		//�}�e���A���̐ݒ�
		pD3DDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pD3DDevice->SetTexture(0, m_ModelPattern[m_Model.nPattn].pTexture[nCntMat]);

		//���f���p�[�c�̕`��
		m_ModelPattern[m_Model.nPattn].pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pD3DDevice->SetMaterial(&matDef);


	if (m_Model.pNormalPolygon == nullptr)
	{
		//�@���̐ݒ�
		SetNormal();
	}
	else
	{
		//�@���̍X�V
		UpdateNormal();
	}
	
#ifdef _DEBUG

	//���C���̃��[���h�}�g���b�N�X�̐ݒ�
	m_pLine->SetMtxWorld(m_Model.mtxWorld);

	//���C���̕`��
	m_pLine->Draw();

#endif // _DEBUG]

}

//*****************************************************************************
// �RD�I�u�W�F�N�g�̔z�u
//*****************************************************************************
void C3DObject::Set3DObject(int nPattn, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nNumVix;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	m_Model.pos = pos;
	m_Model.rot = rot;
	m_Model.nPattn = nPattn;

	//�f�t�H���g�J���[�̕ۑ�
	m_pDefaultColor = new D3DCOLORVALUE[m_ModelPattern[m_Model.nPattn].nNumMatModel];

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;				
	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
	{
		m_pDefaultColor[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
	}


	//���f���̃T�C�Y�̔�r�p�����l
	m_Model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_Model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//���_���̎擾
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�擾
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_Model.nPattn].pMeshModel->GetFVF());

	//���_���b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���ׂĂ̒��_POS�̎擾
	for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (m_Model.vtxMax.x < vtx.x)
		{//X
			m_Model.vtxMax.x = vtx.x;
		}
		if (m_Model.vtxMin.x > vtx.x)
		{
			m_Model.vtxMin.x = vtx.x;
		}

		if (m_Model.vtxMax.y < vtx.y)
		{//Y
			m_Model.vtxMax.y = vtx.y;
		}
		if (m_Model.vtxMin.y > vtx.y)
		{
			m_Model.vtxMin.y = vtx.y;
		}

		if (m_Model.vtxMax.z < vtx.z)
		{//Z
			m_Model.vtxMax.z = vtx.z;
		}
		if (m_Model.vtxMin.z > vtx.z)
		{
			m_Model.vtxMin.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockVertexBuffer();

#ifdef _DEBUG

	//���C���𐶐�
	m_pLine = new CLine;

	//������
	if (m_pLine->Init())
	{
		assert(false);
	}

	//���C���̏��̐ݒ�
	m_pLine->SetLine(m_Model.posParent, m_Model.vtxMax, m_Model.vtxMin, D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));

#endif // _DEBUG]

}

//*****************************************************************************
// �}�g���b�N�X�̌v�Z
//*****************************************************************************
void C3DObject::CalculationMatrix()
{
	
	D3DXMATRIX mtxRoot;					//���̐e�̃��[���h�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRotParent, mtxRotChild;

	//���[���h�}�g���b�N�X�̏������i���̐e�j
	D3DXMatrixIdentity(&mtxRoot);

	//�T�C�Y�̕ύX
	D3DXMatrixScaling(&mtxScaling, m_fSize, m_fSize, m_fSize);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxScaling);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Model.rotParent.y, m_Model.rotParent.x, m_Model.rotParent.z);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//���[���h�}�g���b�N�X���������̕ۑ�
	mtxRotParent = mtxRot;

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, m_Model.posParent.x, m_Model.posParent.y, m_Model.posParent.z);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxTrans);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Model.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Model.rot.y, m_Model.rot.x, m_Model.rot.z);
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxRot);

	//���[���h�}�g���b�N�X���������̕ۑ�
	mtxRotChild = mtxRot;

	D3DXVECTOR3 pos = m_Model.pos + m_Model.posMove;
	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxTrans);

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxRoot);

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X(��������)
	D3DXMatrixMultiply(&m_Model.mtxWorldRot, &mtxRotChild, &mtxRotParent);


	m_Model.vtxMax2 = m_Model.pos + m_Model.vtxMax;
	m_Model.vtxMin2 = m_Model.pos + m_Model.vtxMin;

	//���f���̍ő�ŏ������[���h�}�g���b�N�X�ϊ����ʕۑ�
	D3DXVec3TransformCoord(&m_Model.vtxMax2, &m_Model.vtxMax, &m_Model.mtxWorld);
	D3DXVec3TransformCoord(&m_Model.vtxMin2, &m_Model.vtxMin, &m_Model.mtxWorld);

}

//*****************************************************************************
//�@���ݒ�
//*****************************************************************************
void C3DObject::SetNormal()
{
	int nNumVix;		//���_��
	int nNumIndex;		//�C���f�b�N�X��
	int nNumPolygon;	//�|���S����

	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	BYTE *pIndexBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();
	//�|���S�����̎擾
	nNumPolygon = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumFaces();
	//�C���f�N�X���̎擾
	nNumIndex = nNumPolygon + 2;
	
	//���_�̐ݒ�
	m_Model.pTopPos = new D3DXVECTOR3[nNumVix];

	//�@���̐ݒ�
	m_Model.pNormalPolygon = new D3DXVECTOR3[nNumPolygon];

	//���_�t�H�[�}�b�g�擾
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_Model.nPattn].pMeshModel->GetFVF());

	//���_���b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);


	//���ׂĂ̒��_POS�̎擾
	for (int nCnt = 0; nCnt < nNumVix; nCnt++)
	{
		//���_���W�̎擾
		m_Model.pTopPos[nCnt] = *(D3DXVECTOR3*)pVtxBuff;
		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockVertexBuffer();

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	int nIndex1, nIndex2, nIndex3;
	//�@���̌v�Z
	for (int nCnt = 0; nCnt < nNumPolygon; nCnt++)
	{
		
		nIndex1 = *(WORD*)pIndexBuff;
		nIndex2 = *((WORD*)pIndexBuff + 1);
		nIndex3 = *((WORD*)pIndexBuff + 2);

		//���_���W�̑��
		D3DXVECTOR3 vtx1 = m_Model.pTopPos[nIndex1];
		D3DXVECTOR3 vtx2 = m_Model.pTopPos[nIndex2];
		D3DXVECTOR3 vtx3 = m_Model.pTopPos[nIndex3];

		D3DXVECTOR3 vec1, vec2, vecResult;

		vec1 = vtx2 - vtx1;
		vec2 = vtx3 - vtx1;

		//�O��
		/*if (nCnt % 2 == 1)
		{
			D3DXVec3Cross(&vecResult, &vec2, &vec1);
		}
		else
		{*/
			D3DXVec3Cross(&vecResult, &vec1, &vec2);
		//}
		//���K��
		D3DXVec3Normalize(&vecResult, &vecResult);

		//�ʂ̖@���x�N�g���̕ۑ�
		m_Model.pNormalPolygon[nCnt] = vecResult;

		//�f�[�^��i�߂�
		pIndexBuff+=sizeof(WORD)*3;
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockIndexBuffer();
}

//*****************************************************************************
//�@���̌����̍X�V
//*****************************************************************************
void C3DObject::UpdateNormal()
{
	int nNumPolygon;	//�|���S����
	//�|���S�����̎擾
	nNumPolygon = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumFaces();

	BYTE *pIndexBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	int nIndex1, nIndex2, nIndex3;
	//�@���̌v�Z
	for (int nCnt = 0; nCnt < nNumPolygon; nCnt++)
	{

		nIndex1 = *(WORD*)pIndexBuff;
		nIndex2 = *((WORD*)pIndexBuff + 1);
		nIndex3 = *((WORD*)pIndexBuff + 2);

		//���_���W�̑��
		D3DXVECTOR3 vtx1 = m_Model.pTopPos[nIndex1];
		D3DXVECTOR3 vtx2 = m_Model.pTopPos[nIndex2];
		D3DXVECTOR3 vtx3 = m_Model.pTopPos[nIndex3];

		D3DXVECTOR3 vec1, vec2, vecResult;

		vec1 = vtx2 - vtx1;
		vec2 = vtx3 - vtx1;

		//�O��
		/*if (nCnt % 2 == 1)
		{
		D3DXVec3Cross(&vecResult, &vec2, &vec1);
		}
		else
		{*/
		D3DXVec3Cross(&vecResult, &vec1, &vec2);
		//}
		//���K��
		D3DXVec3Normalize(&vecResult, &vecResult);

		//�ʂ̖@���x�N�g���̕ۑ�
		m_Model.pNormalPolygon[nCnt] = vecResult;

		//�f�[�^��i�߂�
		pIndexBuff += sizeof(WORD) * 3;
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockIndexBuffer();
}

//*****************************************************************************
//�����蔻��
//*****************************************************************************
D3DXVECTOR3 C3DObject::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

bool C3DObject::NormalCollision(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vtx[8];

	vtx[0] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMax2.y, m_Model.vtxMin2.z);
	vtx[1] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMax2.y, m_Model.vtxMin2.z);
	vtx[2] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMin2.y, m_Model.vtxMin2.z);
	vtx[3] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMin2.y, m_Model.vtxMin2.z);

	vtx[4] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMax2.y, m_Model.vtxMax2.z);
	vtx[5] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMax2.y, m_Model.vtxMax2.z);
	vtx[6] = D3DXVECTOR3(m_Model.vtxMax2.x, m_Model.vtxMin2.y, m_Model.vtxMax2.z);
	//vtx[7] = D3DXVECTOR3(m_Model.vtxMin2.x, m_Model.vtxMin2.y, m_Model.vtxMax2.z);

	//�l�p�`�̓��ɂ��邩�ǂ���
	if(!SquareInOut(pos, vtx[0], vtx[1], vtx[2], vtx[3]))
	{
		return false;
	}
	if (!SquareInOut(pos, vtx[1], vtx[5], vtx[6], vtx[2]))
	{
		return false;
	}
	if (!SquareInOut(pos, vtx[4], vtx[5], vtx[1], vtx[0]))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
//�O�p�`�̓��ɂ��邩�ǂ���
//*****************************************************************************
bool C3DObject::TriangleInOut(D3DXVECTOR3 pos, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0[3], fInnerProduct1[3], fInnerProduct2[3];

	vec1 = vtx1 - vtx0;
	vec2 = pos - vtx0;

	fInnerProduct0[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct0[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct0[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx2 - vtx1;
	vec2 = pos - vtx1;

	fInnerProduct1[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct1[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct1[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx0 - vtx2;
	vec2 = pos - vtx2;

	fInnerProduct2[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct2[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct2[2] = vec1.z * vec2.y - vec1.y * vec2.z;


	if (
		(fInnerProduct0[0] > 0.0f && fInnerProduct1[0] > 0.0f && fInnerProduct2[0] > 0.0f)
		|| (fInnerProduct0[0] < 0.0f && fInnerProduct1[0] < 0.0f && fInnerProduct2[0] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[1] > 0.0f && fInnerProduct1[1] > 0.0f && fInnerProduct2[1] > 0.0f)
		|| (fInnerProduct0[1] < 0.0f && fInnerProduct1[1] < 0.0f && fInnerProduct2[1] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[2] > 0.0f && fInnerProduct1[2] > 0.0f && fInnerProduct2[2] > 0.0f)
		|| (fInnerProduct0[2] < 0.0f && fInnerProduct1[2] < 0.0f && fInnerProduct2[2] < 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
//�l�p�`�̓��ɂ��邩�ǂ���
//*****************************************************************************
bool C3DObject::SquareInOut(D3DXVECTOR3 pos, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0[3], fInnerProduct1[3], fInnerProduct2[3] , fInnerProduct3[3];

	vec1 = vtx1 - vtx0;
	vec2 = pos - vtx0;

	fInnerProduct0[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct0[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct0[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx2 - vtx1;
	vec2 = pos - vtx1;

	fInnerProduct1[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct1[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct1[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx3 - vtx2;
	vec2 = pos - vtx2;

	fInnerProduct2[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct2[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct2[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx0 - vtx3;
	vec2 = pos - vtx3;

	fInnerProduct3[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct3[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct3[2] = vec1.z * vec2.y - vec1.y * vec2.z;


	if (
		(fInnerProduct0[0] > 0.0f && fInnerProduct1[0] > 0.0f && fInnerProduct2[0] > 0.0f && fInnerProduct3[0] > 0.0f)
		|| (fInnerProduct0[0] < 0.0f && fInnerProduct1[0] < 0.0f && fInnerProduct2[0] < 0.0f && fInnerProduct3[0] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[1] > 0.0f && fInnerProduct1[1] > 0.0f && fInnerProduct2[1] > 0.0f && fInnerProduct3[1] > 0.0f)
		|| (fInnerProduct0[1] < 0.0f && fInnerProduct1[1] < 0.0f && fInnerProduct2[1] < 0.0f && fInnerProduct3[1] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[2] > 0.0f && fInnerProduct1[2] > 0.0f && fInnerProduct2[2] > 0.0f && fInnerProduct3[2] > 0.0f)
		|| (fInnerProduct0[2] < 0.0f && fInnerProduct1[2] < 0.0f && fInnerProduct2[2] < 0.0f && fInnerProduct3[2] < 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// �J���[�ݒ�
//*****************************************************************************
void C3DObject::SetColor(D3DXCOLOR colorMax, D3DXCOLOR colorMin, float fAttenuation)
{
	if (m_bColor)
	{
		return;
	}

	m_ColorMax = colorMax;
	m_ColorMin = colorMin;
	m_fAttenuationColor = fAttenuation;
	m_ColorChange = colorMax - colorMin;
}

//*****************************************************************************
// ���f���̃Z�b�g
//*****************************************************************************
int C3DObject::SetModel(ModelPattern *pModel)
{
	for (int nCnt = 0; nCnt < MODEL_PATTERN_MAX; nCnt++)
	{
		if (!m_ModelPattern[nCnt].bUse)
		{
			m_ModelPattern[nCnt].nNumMatModel = pModel->nNumMatModel;
			m_ModelPattern[nCnt].pBuffMatModel = pModel->pBuffMatModel;
			m_ModelPattern[nCnt].pMeshModel = pModel->pMeshModel;

			int nNumMatModel = pModel->nNumMatModel;
			for (int nCntTex = 0; nCntTex < nNumMatModel; nCntTex++)
			{
				m_ModelPattern[nCnt].pTexture[nCntTex] = pModel->pTexture[nCntTex];
			}

			m_ModelPattern[nCnt].bUse = true;
			return nCnt;
		}
	}
	return -1;
}

//*****************************************************************************
// ���f���̑S�I������
//*****************************************************************************
void C3DObject::UninitAllModel()
{
	for (int nCnt = 0; nCnt < MODEL_PATTERN_MAX; nCnt++)
	{
		for (int i = 0; i < (int)m_ModelPattern[nCnt].nNumMatModel; i++)
		{
			if (m_ModelPattern[nCnt].pTexture[i] != nullptr)
			{// �e�N�X�`���̉��
				m_ModelPattern[nCnt].pTexture[i]->Release();
				m_ModelPattern[nCnt].pTexture[i] = nullptr;
			}
		}

		//���b�V���̔j��
		if (m_ModelPattern[nCnt].pMeshModel != nullptr)
		{
			m_ModelPattern[nCnt].pMeshModel->Release();
			m_ModelPattern[nCnt].pMeshModel = nullptr;
		}

		//�}�e���A���̔j��
		if (m_ModelPattern[nCnt].pBuffMatModel != nullptr)
		{
			m_ModelPattern[nCnt].pBuffMatModel->Release();
			m_ModelPattern[nCnt].pBuffMatModel = nullptr;
		}

	}

	ZeroMemory(&m_ModelPattern, sizeof(m_ModelPattern));
}

//*****************************************************************************
// ���̃��f���̃��[���h���W
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetWorldPos()
{
	D3DXVECTOR3 pos;
	pos.x = m_Model.mtxWorld._41;
	pos.y = m_Model.mtxWorld._42;
	pos.z = m_Model.mtxWorld._43;

	return pos;
}

D3DXVECTOR3 C3DObject::GetWorlMatrixRot()
{
	D3DXVECTOR3 pos;
	pos.x = m_Model.mtxWorldRot._41;
	pos.y = m_Model.mtxWorldRot._42;
	pos.z = m_Model.mtxWorldRot._43;
	return pos;
}

//*****************************************************************************
// ���̃��f���̃��[���hRot
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetWorldRot()
{
	D3DXVECTOR3 rot;
	rot.x = m_Model.mtxWorld._41;
	rot.y = m_Model.mtxWorld._42;
	rot.z = m_Model.mtxWorld._43;

	return rot;
}

//*****************************************************************************
// ���f���̃T�C�Y
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetSize()
{
	D3DXVECTOR3 size;

	D3DXVECTOR3 minpos, maxpos;

	minpos = m_Model.pos + m_Model.vtxMin;

	maxpos = m_Model.pos + m_Model.vtxMax;

	size = maxpos - minpos;
	
	return size;
}