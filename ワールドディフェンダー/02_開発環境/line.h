//=============================================================================
//
// ���C������ [line.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _LINE_H_			//���̃}�N����`������Ȃ�������
#define _LINE_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLine : public CObject
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int LINE_BUFF_MAX = 16;
	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);		//���W�A�J���[
public:
	CLine();
	~CLine() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXCOLOR col);

	void SetMtxWorld(D3DXMATRIX MtxWorld) { m_MtxWorld = MtxWorld; }

private:
	D3DXMATRIX m_MtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_SizeMax;
	D3DXVECTOR3 m_SizeMin;
	D3DXVECTOR3 m_Pos;
	D3DXCOLOR m_Color;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLine;//���_�o�b�t�@�ւ̃|�C���^
};

#endif
