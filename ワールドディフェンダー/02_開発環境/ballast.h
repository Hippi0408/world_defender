//=============================================================================
//
//�@���I�̏��� [ballast.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _BALLAST_H_			//���̃}�N����`������Ȃ�������
#define _BALLAST_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "3dobject.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBallast : public C3DObject
{
private:
	static const int SURFACE_MAX = 6;
	static const int TOP_MAX = 4;
	
public:
	CBallast();
	~CBallast() override;

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	virtual void Draw() override;//�`�揈��

	//���I�̈ʒu��Set�AGet
	void SetListNumber(int nNumber) { m_nListNumber = nNumber; }
	int GetListNumber() { return m_nListNumber; }

	//���I�̃v���C���[�Ƃ̈ʒu��Set�AGet�AAdd
	void SetPositionWithPlayer(D3DXVECTOR3 PositionWithPlayer) { m_PositionWithPlayer = PositionWithPlayer; }
	D3DXVECTOR3 GetPositionWithPlayer() { return m_PositionWithPlayer; }
	void AddPositionWithPlayer(D3DXVECTOR3 PositionWithPlayer) { m_PositionWithPlayer += PositionWithPlayer; }

	//���I�̃v���C���[�Ƃ̌�����Set�AGet
	void SetFacingWithPlayer(D3DXVECTOR3 FacingWithPlayer) { m_FacingWithPlayer = FacingWithPlayer; }
	D3DXVECTOR3 GetFacingWithPlayer() { return m_FacingWithPlayer; }

	//���I�̐e�ʒu�̈ړ��ʂ�Set�AGet
	void SetParentPosMove(D3DXVECTOR3 ParentPosMove) { m_ParentPosMove = ParentPosMove; }
	D3DXVECTOR3 GetParentPosMove() { return m_ParentPosMove; }

	//�g�p��Ԍn
	void SetUse(bool bUse) { m_bUse = bUse; }
	bool GetUse() { return m_bUse; }

	//���V��Ԍn
	void SetFloating(bool bFloating) { m_bFloating = bFloating; }
	bool GetFloating() { return m_bFloating; }

	//������Move
	void SetMoveRot(D3DXVECTOR3 MoveRot) { m_MoveRot = MoveRot; }

	//���a�̎擾
	float GetRadius() { return m_fRadius; }
	//���a�̐ݒ�
	void SetRadius();
	//�T�C�Y�|�C���g�̎擾
	int GetSizePoint() { return m_nSizePoint; }

	//�����蔻��(�܂Ƃ�)
	D3DXVECTOR3 ConclusionCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min);

	//�J�����`��̗L��
	bool IsExistingDuring2Point(D3DXVECTOR3 posR, D3DXVECTOR3 posV);
	
private:
	D3DXVECTOR3 m_PositionWithPlayer;
	D3DXVECTOR3 m_FacingWithPlayer;
	D3DXVECTOR3 m_ParentPosMove;
	D3DXVECTOR3 m_MoveRot;
	int m_nListNumber;
	bool m_bUse;
	bool m_bFloating;
	float m_fRadius;
	int m_nSizePoint;
	
};

#endif
