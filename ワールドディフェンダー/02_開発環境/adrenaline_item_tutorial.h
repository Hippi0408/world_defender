//=============================================================================
//
// adrenaline_item_tutorial.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ADRENALINE_ITEM_TUTORIAL_H_			//���̃}�N����`������Ȃ�������
#define _ADRENALINE_ITEM_TUTORIAL_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "3dobject.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAdrenalineItemTutorial : public C3DObject
{
private:
	static const float RADIUS;
public:
	CAdrenalineItemTutorial();
	~CAdrenalineItemTutorial() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �v���C���[�Ƃ̂̓����蔻�菈��
	bool PlayerCollision(D3DXVECTOR3 pos);
private:
};

#endif