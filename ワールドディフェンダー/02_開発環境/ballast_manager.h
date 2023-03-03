//=============================================================================
//
//�@���I�̊Ǘ��ҏ��� [ballast_manager.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _BALLAST_MANAGER_H_			//���̃}�N����`������Ȃ�������
#define _BALLAST_MANAGER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "object.h"
#include <map>
#include <list>
#include "3dobject.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBallast;
class CMeshfield;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBallast_Manager : public CObject
{
private:
	static const float MAP_MAX;
	static const D3DXVECTOR3 INIT_POS;
	static const float BASE_RADIUS_PER_RUBBLE_ONE;
	static const int BALLAST_PATTERN = 5;
public:
	CBallast_Manager();
	~CBallast_Manager() override;

	HRESULT Init() override;//������
	void Uninit() override;//�I������
	void Update() override;//�X�V����
	void Draw() override;//�`�揈��

	//�v���C���[�̈ʒu��Set�AGet
	void SetPlListNumber(int nNumber) { m_nPlListNumber = nNumber; }
	int GetPlListNumber() { return m_nPlListNumber; }

	//���I�̐ݒ�
	void SetBallast(int nNumber, Object_Data Data);

	//���b�V���t�B�[���h�̐ݒ�
	void MeshfieldSet(CMeshfield* pMeshfield);

	//�T�C�R�L�l�V�X�G���A�ɂ������炻�̃I�u�W�F�N�g�̃|�C���^��Ԃ�
	CBallast* CheckCircleCollision(D3DXVECTOR3 pos, float fRadius, int nAdrenaline);

	//�T�C�R�L�l�V�X�G���A�ɂ������炻�̃I�u�W�F�N�g�̐F��ς���
	void WithinRangeColor(int nMapGrid, D3DXVECTOR3 pos, float fRadius, int nAdrenaline);

	//��Q���̓����蔻��
	D3DXVECTOR3 CollisionBallast(int nMapGrid, D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min);

	//�G�l�~�[�Ƃ̓����蔻��
	void CollisionEnemy();

	//���V��Ԃ̃��X�g�ǉ�
	void SetFloatingBallst(CBallast* pBallast);

	//��V�I���I�̐���(�����͔�΂����������x�N�g��,���Ƃ͊�{���)
	void SetBallastAcquired(D3DXVECTOR3 vec,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius);

	//���X�g���̓���ւ�
	void ReplacementList(CBallast* pBallast, int nNext);

	//���C�g�̕���Vec��Set,Get
	void SetLight(D3DXVECTOR3 vec) { m_Light = vec; }
	D3DXVECTOR3 GetLight() { return m_Light; }

	//���X�g�̌����m��
	void CreateBallastListData(int nDataNum);

private:
	std::list<CBallast*>* m_pBallastListData;		//�}�b�v��̃I�u�W�F�N�g�����X�g�ŊǗ�����ϐ�
	int m_nBallastListDataMax;						//��L�̕ϐ���Max
	std::list<CBallast*> m_FloatingBallstList;					//���V��Ԃ̊��I
	int m_nPlListNumber;
	CMeshfield *m_pMeshfieldCopy;								//���b�V���t�B�[���h�|�C���^�̃R�s�[�i���̃R�s�[�͉�������Ȃ��j
	int m_nMeshfieldNumMax;										//���b�V���̃}�X�̐��̍ő�
	int m_nBallast_Acquired_Model[BALLAST_PATTERN];
	D3DXVECTOR3 m_Light;		//���C�g�̌���
};

#endif
