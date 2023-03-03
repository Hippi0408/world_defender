//=============================================================================
//
// motion_parts.cpp
// Author : kozuna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "motion_parts.h"
#include "manager.h"
#include <assert.h>
#include "convenience_function.h"
#include "object_type_list.h"

const D3DXVECTOR3 CMotionParts::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CMotionParts::m_nModelMax = 0;
std::list<CMotionParts*> CMotionParts::m_MotionPartslist;			 //�S�Ẵp�[�c�̃��X�g
std::map<int, int> CMotionParts::m_nMotionPlayMotonNum;			//���Đ����̃��[�V�����ԍ�
std::map<int, int> CMotionParts::m_nMotionDestMotonNum;			//���Đ����[�V�����ԍ�
std::map<int, int> CMotionParts::m_nMotionRegistrationNum;			//�o�^�������[�V�������f���Q�̃��[�V�����̓o�^��

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CMotionParts::CMotionParts()
{
	m_nFrame = 0;
	m_nKey = 0;
	m_pParent = nullptr;
	

	m_nModelObjNum = m_nModelMax;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CMotionParts::~CMotionParts()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CMotionParts::Init()
{
	if (FAILED(C3DObject::Init()))
	{
		return -1;
	}
	m_nDestFrame = 0;
	m_bTopUnUpdate = false;
	m_bUnUpdate = true;
	m_bDraw = true;
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CMotionParts::Uninit()
{
	for (int nMotion = 0; nMotion < m_nMotionRegistrationNum[m_nModelObjNum]; nMotion++)
	{
		if (m_MotionKey[nMotion].pKey != nullptr)
		{
			delete[] m_MotionKey[nMotion].pKey;
			m_MotionKey[nMotion].pKey = nullptr;
		}
	}

	C3DObject::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CMotionParts::Update()
{
	// �ړI�̃t���[��
	m_nDestFrame = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey].nFrame;

	// �t���[���̉��Z
	m_nFrame++;
	// �ړI�̃t���[���@���@���݂̃t���[��
	if (m_nDestFrame < m_nFrame)
	{
		// ���̈ʒu�܂ł̌v�Z
		NextMotionPosition();
		//�L�[���̊m�F
		KeyCheck();
		
		m_nFrame = 0;
	}

	// pos�̉��Z
	AddPosMove(m_PosMove);
	// rot�̉��Z
	AddRot(m_RotMove);

	D3DXVECTOR3 rot = GetRot();

	//���K��
	rot = CConvenience_Function::NormalizationRot(rot);

	SetRot(rot);


}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CMotionParts::Draw()
{
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtx;
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();
	// �e�����݂���ꍇ
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxParent);
	}
	
	//���g�̃}�g���b�N�X�̌v�Z
	CalculationMatrix();
	mtx = GetMatrix();

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
	D3DXMatrixMultiply(&mtx, &mtx, &mtxParent);

	SetMatrix(mtx);

	C3DObject::Draw();
}

//*****************************************************************************
//	�I����������
//*****************************************************************************
bool CMotionParts::IsUnused()
{
	return false;
}

//*****************************************************************************
//	���[�V�����ԍ��w��ŃZ�b�g����
//*****************************************************************************
void CMotionParts::SetMotion(int nMotionNum)
{
	m_nFrame = 0;
	m_nKey = 0;
	D3DXVECTOR3 pos = m_MotionKey[nMotionNum].pKey[m_nKey].pos;
	D3DXVECTOR3 rot = m_MotionKey[nMotionNum].pKey[m_nKey].rot;

	// �ړI�̃t���[��
	m_nDestFrame = m_MotionKey[nMotionNum].pKey[m_nKey].nFrame;

	m_PosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetParentPos(pos);
	SetRot(rot);
}

//*****************************************************************************
// ���[�V�����̈ړ��ʂȂǂ̃N���A
//*****************************************************************************
void CMotionParts::ClearMotionMove()
{
	m_nFrame = 0;
	m_nKey = 0;
	m_PosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// �ړI�̃t���[��
	m_nDestFrame = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey].nFrame;

}

//*****************************************************************************
//���ۂ̓����̓o�^
//*****************************************************************************
void CMotionParts::SetMotionData(KEY_SET KeyData)
{
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].bLoop = KeyData.bLoop;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].nKeyMax = KeyData.nKeyMax;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].pKey = KeyData.pKey;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].nNextMotionNum = KeyData.nNextMotionNum;
}

//*****************************************************************************
// �L�[���̊m�F
//*****************************************************************************
void CMotionParts::KeyCheck()
{
	//�L�[�̃J�E���g��i�߂�
	m_nKey++;

	//�ړI�̃L�[��
	int nDestKey = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].nKeyMax;

	//�L�[���K���葽���ꍇ���Z�b�g
	if (m_nKey >= nDestKey)
	{
		m_nKey = 0;

		//���̃��[�V�����ԍ�
		int nNowMotionNum = m_nMotionPlayMotonNum[m_nModelObjNum];

		//���ɍĐ����郂�[�V�����ԍ��i��O�̏ꍇ�Đ����Ă��ԍ��ɂ���j
		int nNextMotionNum = nNowMotionNum;

		//���[�v����ꍇ����
		if (!m_MotionKey[nNowMotionNum].bLoop)
		{
			//���ɍĐ����郂�[�V�����ԍ�
			nNextMotionNum = m_MotionKey[nNowMotionNum].nNextMotionNum;

			//���݂̃p�[�c���p�[�c�ԍ����O���ǂ���
			if (m_nPartsNum == 0)
			{
				m_nMotionPlayMotonNum[m_nModelObjNum] = 0;

				ClearMotionMove();

				AllNextMotionPosition(m_nModelObjNum,m_bTopUnUpdate);
			}
			else
			{
				ClearMotionMove();
			}
		}
	}
}

//*****************************************************************************
// ���̈ʒu�܂ł̌v�Z
//*****************************************************************************
void CMotionParts::NextMotionPosition()
{
	D3DXVECTOR3 nowPos, nextPos,nowRot, nextRot;
	int nFrameRatio;
	//�ړI�̃L�[��
	int nDestKey = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].nKeyMax;
	
	//���݂�KEY���ړI�𒴂�����
	if (m_nKey >= nDestKey - 1)
	{
		//���̈ʒu
		nowPos = GetPosMove();
		//���̈ʒu
		nextPos = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].pos;
		//���̌���
		nowRot = GetRot();
		//���̌���
		nextRot = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].rot;
		//�S�̃t���[�����ɑ΂��Ă̊���
		nFrameRatio = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].nFrame;
	}
	else
	{
		//���̈ʒu
		nowPos = GetPosMove();
		//���̈ʒu
		nextPos = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].pos;
		//���̌���
		nowRot = GetRot();
		
		//�S�̃t���[�����ɑ΂��Ă̊���
		nFrameRatio = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].nFrame;

		//���̌���
		nextRot = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].rot;
	}
	
	//1�t���[��������̓�������
	D3DXVECTOR3 rotMove;
	//1�t���[��������̓����ʒu
	D3DXVECTOR3 posMove;
	
	//�ړI�̈ʒu�@�[�@���݂̈ʒu
	D3DXVECTOR3 pos = nextPos - nowPos;

	//�ړI�̌����@�[�@���݂̌���
	D3DXVECTOR3 rot = nextRot - nowRot;

	//���K��
	rot = CConvenience_Function::NormalizationRot(rot);

	//�����v�Z
	//�ʒu
	posMove = pos / (float)nFrameRatio;
	m_PosMove = posMove;
	//����
	rotMove = CConvenience_Function::NormalizationRot(rot / (float)nFrameRatio) ;

	//���K��
	rotMove = CConvenience_Function::NormalizationRot(rotMove);

	m_RotMove = rotMove;
}

//*****************************************************************************
//�S���̃p�[�c���̈ʒu�܂ł̌v�Z
//*****************************************************************************
void CMotionParts::AllNextMotionPosition(int nMotionNum, bool bUnUpdate)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		if (pMotionParts->GetModelObjNum() == nMotionNum)
		{
			if (bUnUpdate)
			{
				pMotionParts->SetUnUpdate(false);
			}
			else
			{
				pMotionParts->ClearMotionMove();
				pMotionParts->NextMotionPosition();
			}

		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
// �����Ƃ̈�v�����邩�ǂ���
//*****************************************************************************
bool CMotionParts::GetMotionParts(int nMotionNum, int nPartsNum)
{
	if (m_nModelObjNum == nMotionNum
		&& m_nPartsNum == nPartsNum)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// �����Ƃ̈�v�����邩�ǂ���
//*****************************************************************************
bool CMotionParts::GetMotionParts(int nMotionNum)
{
	if (m_nModelObjNum == nMotionNum)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
//�X�V�ƕ`��̃I���I�t
//*****************************************************************************
void CMotionParts::UnUpdateDraw(int nMotionNum, bool bUnUpdate, bool bDraw)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//���[�V�������f���̍X�V�ƕ`��̗L��
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			//�X�V
			pMotionParts->SetUnUpdate(bUnUpdate);
			//�`��
			pMotionParts->SetBoolDraw(bDraw);

		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
// ALL�I������
//*****************************************************************************
void CMotionParts::ALLUninit()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts != nullptr)
		{
			//�I������
			pMotionParts->Uninit();
			delete pMotionParts;
			pMotionParts = nullptr;
		}

		//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
		itr = m_MotionPartslist.erase(itr);
	}
}

//*****************************************************************************
// ALL�X�V����
//*****************************************************************************
void CMotionParts::ALLUpdate()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[����G�l�~�[�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�X�V���������Ȃ�
		if (!pMotionParts->GetUnUpdate())
		{
			//�C�e���[�^�[��i�߂�
			itr++;

			//�ȉ��̏����𖳎�����
			continue;
		}

		//���[�V�����p�[�c�̍X�V����
		pMotionParts->Update();

		//�I����������
		if (pMotionParts->IsUnused())
		{
			//�I������
			pMotionParts->Uninit();
			delete pMotionParts;
			pMotionParts = nullptr;

			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
// ALL�`�揈��
//*****************************************************************************
void CMotionParts::ALLDraw()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[����G�l�~�[�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�`������Ȃ�
		if (!pMotionParts->GetBoolDraw())
		{
			//�C�e���[�^�[��i�߂�
			itr++;

			//�ȉ��̏����𖳎�����
			continue;
		}
		//���[�V�����p�[�c�̍X�V����
		pMotionParts->Draw();

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
// ����OBJ�̐���
//*****************************************************************************
int CMotionParts::CreateMotionObj(MotionData* pMotionData, int nPartsMax)
{
	for (int nCnt = 0; nCnt < nPartsMax; nCnt++)
	{
		CMotionParts* pMotionParts = new CMotionParts;

		if (FAILED(pMotionParts->Init()))
		{
			assert(false);
		}

		pMotionParts->SetPartsNum(nCnt);//��̃��[�V�����̒��̔ԍ�

		pMotionParts->Set3DObject(pMotionData[nCnt].nModelPattern, pMotionData[nCnt].pos, pMotionData[nCnt].rot);
		
		if (pMotionData[nCnt].nParentNum >= 0)
		{
			//�e�̐ݒ�
			pMotionParts->SetMotionParent(GetMotionPartsPointer(m_nModelMax, pMotionData[nCnt].nParentNum));
		}
		
		pMotionParts->SetNormal();

		//���X�g�ɃG�l�~�[����ǉ�
		m_MotionPartslist.push_back(pMotionParts);
	}

	int nModelNum = m_nModelMax;

	//�������̂̓o�^����
	SettingCompletion();

	//���[�V�������f���̍Đ��ɕK�v�ȏ��̏�����
	m_nMotionPlayMotonNum[nModelNum] = 0;
	m_nMotionDestMotonNum[nModelNum] = 0;

	return nModelNum;

}

//*****************************************************************************
// �����ɍ����|�C���^��Ԃ�
//*****************************************************************************
CMotionParts * CMotionParts::GetMotionPartsPointer(int nMotionNum, int nPartsNum)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		// �����ɍ����|�C���^��Ԃ�
		if (pMotionParts->GetMotionParts(nMotionNum, nPartsNum))
		{
			return pMotionParts;
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}

	return nullptr;
}

//*****************************************************************************
//���[�V�������f���̈ړ�
//*****************************************************************************
void CMotionParts::MoveMotionModel(int nModelNum, int nMotionNum, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bUnUpdate)
{
	CMotionParts* pMotionParts = CMotionParts::GetMotionPartsPointer(nModelNum, 0);

	pMotionParts->SetTopUnUpdate(bUnUpdate);

	if (pMotionParts->GetMotionData().bLoop)
	{
		if (m_nMotionPlayMotonNum[nModelNum] != nMotionNum)
		{
			m_nMotionPlayMotonNum[nModelNum] = nMotionNum;
			//�S�̂̎��܂ł̈ʒu�̌v�Z
			AllNextMotionPosition(nModelNum);
		}
	}

	if (pos == rot)
	{
		return;
	}

	if (pos != nullptr)
	{
		GetMotionPartsPointer(nModelNum, 0)->SetParentPos(*pos);
	}

	if (rot != nullptr)
	{
		GetMotionPartsPointer(nModelNum, 0)->SetParentRot(*rot);
	}

}

//*****************************************************************************
//���[�V�������f���̕`��̗L��
//*****************************************************************************
void CMotionParts::SetBoolDraw(bool bDraw, int nMotionNum)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//���[�V�������f���̕`��̗L��
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			pMotionParts->SetBoolDraw(bDraw);
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
//���[�V�������f���̃��C�g�x�N�g��
//*****************************************************************************
void CMotionParts::SetLight(D3DXVECTOR3 vec, int nMotionNum)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//���[�V�������f���̃��C�g�x�N�g��
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			pMotionParts->SetLightVec(vec);
		}
		
		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
//�e�̐ݒ�
//*****************************************************************************
void CMotionParts::AllSetShadowPos(D3DXVECTOR3 pos, int nMotionNum)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�e�̐ݒ�
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			D3DXVECTOR3 ShadowPos = pMotionParts->GetWorldPos();
			ShadowPos.y = pos.y;
			pMotionParts->SetShadowPos(ShadowPos);
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
//���[�V�����̓o�^
//*****************************************************************************
void CMotionParts::SetMotionFileData(const MotionMoveData MotionMoveData, int nMotionNum)
{
	CMotionParts* pMotionParts = nullptr;//�ꎞ�ۊǏꏊ
	int nPartsNum = 0;//�J�E���^�[
	pMotionParts = GetMotionPartsPointer(nMotionNum, nPartsNum);//�����ɍ����|�C���^�̊l��
	while (pMotionParts != nullptr)
	{
		//�ꎞ�ۊǏꏊ
		KEY_SET KeySet;

		KeySet.pKey = new KEY[MotionMoveData.nKeyMax];

		for (int nCnt = 0; nCnt < MotionMoveData.nKeyMax; nCnt++)
		{
			//�t���[��
			KeySet.pKey[nCnt].nFrame = MotionMoveData.pMotionKeyData[nCnt].nFrame;
			//Pos
			KeySet.pKey[nCnt].pos = MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData[nPartsNum].pos;
			//Rot
			KeySet.pKey[nCnt].rot = MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData[nPartsNum].rot;
		}

		//���[�v�ݒ�
		KeySet.bLoop = MotionMoveData.bLoop;
		//�L�[�̍ő�
		KeySet.nKeyMax = MotionMoveData.nKeyMax;
		//���ɍĐ����郂�[�V�����ԍ�
		KeySet.nNextMotionNum = MotionMoveData.nNextMotionNum;

		//�o�^
		pMotionParts->SetMotionData(KeySet);

		nPartsNum++;//�J�E���^�[��i�߂�
		pMotionParts = GetMotionPartsPointer(nMotionNum, nPartsNum);//�����ɍ����|�C���^�̊l��
	}
	
	//���[�V�����̓o�^������
	m_nMotionRegistrationNum[nMotionNum]++;
}

//*****************************************************************************
//�����蔻��i�����̃��f���ԍ��A�����̈ʒu�j
//*****************************************************************************
bool CMotionParts::AllCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, Object_Type_List myobject_type_list)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�����蔻��i�����̃��f���ԍ��A�����̈ʒu�j
		Object_Type_List object_type_list = pMotionParts->GetObject_Type_List();
		if (object_type_list == myobject_type_list)
		{
			if (pMotionParts->NormalCollision(pos))
			{
				return true;
			}

		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}

	return false;
}

//*****************************************************************************
//�e����V�I�ɐݒ肷��i�q���ԍ��A�e�ԍ��j
//*****************************************************************************
void CMotionParts::SettingParent(int nChildren, int nParent)
{
	if (nChildren == nParent)
	{
		return;
	}

	CMotionParts* pMotionPartsChildren = nullptr;
	CMotionParts* pMotionPartsParent = nullptr;

	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�e����V�I�ɐݒ肷��i�q���ԍ��A�e�ԍ��j
		if (pMotionParts->GetMotionParts(nChildren, 0))
		{
			pMotionPartsChildren = pMotionParts;
		}

		if (pMotionParts->GetMotionParts(nParent, 0))
		{
			pMotionPartsParent = pMotionParts;
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}

	if (pMotionPartsChildren == nullptr || pMotionPartsParent == nullptr)
	{
		return;
	}

	if (pMotionPartsChildren->GetMotionParent() == nullptr)
	{
		pMotionPartsChildren->SetMotionParent(pMotionPartsParent);
	}
}

//*****************************************************************************
//���f����������ݒ肷��
//*****************************************************************************
void CMotionParts::AllSetObject_Type_List(int nModelNum, Object_Type_List object_type_list)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[���烂�[�V�����p�[�c�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_MotionPartslist.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//���f����������ݒ肷��
		int nNum = pMotionParts->GetModelObjNum();
		if (nNum == nModelNum)
		{
			pMotionParts->SetObject_Type_List(object_type_list);

		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
//���[�V�����f�[�^�̕���
//*****************************************************************************
int CMotionParts::CopyMotionModel(int nModelNum)
{
	int nPartsNum = 0;//�J�E���^�[
	
	//�Ώۂ̃��[�V�������f���|�C���^
	CMotionParts* pTargetMotionParts = nullptr;

	//������
	pTargetMotionParts = GetMotionPartsPointer(nModelNum, nPartsNum);//�����ɍ����|�C���^�̊l��

	//�p�[�c�̕���
	while (pTargetMotionParts != nullptr)
	{
		CMotionParts* pMotionParts = new CMotionParts;

		if (FAILED(pMotionParts->Init()))
		{
			assert(false);
		}

		pMotionParts->SetPartsNum(nPartsNum);//��̃��[�V�����̒��̔ԍ�


		int nModelPattern = pTargetMotionParts->GetModelPattnNum();
		D3DXVECTOR3 pos = pTargetMotionParts->GetPos();
		D3DXVECTOR3 rot = pTargetMotionParts->GetRot();

		//��{���̕ۑ�
		pMotionParts->Set3DObject(nModelPattern, pos, rot);

		D3DXVECTOR3 vec = pTargetMotionParts->GetLightVec();
		//�e�p�ݒ�
		pMotionParts->SetLightVec(vec);

		//�e�̐ݒ�(�e�����݂���Ȃ�)
		if (nPartsNum > 0)
		{
			//�e�ԍ��̎擾
			int nParent = pTargetMotionParts->GetMotionParent()->GetPartsNum();

			//�e�̐ݒ�
			pMotionParts->SetMotionParent(GetMotionPartsPointer(m_nModelMax, nParent));
		}

		//���[�V�����f�[�^�̔ԍ�
		pMotionParts->SetModelObjNum(m_nModelMax);

		//�@���ݒ�
		pMotionParts->SetNormal();

		//���X�g�Ƀ��[�V�������f������ǉ�
		m_MotionPartslist.push_back(pMotionParts);

		//�J�E���^�[��i�߂�
		nPartsNum++;

		//�Ώۂ̃��[�V�������f���|�C���^
		pTargetMotionParts = GetMotionPartsPointer(nModelNum, nPartsNum);

	}

	//�o�^�������[�V�������̏�����
	m_nMotionRegistrationNum[m_nModelMax] = 0;

	//�ꎞ�ۊǏꏊ
	CMotionParts* pMotionParts = nullptr;

	//�����̕���
	nPartsNum = 0;//�J�E���^�[

	//������
	pMotionParts = GetMotionPartsPointer(m_nModelMax, nPartsNum);//�����ɍ����|�C���^�̊l��

	//���f���f�[�^���̐ݒ�
	while (pMotionParts != nullptr)
	{
		//�R�s�[���̃��[�V�����f�[�^
		CMotionParts* pSourceInformationMotionParts = GetMotionPartsPointer(nModelNum, nPartsNum);
		

		//���[�V�����f�[�^���̐ݒ�
		for (int nCntMotion = 0; nCntMotion < m_nMotionRegistrationNum[nModelNum]; nCntMotion++)
		{
			//�ꎞ�ۊǏꏊ
			KEY_SET KeySet;

			//�R�s�[���̏��擾
			KEY_SET KeySourceInformation = pSourceInformationMotionParts->GetKeyData(nCntMotion);

			//Key�̍ő吔
			int nKeyMax = KeySourceInformation.nKeyMax;

			//Key���̐���
			KeySet.pKey = new KEY[nKeyMax];

			//Key�f�[�^���̐ݒ�
			for (int nCntKey = 0; nCntKey < nKeyMax; nCntKey++)
			{
				//�t���[��
				KeySet.pKey[nCntKey].nFrame = KeySourceInformation.pKey[nCntKey].nFrame;
				//Pos
				KeySet.pKey[nCntKey].pos = KeySourceInformation.pKey[nCntKey].pos;
				//Rot
				KeySet.pKey[nCntKey].rot = KeySourceInformation.pKey[nCntKey].rot;
			}

			//���[�v�ݒ�
			KeySet.bLoop = KeySourceInformation.bLoop;
			//�L�[�̍ő�
			KeySet.nKeyMax = KeySourceInformation.nKeyMax;
			//���ɍĐ����郂�[�V�����ԍ�
			KeySet.nNextMotionNum = KeySourceInformation.nNextMotionNum;

			//�o�^
			pMotionParts->SetMotionData(KeySet);

			//���[�V�����̓o�^������
			m_nMotionRegistrationNum[m_nModelMax]++;

		}

		nPartsNum++;//�J�E���^�[��i�߂�
		pMotionParts = GetMotionPartsPointer(m_nModelMax, nPartsNum);//�����ɍ����|�C���^�̊l��

		//�o�^�������[�V�������̏�����
		m_nMotionRegistrationNum[m_nModelMax] = 0;
	}

	//���[�V�������f���̍Đ��ɕK�v�ȏ��̏�����
	m_nMotionPlayMotonNum[nModelNum] = 0;
	m_nMotionDestMotonNum[nModelNum] = 0;

	// �o�^�������[�V������
	m_nMotionRegistrationNum[m_nModelMax] = m_nMotionRegistrationNum[nModelNum];

	int nNewModelNum = m_nModelMax;

	//�������̂̓o�^����
	SettingCompletion();

	return nNewModelNum;

}

//*****************************************************************************
//�w��f�[�^�̔j��
//*****************************************************************************
void CMotionParts::DestructionMotionModel(int nModelNum)
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_MotionPartslist.begin(); itr != m_MotionPartslist.end();)
	{
		//�C�e���[�^�[����w��f�[�^�̃|�C���^�̑��
		CMotionParts* pMotionParts = *itr;

		//���[�V�����p�[�c�|�C���^�̉��
		if (pMotionParts == nullptr)
		{
			assert(false);
		}

		//�Ώۂ̏��ł͂Ȃ�������
		if (pMotionParts->GetModelObjNum() != nModelNum)
		{
			//�C�e���[�^�[��i�߂�
			itr++;

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�I������
		pMotionParts->Uninit();
		delete pMotionParts;
		pMotionParts = nullptr;

		//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
		itr = m_MotionPartslist.erase(itr);
	}
		

	//Map�̗v�f��j��
	m_nMotionPlayMotonNum.erase(nModelNum);			//���Đ����̃��[�V�����ԍ�
	m_nMotionDestMotonNum.erase(nModelNum);			//���Đ����[�V�����ԍ�
	m_nMotionRegistrationNum.erase(nModelNum);			//�o�^�������[�V�������f���Q�̃��[�V�����̓o�^��
}
