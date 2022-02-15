//---------------------------------------------------
//背景の処理
//---------------------------------------------------
#include "layer_structure.h"
#include "model.h"


//--------------------------
//コンストラクト
//----------------------------

CLayer_Structure::CLayer_Structure()
{
}

//--------------------------
//デストラクト
//----------------------------
CLayer_Structure::~CLayer_Structure()
{

}

//-----------------------------------------------------------
//階層構造の読み込み
//-----------------------------------------------------------
void CLayer_Structure::SetLayer_Structure(const char *sFileName, CModel **apModel)
{
	int nIndex = 0;						//パーツの番号
	int nParent = 0;				//親子関係
	char string[6][255];
	char pPartsFileName[20][255];	//モデルパーツのファイルネーム
	memset(&pPartsFileName, NULL, sizeof(pPartsFileName));
	D3DXVECTOR3 PartsPos, PartsRot;	//各パーツの位置と向き
	int nCntModel = 0;
	int nNumModel = 0;
	int nTypeMode = 0;
	//textファイル読み込み
	FILE *pfile = fopen(sFileName, "r");
	//ヌルチェック
	if (pfile != NULL)
	{
		while (1)
		{
			//一単語を読み込む
			fscanf(pfile, "%s", &string[0]);
			//何に使うモデル化判断
			if (strcmp(string[0], "MODEL_TYPE") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
				fscanf(pfile, "%d", &nTypeMode);
			}

			//モデル数
			if (strcmp(string[0], "NUM_MODEL") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
				fscanf(pfile, "%d", &nNumModel);
				m_MaxParts = nNumModel;
			}

			//モデルのファイルのなまえ
			if (strcmp(string[0], "MODEL_FILENAME") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
 				fscanf(pfile, "%s", &pPartsFileName[nCntModel]);
				nCntModel++;
			}

			//--------------------------------------
			//モデルの階層構造の設定
			//--------------------------------------
			while (strcmp(string[0], "CHARACTERSET") == 0)
			{
				fscanf(pfile, "%s", &string[1]);

				//パーツの設定
				while (strcmp(string[1], "PARTSSET") == 0)
				{
					fscanf(pfile, "%s", &string[2]);
					//モデルパーツの番号
					if (strcmp(string[2], "INDEX") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%d", &nIndex);
					}
					//親の番号
					if (strcmp(string[2], "PARENT") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%d", &nParent);
					}
					//位置
					if (strcmp(string[2], "POS") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
					}
					//向き
					if (strcmp(string[2], "ROT") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
					}
					//モデルの生成
					if (strcmp(string[2], "END_PARTSSET") == 0)
					{
						apModel[nIndex] = CModel::Create(PartsPos, PartsRot, nIndex, nTypeMode);

						//親子関係を付ける
						if (nParent != -1)
						{
							apModel[nIndex]->SetParent(apModel[nParent]);
							apModel[nIndex]->SetNumParent(nParent);
						}
						else
						{
							apModel[nIndex]->SetParent(NULL);
							apModel[nIndex]->SetNumParent(-1);
						}
						break;
					}

				}
				//終了
				if (strcmp(string[1], "END_CHARACTERSET") == 0)
				{
					break;
				}

			}

			//スクリプト読込の終了
			if (strcmp(string[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}
}

void CLayer_Structure::SetLayer_Structure(const char * sFileName, vector<CModel*>& pModel)
{
	int nIndex = 0;						//パーツの番号
	int nParent = 0;				//親子関係
	char string[6][255];
	char pPartsFileName[20][255];	//モデルパーツのファイルネーム
	memset(&pPartsFileName, NULL, sizeof(pPartsFileName));
	D3DXVECTOR3 PartsPos, PartsRot;	//各パーツの位置と向き
	int nCntModel = 0;
	int nNumModel = 0;
	int nTypeMode = 0;
	//textファイル読み込み
	FILE *pfile = fopen(sFileName, "r");
	//ヌルチェック
	if (pfile != NULL)
	{
		while (1)
		{
			//一単語を読み込む
			fscanf(pfile, "%s", &string[0]);
			//何に使うモデル化判断
			if (strcmp(string[0], "MODEL_TYPE") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
				fscanf(pfile, "%d", &nTypeMode);
			}

			//モデル数
			if (strcmp(string[0], "NUM_MODEL") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
				fscanf(pfile, "%d", &nNumModel);
				m_MaxParts = nNumModel;
				//動的確保
				for (int nCnt  = 0; nCnt  < m_MaxParts; nCnt ++)
				{
					CModel *Securement = {};
					pModel.push_back(Securement);

				}
			}

			//モデルのファイルのなまえ
			if (strcmp(string[0], "MODEL_FILENAME") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
				fscanf(pfile, "%s", &pPartsFileName[nCntModel]);
				nCntModel++;
			}

			//--------------------------------------
			//モデルの階層構造の設定
			//--------------------------------------
			while (strcmp(string[0], "CHARACTERSET") == 0)
			{
				fscanf(pfile, "%s", &string[1]);

				//パーツの設定
				while (strcmp(string[1], "PARTSSET") == 0)
				{
					fscanf(pfile, "%s", &string[2]);
					//モデルパーツの番号
					if (strcmp(string[2], "INDEX") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%d", &nIndex);
					}
					//親の番号
					if (strcmp(string[2], "PARENT") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%d", &nParent);
					}
					//位置
					if (strcmp(string[2], "POS") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
					}
					//向き
					if (strcmp(string[2], "ROT") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
					}
					//モデルの生成
					if (strcmp(string[2], "END_PARTSSET") == 0)
					{
						pModel[nIndex] = CModel::Create(PartsPos, PartsRot, nIndex, nTypeMode);

						//親子関係を付ける
						if (nParent != -1)
						{
							pModel[nIndex]->SetParent(pModel[nParent]);
							pModel[nIndex]->SetNumParent(nParent);
						}
						else
						{
							pModel[nIndex]->SetParent(NULL);
							pModel[nIndex]->SetNumParent(-1);
						}
						break;
					}

				}
				//終了
				if (strcmp(string[1], "END_CHARACTERSET") == 0)
				{
					break;
				}

			}

			//スクリプト読込の終了
			if (strcmp(string[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}
