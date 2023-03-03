//=============================================================================
//
// adrenaline_item.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "adrenaline_item.h"
#include <assert.h>
#include "convenience_function.h"
#include "read.h"

const float CAdrenalineItem::RADIUS = 100.0f;

//�ÓI�����o�[�ϐ�
int CAdrenalineItem::m_nModelNum;
D3DXVECTOR3 CAdrenalineItem::m_LightVec;
std::list<CAdrenalineItem*> CAdrenalineItem::m_AdrenalineItemList;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAdrenalineItem::CAdrenalineItem()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAdrenalineItem::~CAdrenalineItem()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CAdrenalineItem::Init()
{
	//������
	if (FAILED(C3DObject::Init()))
	{
		assert(false);
	}
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CAdrenalineItem::Uninit()
{
	//�I������
	C3DObject::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CAdrenalineItem::Update()
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += D3DXToRadian(5.0f);

	rot = CConvenience_Function::NormalizationRot(rot);

	SetRot(rot);

	//�A�C�e���̍X�V����
	C3DObject::Update();
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CAdrenalineItem::Draw()
{
	//�}�g���b�N�X�̌v�Z
	CalculationMatrix();
	//�A�C�e���̕`�揈��
	C3DObject::Draw();
}

//*****************************************************************************   
// ������
//*****************************************************************************
HRESULT CAdrenalineItem::AllInit()
{
	CRead Read;

	m_nModelNum = Read.ReadXFile("data/MODEL/�A�h���i����.x");

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CAdrenalineItem::AllUninit()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//�C�e���[�^�[����A�C�e���̃|�C���^�̑��
		CAdrenalineItem* pAdrenalineItem = *itr;

		//�A�C�e���|�C���^�̉��
		if (pAdrenalineItem != nullptr)
		{
			//�I������
			pAdrenalineItem->Uninit();
			delete pAdrenalineItem;
			pAdrenalineItem = nullptr;
		}

		//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
		itr = m_AdrenalineItemList.erase(itr);
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CAdrenalineItem::AllUpdate()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//�C�e���[�^�[����A�C�e���̃|�C���^�̑��
		CAdrenalineItem* pAdrenalineItem = *itr;

		//�A�C�e���|�C���^�̉��
		if (pAdrenalineItem == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_AdrenalineItemList.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�A�C�e���̍X�V����
		pAdrenalineItem->Update();

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CAdrenalineItem::AllDraw()
{

	//�C�e���[�^�[���[�v
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//�C�e���[�^�[����A�C�e���̃|�C���^�̑��
		CAdrenalineItem* pAdrenalineItem = *itr;

		//�A�C�e���|�C���^�̉��
		if (pAdrenalineItem == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_AdrenalineItemList.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�A�C�e���̍X�V����
		pAdrenalineItem->Draw();


		//�C�e���[�^�[��i�߂�
		itr++;
	}

}

//*****************************************************************************
// �N���G�C�g����
//*****************************************************************************
void CAdrenalineItem::DropAdrenalineItem(D3DXVECTOR3 pos)
{
	//�A�C�e���N���X�̃|�C���^�ϐ�
	CAdrenalineItem* pAdrenalineItem = nullptr;

	//���I�m��
	pAdrenalineItem = new CAdrenalineItem;
	
	//������
	if (FAILED(pAdrenalineItem->Init()))
	{
		assert(false);
	}

	//�e�̐ݒ�
	pAdrenalineItem->SetLightVec(m_LightVec);

	//�ׂ����ݒ�
	pAdrenalineItem->Set3DObject(m_nModelNum, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�A�C�e���̈ʒu
	pAdrenalineItem->SetParentPos(pos);

	//���X�g�ɃA�C�e������ǉ�
	m_AdrenalineItemList.push_back(pAdrenalineItem);
}

//*****************************************************************************
// �v���C���[�Ƃ̂̓����蔻�菈��
//*****************************************************************************
bool CAdrenalineItem::PlayerCollision(D3DXVECTOR3 pos)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_AdrenalineItemList.begin(); itr != m_AdrenalineItemList.end();)
	{
		//�C�e���[�^�[����A�C�e���̃|�C���^�̑��
		CAdrenalineItem* pAdrenalineItem = *itr;

		//�A�C�e���|�C���^�̉��
		if (pAdrenalineItem == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_AdrenalineItemList.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//����
		bool bHit = CConvenience_Function::CircleCollision(pAdrenalineItem->GetParentPos(), RADIUS, pos, 0.0f);

		//��L�̌��ʂ�true
		if (bHit)
		{
			//�A�C�e���|�C���^�̉��
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_AdrenalineItemList.erase(itr);

			//�I������
			pAdrenalineItem->Uninit();
			delete pAdrenalineItem;
			pAdrenalineItem = nullptr;

			//�����𔲂���
			return true;
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}

	return false;
}
