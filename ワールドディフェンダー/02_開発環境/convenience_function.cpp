//=============================================================================
//
// convenience_function.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "convenience_function.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CConvenience_Function::CConvenience_Function()
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CConvenience_Function::~CConvenience_Function()
{
}

//*****************************************************************************
// Rot�̐��K��
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::NormalizationRot(D3DXVECTOR3 In)
{
	D3DXVECTOR3 rot = In;

	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	return rot;
}

//*****************************************************************************
// �~���m�̓����蔻��
//*****************************************************************************
bool CConvenience_Function::CircleCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//�ʒu�̕ۑ�
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//�Q���̕��̂̔��a���m�̘a
	float fDiff = fRadius1 + fRadius2;

	//�v�Z�p�ϐ�
	float fCalculationX, fCalculationZ;

	//X�̍���
	fCalculationX = Pos1.x - Pos2.x;
	//Y�̍���
	fCalculationZ = Pos1.z - Pos2.z;

	//���݂̂Q�_�̋���
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationZ * fCalculationZ);

	//�Q���̕��̂̔��a���m�̘a��茻�݂̂Q�_�̋��������������ǂ���
	if (fDiff >= fLength)
	{
		//��������
		return true;
	}

	//�������ĂȂ�
	return false;
}

//*****************************************************************************
// �����m�̓����蔻��
//*****************************************************************************
bool CConvenience_Function::SphereCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//�ʒu�̕ۑ�
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//�Q���̕��̂̔��a���m�̘a
	float fDiff = fRadius1 + fRadius2;

	//�v�Z�p�ϐ�
	D3DXVECTOR3 Distance;

	//2�_�Ƃ̋���
	Distance = Pos1 - Pos2;

	//���݂̂Q�_�̋���
	float fLength = sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//�Q���̕��̂̔��a���m�̘a��茻�݂̂Q�_�̋��������������ǂ���
	if (fDiff >= fLength)
	{
		//��������
		return true;
	}

	//�������ĂȂ�
	return false;
}

//*****************************************************************************
// 2�_�Ԃ̋���
//*****************************************************************************
float CConvenience_Function::PointDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	//�ʒu�̕ۑ�
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//�v�Z�p�ϐ�
	D3DXVECTOR3 Distance;

	//2�_�Ƃ̋���
	Distance = Pos1 - Pos2;

	//���݂̂Q�_�̋���
	float fLength = sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//�v�Z���ʂ�Ԃ�
	return fLength;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A���ɂ��邩�ǂ���
//*****************************************************************************
bool CConvenience_Function::InnerProductCollision(D3DXVECTOR3 * pTop1, D3DXVECTOR3 * pTop2, D3DXVECTOR3 TopTarget)
{
	//�`�F�b�N�p
	D3DXVECTOR3 aPos[4];

	if (!InnerProductCollisionBase(pTop1, TopTarget))
	{
		return false;
	}
	if (!InnerProductCollisionBase(pTop2, TopTarget))
	{
		return false;
	}

	aPos[0] = pTop1[0];
	aPos[1] = pTop1[1];
	aPos[2] = pTop2[1];
	aPos[3] = pTop1[0];

	if (!InnerProductCollisionSideX(aPos, TopTarget))
	{
		return false;
	}

	aPos[0] = pTop1[2];
	aPos[1] = pTop1[3];
	aPos[2] = pTop2[3];
	aPos[3] = pTop1[2];

	if (!InnerProductCollisionSideX(aPos, TopTarget))
	{
		return false;
	}


	aPos[0] = pTop1[0];
	aPos[1] = pTop1[3];
	aPos[2] = pTop2[3];
	aPos[3] = pTop1[0];

	if (!InnerProductCollisionSideZ(aPos, TopTarget))
	{
		return false;
	}

	aPos[0] = pTop1[1];
	aPos[1] = pTop1[2];
	aPos[2] = pTop2[2];
	aPos[3] = pTop1[1];

	if (!InnerProductCollisionSideX(aPos, TopTarget))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A���ɂ��邩�ǂ����i��ʁj
//*****************************************************************************
bool CConvenience_Function::InnerProductCollisionBase(D3DXVECTOR3* pTop1, D3DXVECTOR3 Top2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2, fInnerProduct3;


	vec1 = pTop1[1] - pTop1[0];
	vec2 = Top2 - pTop1[0];

	fInnerProduct0 = vec1.x * vec2.z - vec1.z * vec2.x;

	vec1 = pTop1[2] - pTop1[1];
	vec2 = Top2 - pTop1[1];

	fInnerProduct1 = vec1.x * vec2.z - vec1.z * vec2.x;

	vec1 = pTop1[3] - pTop1[2];
	vec2 = Top2 - pTop1[2];

	fInnerProduct2 = vec1.x * vec2.z - vec1.z * vec2.x;

	vec1 = pTop1[0] - pTop1[3];
	vec2 = Top2 - pTop1[3];

	fInnerProduct3 = vec1.x * vec2.z - vec1.z * vec2.x;

	if (
		(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f && fInnerProduct3 >= 0.0f)
		|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f && fInnerProduct3 <= 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A���ɂ��邩�ǂ����i���ʁj
//*****************************************************************************
bool CConvenience_Function::InnerProductCollisionSideX(D3DXVECTOR3* pTop1, D3DXVECTOR3 Top2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2, fInnerProduct3;


	vec1 = pTop1[1] - pTop1[0];
	vec2 = Top2 - pTop1[0];

	fInnerProduct0 = vec1.x * vec2.y - vec1.y * vec2.x;

	vec1 = pTop1[2] - pTop1[1];
	vec2 = Top2 - pTop1[1];

	fInnerProduct1 = vec1.x * vec2.y - vec1.y * vec2.x;

	vec1 = pTop1[3] - pTop1[2];
	vec2 = Top2 - pTop1[2];

	fInnerProduct2 = vec1.x * vec2.y - vec1.y * vec2.x;

	vec1 = pTop1[0] - pTop1[3];
	vec2 = Top2 - pTop1[3];

	fInnerProduct3 = vec1.x * vec2.y - vec1.y * vec2.x;

	if (
		(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f && fInnerProduct3 >= 0.0f)
		|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f && fInnerProduct3 <= 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A���ɂ��邩�ǂ����i���ʁj
//*****************************************************************************
bool CConvenience_Function::InnerProductCollisionSideZ(D3DXVECTOR3* pTop1, D3DXVECTOR3 Top2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0, fInnerProduct1, fInnerProduct2, fInnerProduct3;


	vec1 = pTop1[1] - pTop1[0];
	vec2 = Top2 - pTop1[0];

	fInnerProduct0 = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = pTop1[2] - pTop1[1];
	vec2 = Top2 - pTop1[1];

	fInnerProduct1 = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = pTop1[3] - pTop1[2];
	vec2 = Top2 - pTop1[2];

	fInnerProduct2 = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = pTop1[0] - pTop1[3];
	vec2 = Top2 - pTop1[3];

	fInnerProduct3 = vec1.z * vec2.y - vec1.y * vec2.z;

	if (
		(fInnerProduct0 >= 0.0f && fInnerProduct1 >= 0.0f && fInnerProduct2 >= 0.0f && fInnerProduct3 >= 0.0f)
		|| (fInnerProduct0 <= 0.0f && fInnerProduct1 <= 0.0f && fInnerProduct2 <= 0.0f && fInnerProduct3 <= 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A�����o���i��ʁj
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::InnerProductCollisionBaseExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos)
{
	//�ʂ̂P���_
	D3DXVECTOR3 P1 = Top1;
	//�ʂ̖@��
	D3DXVECTOR3 vec = Normal;
	//�Ώۂ̈ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(TopPos.x, TopPos.y, TopPos.z); 

	//�Ώۂ�Pos�̖ʓ���Y
	float fPolygonY = P1.y - ((pos.x - P1.x) * vec.x + (pos.z - P1.z) * vec.z) / vec.y;

	//Y�̕ۑ�
	pos.y = fPolygonY;

	//Pos��Ԃ�
	return pos;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A�����o���iX���ʁj
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::InnerProductCollisionSideXExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos)
{
	//�ʂ̂P���_
	D3DXVECTOR3 P1 = Top1;
	//�ʂ̖@��
	D3DXVECTOR3 vec = Normal;
	//�Ώۂ̈ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(TopPos.x, TopPos.y, TopPos.z);

	//�Ώۂ�Pos�̖ʓ���X
	float fPolygonX = P1.x - ((pos.z - P1.z) * vec.z + (pos.y - P1.y) * vec.y) / vec.x;
	
	//X�̕ۑ�
	pos.x = fPolygonX;

	//Pos��Ԃ�
	return pos;
}

//*****************************************************************************
// ���ς̋�`�����蔻��A�����o���iZ���ʁj
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::InnerProductCollisionSideZExtrusion(D3DXVECTOR3 Top1, D3DXVECTOR3 Normal, D3DXVECTOR3 TopPos)
{
	//�ʂ̂P���_
	D3DXVECTOR3 P1 = Top1;
	//�ʂ̖@��
	D3DXVECTOR3 vec = Normal;
	//�Ώۂ̈ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(TopPos.x, TopPos.y, TopPos.z);

	//�Ώۂ�Pos�̖ʓ���Z
	float fPolygonZ = P1.z - ((pos.x - P1.x) * vec.x + (pos.y - P1.y) * vec.y) / vec.z;

	//Z�̕ۑ�
	pos.z = fPolygonZ;

	//Pos��Ԃ�
	return pos;
}

//*****************************************************************************
// �_�̕����x�N�g���̐���
//*****************************************************************************
D3DXVECTOR3 CConvenience_Function::PointOrientationVectorGeneration(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	//�����x�N�g��
	D3DXVECTOR3 vec;

	//�x�N�g���̐���
	vec = pos1 - pos2;

	//������1�ɂ���
	D3DXVec3Normalize(&vec,&vec);

	return vec;
}
