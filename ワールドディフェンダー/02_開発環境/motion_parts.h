//=============================================================================
//
// motion_parts.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _MOTION_PARTS_H_			//���̃}�N����`������Ȃ�������
#define _MOTION_PARTS_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "3dobject.h"
#include <list>
#include <map>

//*****************************************************************************
// �\����`
//*****************************************************************************
// �\����
struct MotionData//���[�V�����̍\��
{
	int nModelPattern;//�g�p���郂�f���̃C���f�b�N
	int nParentNum;//�e�̔ԍ�
	D3DXVECTOR3 pos;//�e����̈ʒu
	D3DXVECTOR3 rot;//�e����̌���
};

// ���[�V�����p�[�c�̃f�[�^
struct MotionPartsData
{//�p�[�c����
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
};

//���[�V�����L�[����̃f�[�^
struct MotionKeyData
{//�L�[����
	MotionPartsData* pMotionPartsData;//�p�[�c���K�v
	int nFrame;
};

// ���[�V���������̃f�[�^
struct MotionMoveData
{//���[�V��������
	MotionKeyData* pMotionKeyData;//�L�[���K�v
	int nKeyMax;
	bool bLoop;
	int nNextMotionNum;		//���̍Đ����郂�[�V�����ԍ�
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMotionParts : public C3DObject
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int MAX_MOTION = 16;
	static const int MAX_KEY = 12;
	static const int MAX_MOTION_ALL = 120;
public:

	// �\����
	struct KEY
	{
		int nFrame;//���݂̃L�[�̎n�܂肩��I���܂łɂ�����t���[����
		D3DXVECTOR3 pos;//���݂̃L�[�̎n�܂肩��I���̈ʒu
		D3DXVECTOR3 rot;//���݂̃L�[�̎n�܂肩��I���̌���
	};

	struct KEY_SET
	{
		KEY* pKey;//�L�[�̍ő�l���K�v
		int nKeyMax;//�L�[�̍ő�
		bool bLoop;//���݂̃��[�V���������[�v�Đ����邩
		int nNextMotionNum;		//���̍Đ����郂�[�V�����ԍ�
	};

	CMotionParts();
	~CMotionParts() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	bool IsUnused();

	void SetMotion(int nMotionNum);
	void ClearMotionMove();
	void SetMotionData(KEY_SET KeyData);//���ۂ̓����̓o�^
	KEY_SET GetMotionData() { return m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]]; }
	void KeyFrameReset() { m_nKey = 0; m_nFrame = 0; }

	int GetModelObjNum() { return m_nModelObjNum; }
	void SetModelObjNum(int nModelObjNum) { m_nModelObjNum = nModelObjNum; }

	void SetPartsNum(int nPartsNum) { m_nPartsNum = nPartsNum; }
	int GetPartsNum() { return m_nPartsNum; }
	void KeyCheck();//�L�[���̊m�F
	void NextMotionPosition();//���̈ʒu�܂ł̌v�Z
	static void AllNextMotionPosition(int nMotionNum, bool bUnUpdate = false);//�S���̃p�[�c���̈ʒu�܂ł̌v�Z
	void SetMotionParent(CMotionParts* pMotionParent) { m_pParent = pMotionParent; }
	CMotionParts* GetMotionParent() { return m_pParent; }
	bool GetMotionParts(int nMotionNum, int nPartsNum);//�����Ƃ̈�v�����邩�ǂ���
	bool GetMotionParts(int nMotionNum);//�����Ƃ̈�v�����邩�ǂ���
	void SetBoolDraw(bool bDraw) { m_bDraw = bDraw; }
	bool GetBoolDraw() { return m_bDraw; }

	//�X�V�ƕ`��̃I���I�t
	static void UnUpdateDraw(int nMotionNum, bool bUnUpdate = false, bool bDraw = false);

	void SetPosMove(D3DXVECTOR3 pos) { m_PosMove = pos; }
	void SetRotMove(D3DXVECTOR3 rot) { m_RotMove = rot; }

	//���݂̃��[�V�������I�������X�V�����Ȃ�����
	bool GetUnUpdate() { return m_bUnUpdate; }
	void SetUnUpdate(bool bUnUpdate) { m_bUnUpdate = bUnUpdate; }
	void SetTopUnUpdate(bool bUnUpdate) { m_bTopUnUpdate = bUnUpdate; }

	//���̃p�[�c�̃��[�V�������̐ݒ�Ǝ擾
	KEY_SET GetKeyData(int nMotion) { return m_MotionKey[nMotion]; }

	static void ALLUninit();//���ׂĂ̏I������
	static void ALLUpdate();//���ׂĂ̍X�V����
	static void ALLDraw();//���ׂĂ̕`�揈��

	static void SettingCompletion() { m_nModelMax++; }//�������̂̓o�^����
	static int CreateMotionObj(MotionData* pMotionData,int nPartsMax);//����OBJ�̐���
	static CMotionParts* GetMotionPartsPointer(int nMotionNum, int nPartsNum);

	static void MoveMotionModel(int nModelNum, int nMotionNum, D3DXVECTOR3 *pos = nullptr, D3DXVECTOR3 *rot = nullptr,  bool bUnUpdate = false);//���[�V�������f���̈ړ�
	static void SetBoolDraw(bool bDraw, int nMotionNum);//���[�V�������f���̕`��̗L��
	static void SetLight(D3DXVECTOR3 vec, int nMotionNum);//���[�V�������f���̃��C�g�x�N�g��
	static void AllSetShadowPos(D3DXVECTOR3 pos, int nMotionNum);//�e�̐ݒ�
	static void SetMotionFileData(const MotionMoveData MotionMoveData, int nMotionNum);//���[�V�����̓o�^
	static bool AllCollision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, Object_Type_List myobject_type_list);//�����蔻��i�����̃��f���ԍ��A�����̈ʒu�j
	static void SettingParent(int nChildren, int nParent);//�e����V�I�ɐݒ肷��i�q���ԍ��A�e�ԍ��j
	static void AllSetObject_Type_List(int nModelNum, Object_Type_List object_type_list);//���f����������ݒ肷��
	static int CopyMotionModel(int nModelNum);//���[�V�����f�[�^�̕���
	static void DestructionMotionModel(int nModelNum);//�w��f�[�^�̔j��

private:
	static std::list<CMotionParts*> m_MotionPartslist;			 //�S�Ẵp�[�c�̃��X�g

	static std::map<int, int> m_nMotionPlayMotonNum;			//���Đ����̃��[�V�����ԍ�
	static std::map<int, int> m_nMotionDestMotonNum;			//���Đ����[�V�����ԍ�
	static std::map<int, int> m_nMotionRegistrationNum;			//�o�^�������[�V�������f���Q�̃��[�V�����̓o�^��

	static int m_nModelMax;//�o�^�����������̂̐�

	CMotionParts* m_pParent;		//�e�̃|�C���^
	KEY_SET m_MotionKey[MAX_MOTION];//�o�^���������̏��
	D3DXVECTOR3 m_RotMove;			//1�t���[��������̓�����(rot)
	D3DXVECTOR3 m_PosMove;			//1�t���[��������̓�����(pos)
	int m_nModelObjNum;				//���[�V�����I�u�W�F�N�g�̔ԍ�
	int m_nFrame;					//���݂̃t���[��
	int m_nDestFrame;
	int m_nKey;						//���݂̃L�[
	int m_nPartsNum;				//�������̂̒��̔ԍ�
	bool m_bDraw;					//�`������邩���Ȃ���
	bool m_bTopUnUpdate;			//���݂̃��[�V�������I�������X�V�����Ȃ�����(Top�p)
	bool m_bUnUpdate;				//���݂̃��[�V�������I�������X�V�����Ȃ�����

};

#endif