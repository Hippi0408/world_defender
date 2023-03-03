//=============================================================================
//
// ���I���� [ballast.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "ballast.h"
#include "convenience_function.h"
#include "game.h"
#include "manager.h"
#include "ballast_manager.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CBallast::CBallast()
{
	m_nListNumber = 0;
	m_MoveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bFloating = false;
	m_ParentPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CBallast::~CBallast()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CBallast::Init()
{
	m_nSizePoint = 0;

	//�g�p����
	m_bUse = true;

	//������
	if (FAILED(C3DObject::Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CBallast::Uninit()
{
	//�e�N���X�̏I������
	C3DObject::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CBallast::Update()
{
	//�e���̎擾
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move = GetPosMove();
	D3DXVECTOR3 rot = GetRot();

	if (!m_bFloating)
	{
		//�q�̈ʒu�̍X�V
		pos = GetParentPos();
		pos += move;
		SetParentPos(pos);
	}
	else
	{
		//�q�̈ʒu�̍X�V
		pos = GetPos();
		pos += move;
		SetPos(pos);
	}


	//�����̍X�V
	rot += m_MoveRot;
	SetRot(rot);

	//�e�ʒu�̍X�V
	AddParentPos(m_ParentPosMove);

	//�e�N���X�̍X�V
	C3DObject::Update();
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CBallast::Draw()
{
	//�}�g���b�N�X�̌v�Z
	CalculationMatrix();
	//�e�N���X�̕`�揈��
	C3DObject::Draw();
}

//*****************************************************************************
//���a�̐ݒ�
//*****************************************************************************
void CBallast::SetRadius()
{
	//�e��
	float fSideX, fSideY, fSideZ;

	//�T�C�Y
	D3DXVECTOR3 Max, Min;

	//�T�C�Y�̍ő�ŏ��̎擾
	Max = GetVtxMax();
	Min = GetVtxMin();

	//�e�ӂ̌v�Z
	fSideX = Max.x - Min.x;
	fSideY = Max.y - Min.y;
	fSideZ = Max.z - Min.z;

	//�Ίp���̐���
	float fDiagonalLine = sqrt(fSideX * fSideX + fSideY * fSideY + fSideZ * fSideZ);

	//�Ίp�����甼�a�̐���
	m_fRadius = fDiagonalLine / 2.0f;

	//�T�C�Y�|�C���g�̌v�Z
	m_nSizePoint = (int)m_fRadius / 10.0f;

	//�T�C�Y�|�C���g�̒���
	if (m_nSizePoint > 100)
	{
		m_nSizePoint = 100;
	}


}

//*****************************************************************************
//	�����蔻��
//*****************************************************************************
D3DXVECTOR3 CBallast::ConclusionCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	//�ŏI�I�ȉ����o���l
	D3DXVECTOR3 Add = pos;

	
	//�ڐG�m�F�p�ϐ�
	D3DXVECTOR3 HittingTargetPosMax[SURFACE_MAX], HittingTargetPosMin[SURFACE_MAX], HittingTargetOldPosMax[SURFACE_MAX], HittingTargetOldPosMin[SURFACE_MAX];

	//���f���̈ʒu
	D3DXVECTOR3 ModelPos = GetPos();
	D3DXVECTOR3 ModelRot = GetRot();
	D3DXVECTOR3 ModelMax = GetVtxMax();
	D3DXVECTOR3 ModelMin = GetVtxMin();

	//�}�g���b�N�X�ϊ����|����
	D3DXVECTOR3 MatrixConversionMax, MatrixConversionMin;

	//���
	MatrixConversionMax = ModelPos + ModelMax;
	MatrixConversionMin = ModelPos + ModelMin;

	//���g�̉�]�̃}�g���b�N�X
	D3DXMATRIX ThisMatrixRot = GetMatrix();

	//�����蔻��p�̖ʂ̂S���_
	D3DXVECTOR3 aTop[4];
	D3DXVECTOR3 Top,Down;


	Top = D3DXVECTOR3(0.0f, MatrixConversionMax.y, 0.0f);
	Down = D3DXVECTOR3(0.0f, MatrixConversionMin.y, 0.0f);
	
	D3DXVec3TransformCoord(&Top, &Top, &ThisMatrixRot);
	D3DXVec3TransformCoord(&Down, &Down, &ThisMatrixRot);

	//*****************************************************************************
	//���
	//*****************************************************************************

	aTop[0] = D3DXVECTOR3(MatrixConversionMin.x, MatrixConversionMax.y, MatrixConversionMin.z);
	aTop[1] = D3DXVECTOR3(MatrixConversionMin.x, MatrixConversionMax.y, MatrixConversionMax.z);
	aTop[2] = D3DXVECTOR3(MatrixConversionMax.x, MatrixConversionMax.y, MatrixConversionMax.z);
	aTop[3] = D3DXVECTOR3(MatrixConversionMax.x, MatrixConversionMax.y, MatrixConversionMin.z);

	D3DXVec3TransformCoord(&aTop[0], &aTop[0], &ThisMatrixRot);
	D3DXVec3TransformCoord(&aTop[1], &aTop[1], &ThisMatrixRot);
	D3DXVec3TransformCoord(&aTop[2], &aTop[2], &ThisMatrixRot);
	D3DXVec3TransformCoord(&aTop[3], &aTop[3], &ThisMatrixRot);

	if (!CConvenience_Function::InnerProductCollisionBase(aTop, D3DXVECTOR3(pos.x, pos.y, pos.z)))
	{
		//�����o���l��Ԃ�
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (!(pos.y > Down.y
		&& pos.y < Top.y))
	{
		//�����o���l��Ԃ�
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	D3DXVECTOR3 TopPos;

	//���I�̏�ʗp��Pos
	D3DXVECTOR3 BallstTop;


	//����

	//�ړ��x�N�g��
	D3DXVECTOR3 Vec = pos - oldpos;

	D3DXVECTOR3 vec1, vec2;
	float fInnerProductPos, fInnerProductOldPos;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�w��̕ӂ̒��ɂ��邩�ǂ���
		vec1 = aTop[(nCnt + 1) % 4] - aTop[nCnt];
		vec2 = pos - aTop[nCnt];

		fInnerProductPos = vec1.x * vec2.z - vec1.z * vec2.x;

		vec1 = aTop[(nCnt + 1) % 4] - aTop[nCnt];
		vec2 = oldpos - aTop[nCnt];

		fInnerProductOldPos = vec1.x * vec2.z - vec1.z * vec2.x;

		//����ꍇ
		if (fInnerProductPos < 0.0f
			&& fInnerProductOldPos >= 0.0f
			&& oldpos.y > Down.y
			&& oldpos.y < Top.y
			)
		{
			//�v�Z�p�ϐ��錾
			D3DXVECTOR3 V, V1, V2, Cross1, Cross2, Result;
			float fT2;

			//�����Ɛ����̏Փ�
			V = aTop[nCnt] - oldpos;
			V1 = pos - oldpos;
			V2 = aTop[(nCnt + 1) % 4] - aTop[nCnt];

			V.y = 0.0f;
			V1.y = 0.0f;
			V2.y = 0.0f; 

			fT2 = (V.x * V1.z - V.z * V1.x) / (V1.x * V2.z - V1.z * V2.x);

			Result = aTop[nCnt] + fT2 * V2;

			Result.y = pos.y;

			V1 *= -1.0f;

			D3DXVec3Normalize(&V1, &V1);

			return Result + V1;

		}

	}
		
	//*****************************************************************************
	//���
	//*****************************************************************************

	//�@��
	D3DXVECTOR3 Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�@����Rot�}�g���b�N�X�ŕϊ�
	D3DXVec3TransformCoord(&Normal, &Normal, &GetMatrixRot());

	//�x�N�g���̒������P�ɂ���
	D3DXVec3Normalize(&Normal,&Normal);

	//��ʂ̂P���_
	 TopPos = GetPos() + D3DXVECTOR3(MatrixConversionMin.x, MatrixConversionMax.y, MatrixConversionMin.z);
	
	//���I�̏�ʗp��Pos
	 BallstTop;

	//��ʗp�̓����蔻��
	BallstTop = CConvenience_Function::InnerProductCollisionBaseExtrusion(TopPos, Normal, D3DXVECTOR3(pos.x, pos.y + ModelMin.y, pos.z));

	//��ʂ̍�������
	if (pos.y < BallstTop.y
		&& oldpos.y >= BallstTop.y)
	{
		//�����o���l��Ԃ�
		return BallstTop;
	}

	//�����o���l��Ԃ�
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//�J�����`��̗L��
//*****************************************************************************
bool CBallast::IsExistingDuring2Point(D3DXVECTOR3 posR, D3DXVECTOR3 posV)
{
	//�}�g���b�N�X
	D3DXMATRIX mat, invmat;

	//�}�g���b�N�X�̎擾
	mat = GetMatrix();

	//�t�s��̌v�Z
	D3DXMatrixInverse(&invmat, NULL, &mat);

	//�ϊ�
	D3DXVec3TransformCoord(&posR, &posR, &invmat);
	D3DXVec3TransformCoord(&posV, &posV, &invmat);

	//���b�V�����
	LPD3DXMESH pMesh = GetMeshData();

	//�����_���王�_�̃x�N�g��
	D3DXVECTOR3 Vec = posV - posR;

	//�������P�ɂ���
	D3DXVec3Normalize(&Vec,&Vec);

	//�����������ǂ���
	BOOL bHit = false;

	//���C���΂��ă��b�V���Ɠ������Ă邩�ǂ���
	if (FAILED(D3DXIntersect(pMesh, &posR, &Vec, &bHit,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)))
	{
		return false;
	}

	return bHit;
}
