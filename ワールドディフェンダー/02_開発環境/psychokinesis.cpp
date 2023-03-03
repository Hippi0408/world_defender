//=============================================================================
//
// psychokinesis.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "psychokinesis.h"
#include "ballast_manager.h"
#include "ballast.h"
#include <assert.h>
#include "input.h"
#include "game.h"
#include "manager.h"
#include <time.h>
#include "convenience_function.h"
#include "meshfield.h"
#include "tutorial.h"
#include "sound.h"

const float CPsychokinesis::BALLAST_MOVE = 0.01f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPsychokinesis::CPsychokinesis()
{
	m_nCurrentFloatingNumber = 0;
	m_nCoolTime = 0;
	m_fRadius = 0.0f;
	m_nAdrenalineValue = 0;
	m_PlPos = D3DXVECTOR3();
	m_CameraVec = D3DXVECTOR3();
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPsychokinesis::~CPsychokinesis()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CPsychokinesis::Init()
{

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CPsychokinesis::Uninit()
{
	//�S�v�f�̍폜
	m_BallastCopyList.clear();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CPsychokinesis::Update()
{
	// ���V����
	SetFloating();

	//���̓f�o�C�X�̎擾
	CInput *pInput = CInput::GetKey();

	//�T�C�R�L�l�V�X������Ԃ�������
	if (pInput->Press(MOUSE_INPUT_LEFT))
	{
		// �T�C�R�L�l�V�X����
		Psychokinesis();
	}
	else
	{//���߂���

		if (pInput->Press(MOUSE_INPUT_RIGHT))
		{//�N�[���^�C������

			//�N�[���^�C���v�Z
			m_nCoolTime--;

			//�N�[���^�C�����O��葽��������
			if (m_nCoolTime > 0)
			{
				//�N�[���^�C����
				return;
			}

			//�N�[���^�C���̔���
			m_nCoolTime = COOL_TIME;
		}

		// �T�C�R�L�l�V�X�U������
		PsychokinesisAttack();
	}
}

//*****************************************************************************
// �X�V����(�ʒu�A�����A�J������Vec�A���a�X�V�A�o���ʒu)
//*****************************************************************************
void CPsychokinesis::Update(int nPlMap, D3DXVECTOR3 pos, D3DXVECTOR3 rot,  D3DXVECTOR3 CameraVec, float fRadius, float fFloatingHeight, int nAdrenalineValue)
{
	//�A�h���i�����̒l�̕ۑ�
	m_nAdrenalineValue = nAdrenalineValue;

	//�J�����̃x�N�g��
	m_CameraVec = CameraVec;

	//�v���C���[�̈ʒu�X�V
	m_PlPos = pos;

	//�J�����̌����X�V
	m_CameraRot = rot;

	//�T�C�R�L�l�V�X�G���A�̔��a�̍X�V
	m_fRadius = fRadius;

	//�T�C�R�L�l�V�X�ŕ��������̍X�V
	m_fFloatingHeight = fFloatingHeight;

	//�͈͓��̊��I�̐F�ς�
	BallastWithinRangeColor();

	//�X�V����
	Update();

	//�}�l�[�W���[�̎擾
	CManager *pManager = GetManager();

	//���̃|�C���^
	CMeshfield* pMeshfield = nullptr;

	//���I�}�l�[�W���[�̃|�C���^
	CBallast_Manager* pBallast_Manager = nullptr;

	//�`���[�g���A���̃|�C���^
	CTutorial* pTutorial = nullptr;

	//�Q�[���̃|�C���^
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//�`���[�g���A���̎擾
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//���̎擾
		pMeshfield = pTutorial->GetMeshfield();

		//���I�}�l�[�W���[�̎擾
		pBallast_Manager = pTutorial->GetBallast_Manager();
		break;

	case TYPE_GAME:

		//�Q�[���̎擾
		pGame = (CGame*)pManager->GetGameObject();

		//���̎擾
		pMeshfield = pGame->GetMeshfield();
	
		//���I�}�l�[�W���[�̎擾
		pBallast_Manager = pGame->GetBallast_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//�}�b�v�̉��s�Ƀ��b�V����
	int nDepthGrid = pMeshfield->GetMeshZ();

	//�����蔻����`�F�b�N���郁�b�V��
	int aMapGrid[9];

	//�v���C���[�̂��郁�b�V��
	int nPlMapGrid = nPlMap - nDepthGrid;

	//�v���C���[�̂��郁�b�V���������W�ӏ��̊���o��
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		aMapGrid[nCnt * 3] = nPlMapGrid + nDepthGrid * nCnt - 1;
		aMapGrid[nCnt * 3 + 1] = nPlMapGrid + nDepthGrid * nCnt;
		aMapGrid[nCnt * 3 + 2] = nPlMapGrid + nDepthGrid * nCnt + 1;
	}

	//�w��͈͂̊��I�̓����蔻��
	for (int nCnt = 0; nCnt < 9; nCnt++)
	{
		pBallast_Manager->WithinRangeColor(aMapGrid[nCnt], pos, fRadius, m_nAdrenalineValue);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CPsychokinesis::Draw()
{

}

//*****************************************************************************
// ���V����
//*****************************************************************************
void CPsychokinesis::SetFloating()
{
	//Sin�p�J�E���g
	int nCnt = 0;

	//DestPos�̐ݒ�
	for (auto itr = m_BallastCopyList.begin(); itr != m_BallastCopyList.end(); itr++)
	{
		//Sin�p�J�E���g��i�߂�
		nCnt++;

		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CBallast* pBallast = *itr;

		//���INULL�`�F�b�N
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//���Ԃ��T�C���J�[�u�Ɏg�p
		int nTime = (int)(time(NULL) % TIME) + nCnt;

		//�㉺������T�C���J�[�u
		D3DXVECTOR3 Add = D3DXVECTOR3(0.0f, sinf((float)nTime) * 2.0f, 0.0f);
		pBallast->AddPositionWithPlayer(Add);

		//�ꎞ�ۊǏꏊ
		D3DXVECTOR3 Pos, Rot, DestPos, move, PositionWithPlayer;

		//�v�Z�p�̃}�g���b�N�X
		D3DXMATRIX mtxRot;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxRot);

		Rot = pBallast->GetFacingWithPlayer();

		Rot = CConvenience_Function::NormalizationRot(m_CameraRot - Rot);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);

		//�v���C���[�Ƃ̈ʒu
		PositionWithPlayer = pBallast->GetPositionWithPlayer();

		//�}�g���b�N�X�ϊ�
		D3DXVec3TransformCoord(&DestPos, &PositionWithPlayer, &mtxRot);

		//���I�̌��݂̈ʒu
		Pos = pBallast->GetPos();

		//�ړ��ʂ̌v�Z
		move = (DestPos - Pos) * 0.01f;

		//���I��Move��ύX
		pBallast->SetPosMove(move);

		//�v���C���[�̈ʒu�̓o�^
		pBallast->SetParentPos(m_PlPos);
	}
}

//*****************************************************************************
// �T�C�R�L�l�V�X����
//*****************************************************************************
void CPsychokinesis::Psychokinesis()
{
	//���V�\��
	if (m_nCurrentFloatingNumber > (int)m_nAdrenalineValue * 0.1f)
	{
		return;
	}

	//���I�̃|�C���^
	CBallast* pBallast = nullptr;

	//�}�l�[�W���[�̎擾
	CManager *pManager = GetManager();

	//���I�}�l�[�W���[�̃|�C���^
	CBallast_Manager* pBallast_Manager = nullptr;

	//�`���[�g���A���̃|�C���^
	CTutorial* pTutorial = nullptr;

	//�Q�[���̃|�C���^
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//�`���[�g���A���̎擾
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//���I�}�l�[�W���[�̎擾
		pBallast_Manager = pTutorial->GetBallast_Manager();
		break;

	case TYPE_GAME:

		//�Q�[���̎擾
		pGame = (CGame*)pManager->GetGameObject();

		//���I�}�l�[�W���[�̎擾
		pBallast_Manager = pGame->GetBallast_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//���I�}�l�[�W���[����Ώۂ̊��I�|�C���^�̎擾
	pBallast = pBallast_Manager->CheckCircleCollision(m_PlPos, m_fRadius, m_nAdrenalineValue);

	//NULL�`�F�b�N
	if (pBallast == nullptr)
	{
		return;
	}

	//�T�E���h
	PlaySound(SOUND_LABEL_SE_FLOATING);

	//���V���̑���
	m_nCurrentFloatingNumber++;

	//�ꎞ�ۊǏꏊ
	D3DXVECTOR3 Pos, DestPos, move;

	//�v���C���[�܂ł̈ʒu
	D3DXVECTOR3 PositionWithPlayer;
	PositionWithPlayer = pBallast->GetParentPos() - m_PlPos;

	//�ʒu�̐ݒ�
	pBallast->SetPos(PositionWithPlayer);

	//�J�����̌����̕ۑ�
	pBallast->SetFacingWithPlayer(m_CameraRot);

	//Y�l�̒���
	PositionWithPlayer.y = m_fFloatingHeight * 3.0f;

	//�v���C���[�Ƃ̈ʒu�ݒ�
	pBallast->SetPositionWithPlayer(PositionWithPlayer);

	//���I�̌��݂̈ʒu
	Pos = pBallast->GetParentPos();

	//�ړ��ʂ̌v�Z
	move = (PositionWithPlayer - Pos) * 0.01f;

	//���I��Move��ύX
	pBallast->SetPosMove(move);

	//�v���C���[�̈ʒu�̓o�^
	pBallast->SetParentPos(m_PlPos);

	// ���ݎ����̏��ŏ�����
	srand((unsigned int)time(NULL));

	//��]�̐ݒ�
	int nRand = rand() % 8 + 1;
	float fRadianMove = (nRand * 0.1f) + 0.2f;
	D3DXVECTOR3 rotmove = D3DXVECTOR3(0.0f, D3DXToRadian(-fRadianMove), 0.0f);
	pBallast->SetMoveRot(rotmove);

	//���V��Ԃ̐ݒ�
	pBallast->SetFloating(true);

	//���X�g�ɒǉ�
	m_BallastCopyList.push_back(pBallast);

	//���X�g�ɒǉ�
	pBallast_Manager->SetFloatingBallst(pBallast);
}

//*****************************************************************************
// �T�C�R�L�l�V�X�U������
//*****************************************************************************
void CPsychokinesis::PsychokinesisAttack()
{
	//Move�̐ݒ�
	for (auto itr = m_BallastCopyList.begin(); itr != m_BallastCopyList.end();)
	{
		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CBallast* pBallast = *itr;

		//���I�𓊂���
		if (pBallast != nullptr)
		{
			//�e���̃��Z�b�g
			D3DXVECTOR3 WorldPos = pBallast->GetWorldPos();
			pBallast->SetParentPos(WorldPos);
			pBallast->SetPos(D3DXVECTOR3(0.0f,0.0f,0.0f));
			pBallast->SetPosMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//pBallast->SetMoveRot(D3DXVECTOR3(D3DXToRadian(-10), 0.0f, 0.0f));

			//�����������ւ̈ړ��ʂ̎Z�o
			D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			//�v�Z�p�̃}�g���b�N�X
			D3DXMATRIX mtxRot;

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxRot);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_CameraVec.y, m_CameraVec.x, m_CameraVec.z);

			D3DXVec3TransformCoord(&Move,&Move,&mtxRot);

			Move = Move * 50.0f;

		  /*Move.x = sinf(m_CameraRot.y) * -50.0f;
			Move.z = cosf(m_CameraRot.y) * -50.0f;*/

			//�ړ��ʂ̐ݒ�
			pBallast->SetPosMove(Move);

			//���V��Ԃ̐ݒ�
			pBallast->SetFloating(false);

		}

		//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
		itr = m_BallastCopyList.erase(itr);

		//���V���̌���
		m_nCurrentFloatingNumber--;

		//�T�E���h
		PlaySound(SOUND_LABEL_SE_ATTACK);
	}

	//���V���̒���
	if (m_nCurrentFloatingNumber < 0)
	{
		m_nCurrentFloatingNumber = 0;
	}
}

//*****************************************************************************
// �͈͓��̊��I�̐F�ς�
//*****************************************************************************
void CPsychokinesis::BallastWithinRangeColor()
{


}
