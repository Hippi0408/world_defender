//=============================================================================
//
// camera.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _CAMERA_H_			//���̃}�N����`������Ȃ�������
#define _CAMERA_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCamera : public CObject
{
private:
	
public:
	CCamera();
	virtual ~CCamera() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	void SetCamera();

	//��{���
	D3DXVECTOR3 GetPosV() { return m_posV; }
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void AddPosV(D3DXVECTOR3 addV) { m_posV += addV; }

	D3DXVECTOR3 GetPosR() { return m_posR; }
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	void AddPosR(D3DXVECTOR3 addR) { m_posR += addR; }

	void SetVecU(D3DXVECTOR3 vecU) { m_vecU = vecU; }
	float GetRot();

	void SetVibration(int nVibrationCntMax, int nVibration);
	bool CheckVibration();

private:
	D3DXVECTOR3 m_posV;		//���_
	D3DXVECTOR3 m_posVDest;		//���_
	D3DXVECTOR3 m_posR;		//�����_
	D3DXVECTOR3 m_posRDest;		//�����_
	D3DXVECTOR3 m_vecU;		//������x�N�g��
	D3DXMATRIX m_mtxProjection; //�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		  //�r���[�}�g���b�N�X
	float fDistance;			//���_�������_�̂܂ł̋���
	D3DXVECTOR3 m_AddVibration;
	int m_nVibration;
	int m_nVibrationCnt;
	int m_nVibrationCntMax;
};

#endif