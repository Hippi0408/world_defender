//=============================================================================
//
// ElasticityPolygon.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ELASTICITYPOLYGON_H_			//���̃}�N����`������Ȃ�������
#define _ELASTICITYPOLYGON_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CElasticityPolygon : public CObject
{
	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

public:
	CElasticityPolygon();//�R���X�g���N�^
	virtual ~CElasticityPolygon() override;//�f�X�g���N�^

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	virtual void Draw() override;//�`�揈��

	virtual bool IsUnused() { return true; }//�I������

	void SetPolygon();//�X�V�����O�ł̃Z�b�g�|���S��
	void SetDiagonalLine(float fwidth, float fheight);//�c�����̐ݒ�
	bool Collision(D3DXVECTOR3 pos, float radius);//�����蔻��

	void SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot) { m_Pos = pos; m_Rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void AddPos(D3DXVECTOR3 pos) { m_Pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void SetColor(D3DXCOLOR col) { m_Col = col; }
	void SetTextIndex(int nIndex) { m_nTextIndex = nIndex; }
	void SetFlashing(int nflashing) { m_nFlashingCnt = nflashing; }
	void SetFadeSpeed(float fspeed) { m_fFadeSpeed = fspeed; }

	void SetHeightElasticity(float fHeightElasticity);//�L�т�|���S���c���̃Z�b�g
	void SetHeightMax(float fHeightMax) { m_fHeightMax = fHeightMax; }//�L�т�|���S���c��Max�̃Z�b�g
	float GetHeightMax() { return m_fHeightMax; }
	float GetHeight() { return m_fHeight; }
	void AddHeightElasticity(float fHeightElasticity) { m_fHeightElasticity += fHeightElasticity; }//�L�т�|���S���̉���
	float GetHeightElasticity() { return m_fHeightElasticity; }//���݂̒���

	D3DXVECTOR2 GetUVSizeMax() { return m_UVSizeMax; }
	D3DXVECTOR2 GetUVSizeMin() { return m_UVSizeMin; }

	void SetUVSizeMax(D3DXVECTOR2 UVSize) { m_UVSizeMax = UVSize; }
	void SetUVSizeMin(D3DXVECTOR2 UVSize) { m_UVSizeMin = UVSize; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	D3DXCOLOR m_Col;
	D3DXVECTOR2 m_UVSizeMax;
	D3DXVECTOR2 m_UVSizeMin;

	float m_fWidth;//����
	float m_fHeight;//�c��

	float m_fHeightMax;//�c��

	float m_fHeightElasticity;//�L�k����T�C�Y�i���̐L�k�j
	
	float m_lenTop;//�Ίp���̒���Top
	float m_lenUnder;//�Ίp���̒���Under
	
	float m_angleTop;//�Ίp���̊p�xTop
	float m_angleUnder;//�Ίp���̊p�xUnder
	bool m_bFadeInOut;//�t�F�[�h���(true = in,false = out)
	int m_nFlashingCnt;
	float m_fFadeSpeed;
	int m_nTextIndex;

};

#endif