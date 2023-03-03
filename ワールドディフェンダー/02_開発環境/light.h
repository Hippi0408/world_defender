//=============================================================================
//
// ���C�g���� [light.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _LIGHT_H_			//���̃}�N����`������Ȃ�������
#define _LIGHT_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLight : public CObject
{
private:
	static const int LIGHT_MAX = 4;

public:
	CLight();
	~CLight() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetLightVec();

private:
	D3DLIGHT9 m_alight[LIGHT_MAX];//���C�g�̏��

};

#endif

