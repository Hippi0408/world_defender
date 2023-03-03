//=============================================================================
//
// texture.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _TEXTURE_H_			//このマクロ定義がされなかったら
#define _TEXTURE_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexture
{
private:
	static const int TEXTURE_MAX = 32;
public:
	CTexture();
	~CTexture();

	static CTexture *Create();
	static CTexture *GetTextuerIndex() { return m_pTextuerClass; }
	static int LoadTexture(char *address);
	static void Uninit();
	static LPDIRECT3DTEXTURE9 GetTexture(int nindex) { return m_pTexture[nindex]; }

private:
	static int m_nNowIndex;
	static CTexture *m_pTextuerClass;
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];
};

#endif