//=============================================================================
//
// read.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "read.h"
#include <stdio.h>
#include "manager.h"
#include <assert.h>
#include "motion_parts.h"
#include "texture.h"
#include "meshfield.h"
#include "game.h"
#include "ballast_manager.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CRead::CRead()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CRead::~CRead()
{
}

//*****************************************************************************
// マップの読み込み
//*****************************************************************************
CMeshfield * CRead::ReadMap(char * sFilePath, CBallast_Manager* pBallast_Manager)
{
	//メッシュフィールド用のポインタ宣言
	CMeshfield *pMeshfield = nullptr;

	//モデルインデックス
	int *pModelIndex = nullptr;

	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];		//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];	//頭の文字を読み取るための変数

											//メッシュフィールド
	pMeshfield = new CMeshfield;

	//初期化
	if (FAILED(pMeshfield->Init()))
	{
		assert(false);
	}

	//メッシュ情報の一時保存
	MeshfieldStructure MeshData;
	ZeroMemory(&MeshData, sizeof(MeshData));

	//読み込み可能モデル数
	int nModelNumber = 0;

	//モデルの読み込む数の保存
	int nModelNumberMax = 0;

	//ファイルを開く
	pFile = fopen(sFilePath, "r");

	if (pFile == nullptr)
	{//開けなかった時用
		assert(false);
	}

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "TEXTURE_FILENAME") == 0)
		{//テクスチャの読み込み

		 //テクスチャパスを読み取るための変数
			char cBffPath[LINE_MAX_READING_LENGTH];
			//文字列の分析
			sscanf(cBff, "%s = %s", &cBffHead, &cBffPath);
			//テクスチャの読み込み
			MeshData.nTextIndex = CTexture::LoadTexture(cBffPath);
		}
		else if (strcmp(&cBffHead[0], "NUM_MODEL") == 0)
		{//モデルの読み込み数

		 //モデル番号の配列ポインタのNULLチェック
			if (pModelIndex != nullptr)
			{
				assert(false);
			}

			//文字列の分析
			sscanf(cBff, "%s = %d", &cBffHead, &nModelNumberMax);
			//モデル番号の配列で管理
			pModelIndex = new int[nModelNumberMax];
		}
		else if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//モデルファイルの読み込み

		 //モデル番号の配列ポインタのNULLチェック
			if (pModelIndex == nullptr)
			{
				assert(false);
			}

			//登録予定の数より多かった場合
			if (nModelNumber >= nModelNumberMax)
			{
				assert(false);
			}

			//モデルパスを読み取るための変数
			char cBffPath[LINE_MAX_READING_LENGTH];
			//文字列の分析
			sscanf(cBff, "%s = %s", &cBffHead, &cBffPath);

			//モデルのロードとインデックスの保存
			pModelIndex[nModelNumber] = ReadXFile(cBffPath);

			//モデル番号配列の保存先を変更
			nModelNumber++;

		}
		else if (strcmp(&cBffHead[0], "FIELDSET") == 0)
		{//地面の設定

		 //文字列の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "POS") == 0)
				{//Posの設定

				 //Posの一時保存
					D3DXVECTOR3 pos;

					//文字列の分析
					sscanf(cBff, "%s = %f %f %f", &cBffHead, &pos.x, &pos.y, &pos.z);

					//メッシュフィールドのPosの設定
					MeshData.pos = pos;
				}
				else if (strcmp(&cBffHead[0], "ROT") == 0)
				{//Rotの設定

				 //Rotの一時保存
					D3DXVECTOR3 rot;

					//文字列の分析
					sscanf(cBff, "%s = %f %f %f", &cBffHead, &rot.x, &rot.y, &rot.z);

					//メッシュフィールドのRotの設定
					MeshData.rot = rot;
				}
				else if (strcmp(&cBffHead[0], "COLOR") == 0)
				{//Colorの設定

				 //Colorの一時保存
					D3DXCOLOR Color;

					//文字列の分析
					sscanf(cBff, "%s = %f %f %f %f", &cBffHead, &Color.r, &Color.g, &Color.b, &Color.a);

					//メッシュフィールドのColorの設定
					MeshData.col = Color;
				}
				else if (strcmp(&cBffHead[0], "SIZE") == 0)
				{//SIZEの設定

				 //SIZEの一時保存
					float fSizeX, fSizeZ;

					//文字列の分析
					sscanf(cBff, "%s = %f %f", &cBffHead, &fSizeX, &fSizeZ);

					//メッシュフィールドのSizeの設定
					MeshData.fRadiusX = fSizeX;
					MeshData.fRadiusZ = fSizeZ;
				}
				else if (strcmp(&cBffHead[0], "DIVISION") == 0)
				{//DIVISIONの設定

				 //DIVISIONの一時保存
					int nMeshX, nMeshZ;

					//文字列の分析
					sscanf(cBff, "%s = %d %d", &cBffHead, &nMeshX, &nMeshZ);

					//メッシュフィールドのDIVISIONの設定
					MeshData.nMeshX = nMeshX;
					MeshData.nMeshZ = nMeshZ;
				}
				else if (strcmp(&cBffHead[0], "END_FIELDSET") == 0)
				{//メッシュ情報の終わりの設定

					//メッシュ情報の設定
					pMeshfield->SetMeshfield(MeshData);

					//データの生成
					pBallast_Manager->CreateBallastListData(pMeshfield->GetMeshfieldNumMax());

					//メッシュフィールドの情報から必要な数値の取得
					pBallast_Manager->MeshfieldSet(pMeshfield);

					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));
			}

		}
		else if (strcmp(&cBffHead[0], "MODELSET") == 0)
		{//モデルの設定

		 //３DOBJ情報の一時保管場所
			Object_Data Data;
			ZeroMemory(&Data, sizeof(Data));

			//文字列の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
			{


				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "TYPE") == 0)
				{//TYPEの設定

					int nPattn = 0;

					//文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &nPattn);

					Data.nPattn = pModelIndex[nPattn];

				}
				else if (strcmp(&cBffHead[0], "POS") == 0)
				{//POSの設定

				 //Pos一時保管場所
					D3DXVECTOR3 pos;

					//文字列の分析
					sscanf(cBff, "%s = %f %f %f", &cBffHead, &pos.x, &pos.y, &pos.z);

					//Posの保存
					Data.pos = pos;
				}
				else if (strcmp(&cBffHead[0], "ROT") == 0)
				{//ROTの設定

				 //Rot一時保管場所
					D3DXVECTOR3 rot;

					//文字列の分析
					sscanf(cBff, "%s = %f %f %f", &cBffHead, &rot.x, &rot.y, &rot.z);

					//Rotの保存
					Data.rot = rot;
				}
				else if (strcmp(&cBffHead[0], "END_MODELSET") == 0)
				{//モデルの設定の終わり

				 //このオブジェクトがメッシュのどこに居るかを調べる用
					int nPosLocation = 0;

					//引く数のPosがどのマスに居るかを返す
					nPosLocation = pMeshfield->CheckPosLocation(Data.pos);

					//エラー検知用
					if (nPosLocation < 0)
					{
						assert(false);
					}

					//瓦礫マネージャーがNULLだったら
					if (pBallast_Manager == nullptr)
					{
						assert(false);
					}

					//瓦礫の設置
					pBallast_Manager->SetBallast(nPosLocation, Data);

					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//スクリプトの終わり
			break;
		}

		//保存中の文字列の初期化
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));
	}

	//モデルインデックス
	if (pModelIndex != nullptr)
	{
		delete[] pModelIndex;
		pModelIndex = nullptr;
	}

	//ファイルを閉じる
	fclose(pFile);

	//メッシュポインタ宣言のリターン
	return pMeshfield;
}


//*****************************************************************************
// モーションファイルの読み込み
//*****************************************************************************
int CRead::ReadMotion(char * sXFilePath)
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];		//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];	//頭の文字を読み取るための変数
	//モデル構造体の保存用変数宣言
	MotionData* pMotiondata = nullptr;
	//各モデルのインデックス
	int* pMotionIndex = nullptr;

	int nMotionMax = 0;//モデルの数

	int nSettingCompletionParts = 0;//設定の完了したパーツ数

	int nSettingCompletionModel = 0;//設定の完了したモデル数

	//セットモーションオブジェクト
	int nMotionNum = 0;

	//ファイルを開く
	pFile = fopen(sXFilePath, "r");

	if (pFile == nullptr)
	{//開けなかった時用
		assert(false);
	}

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "NUM_MODEL") == 0)
		{//モデル数
			if (pMotionIndex != nullptr)
			{
				assert(false);
			}
			sscanf(cBff, "%s = %d", &cBffHead, &nMotionMax);
			pMotionIndex = new int[nMotionMax];
		}
		else if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[LINE_MAX_READING_LENGTH];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//Xファイルの読み込み
			pMotionIndex[nSettingCompletionModel] = ReadXFile(sPath);
			nSettingCompletionModel++;
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//動く物の配置用

		 //モデルセットに必要な情報読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//パーツ数
				 //文字列の分析
					if (pMotiondata != nullptr)
					{
						assert(false);
					}
					int nPartsMax = 0;//パーツの数
					sscanf(cBff, "%s = %d", &cBffHead, &nPartsMax);

					pMotiondata = new MotionData[nPartsMax];
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//移動量
				 //文字列の分析
					//sscanf(cBff, "%s = %f", &cBffHead, &);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//ジャンプ量
				 //文字列の分析
					//sscanf(cBff, "%s = %f", &cBffHead, &);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{//モデルひとつのセット用のデータ読み取り
				 //インデックス保存用
				
					//モデルセットに必要な情報読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//使用するモデルの番号
						 //文字列の分析
							int nNum = 0;
							sscanf(cBff, "%s = %d", &cBffHead, &nNum);
							pMotiondata[nSettingCompletionParts].nModelPattern = pMotionIndex[nNum];
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//親の番号
						 //文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &pMotiondata[nSettingCompletionParts].nParentNum);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//モデルの位置
						 //文字列の分析
							D3DXVECTOR3 pos;
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pos.x, &pos.y, &pos.z);
							pMotiondata[nSettingCompletionParts].pos = pos;
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//モデルの向き
						 //文字列の分析
							D3DXVECTOR3 rot;
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &rot.x, &rot.y, &rot.z);
							pMotiondata[nSettingCompletionParts].rot = rot;
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//モデル１パーツのセット終了
							nSettingCompletionParts++;
							break;
						}

						//保存中の文字列の初期化
						ZeroMemory(&cBff, sizeof(cBff));
						ZeroMemory(&cBffHead, sizeof(cBffHead));

					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//モデルのデータ読み取り終了
					//セットモーションオブジェクト
					nMotionNum = CMotionParts::CreateMotionObj(pMotiondata, nSettingCompletionParts);

					if (pMotiondata != nullptr)
					{
						delete[] pMotiondata;
						pMotiondata = nullptr;
					}

					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));

			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//モーション用のデータ読み取り
			MotionMoveData MotionMoveData;
			MotionMoveData.pMotionKeyData = nullptr;
			MotionMoveData.bLoop = false;
			MotionMoveData.nKeyMax = 0;
			MotionMoveData.nNextMotionNum = 0;

			int nMotionKeyMax = 0;
			int nMotionKey = 0;//使ったキーの数のカウント
			//モデルセットに必要な情報読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//このモーションはループ再生するかどうか
				 //文字列の分析
					int nLoop;
					sscanf(cBff, "%s = %d", &cBffHead, &nLoop);
					MotionMoveData.bLoop = false;
					if (nLoop != 0)
					{
						MotionMoveData.bLoop = true;
					}
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//モーションのキー数
				 //文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &nMotionKeyMax);

					if (MotionMoveData.pMotionKeyData != nullptr)
					{
						assert(false);
					}
					MotionMoveData.nKeyMax = nMotionKeyMax;//キー数の保存
					MotionMoveData.pMotionKeyData = new MotionKeyData[nMotionKeyMax];//キーごとに必要なデータの確保

					for (int nKey = 0; nKey < nMotionKeyMax; nKey++)
					{
						//初期化
						MotionMoveData.pMotionKeyData[nKey].pMotionPartsData = nullptr;
					}
				}
				else if (strcmp(&cBffHead[0], "NEXT_MOTION") == 0)
				{
					int nNext = 0;
					sscanf(cBff, "%s = %d", &cBffHead, &nNext);

					MotionMoveData.nNextMotionNum = nNext;
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{//モデルパーツのモーション設定


					//パーツごとに必要な情報を確保
					if (MotionMoveData.pMotionKeyData[nMotionKey].pMotionPartsData != nullptr)
					{
						assert(false);
					}
					MotionMoveData.pMotionKeyData[nMotionKey].pMotionPartsData = new MotionPartsData[nSettingCompletionParts];

					

					int nModelPasCnt = 0;		//モデルパーツのセット番号

					//モデルセットに必要な情報読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//このモーションは何フレーム数で再生するかチェック
						 //文字列の分析
							int nFrame = 0;
							sscanf(cBff, "%s = %d", &cBffHead, &nFrame);
							if (nFrame == 0)
							{//再生フレーム数が0の場合1にする
								nFrame = 1;
							}
							MotionMoveData.pMotionKeyData[nMotionKey].nFrame = nFrame;
						}
						else if (strcmp(&cBffHead[0], "KEY") == 0)
						{//モーションの１パーツのセット
						 //モデルセットに必要な情報読み取りループ処理
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//文字列の分析
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//モーション中の１パーツの位置
								 //文字列の分析
									D3DXVECTOR3 pos;
									sscanf(cBff, "%s = %f%f%f", &cBffHead,&pos.x,&pos.y,&pos.z);

									MotionMoveData.pMotionKeyData[nMotionKey].pMotionPartsData[nModelPasCnt].pos = pos;
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//モーション中の１パーツの向き
								 //文字列の分析
									D3DXVECTOR3 rot;
									sscanf(cBff, "%s = %f%f%f", &cBffHead, &rot.x, &rot.y, &rot.z);

									MotionMoveData.pMotionKeyData[nMotionKey].pMotionPartsData[nModelPasCnt].rot = rot;
								}
								else if (strcmp(&cBffHead[0], "END_KEY") == 0)
								{//モーション中の１パーツのセット終了
									nModelPasCnt++;//１パーツのセット終了
									break;
								}

								//保存中の文字列の初期化
								ZeroMemory(&cBff, sizeof(cBff));
								ZeroMemory(&cBffHead, sizeof(cBffHead));

							}
						}
						else if (strcmp(&cBffHead[0], "END_KEYSET") == 0)
						{
							nMotionKey++;//設定したキー数のカウント
							break;
						}

						//保存中の文字列の初期化
						ZeroMemory(&cBff, sizeof(cBff));
						ZeroMemory(&cBffHead, sizeof(cBffHead));

					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{//モデルパーツのモーション設定終了
					//モーションの登録
					CMotionParts::SetMotionFileData(MotionMoveData, nMotionNum);

					for (int nCnt = 0; nCnt < nMotionKey; nCnt++)
					{
						if (MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData != nullptr)
						{
							delete[] MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData;
							MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData = nullptr;
						}
					}

					if (MotionMoveData.pMotionKeyData != nullptr)
					{
						delete[] MotionMoveData.pMotionKeyData;
						MotionMoveData.pMotionKeyData = nullptr;
					}

					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));

			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//テキストファイルを読み切った時
			break;
		}

		//保存中の文字列の初期化
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));

	}

	//ファイルを閉じる
	fclose(pFile);

	

	if (pMotionIndex != nullptr)
	{
		delete[] pMotionIndex;
		pMotionIndex = nullptr;
	}

	return nMotionNum;
}

//*****************************************************************************
// Xファイルの読み込み
//*****************************************************************************
int CRead::ReadXFile(char * sXFilePath)
{
	C3DObject::ModelPattern Model;

	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(sXFilePath,
		D3DXMESH_SYSTEMMEM,
		pD3DDevice,
		nullptr,
		&Model.pBuffMatModel,
		nullptr,
		&Model.nNumMatModel,
		&Model.pMeshModel);

	Model.bUse = true;

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)Model.pBuffMatModel->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)Model.nNumMatModel; i++)
	{
		Model.pTexture[i] = nullptr;

		if (pMat[i].pTextureFilename != nullptr)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pD3DDevice,
				pMat[i].pTextureFilename,
				&Model.pTexture[i]);
		}
		else
		{
			Model.pTexture[i] = nullptr;
		}
	}

	//モデルの登録
	return C3DObject::SetModel(&Model);
}