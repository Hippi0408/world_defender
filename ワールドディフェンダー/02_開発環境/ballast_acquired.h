//=============================================================================
//
//�@���I�̏��� [ballast_acquired.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _BALLAST_ACQUIRED_H_			//���̃}�N����`������Ȃ�������
#define _BALLAST_ACQUIRED_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "ballast.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBallast_Acquired : public CBallast
{
private:
	static const float GRAVITY;
	static const float MOVE_MAGNIFICATION;
	static const float MOVE_ATTENUATION;

public:
	CBallast_Acquired();
	~CBallast_Acquired() override;

	void Update() override;//�X�V����
	
	void SetVec(D3DXVECTOR3 Vec);
	D3DXVECTOR3 GetVec() {return m_Vec; }

private:
	D3DXVECTOR3 m_Vec;
	bool m_bFallenGroundOnce;
};

#endif
