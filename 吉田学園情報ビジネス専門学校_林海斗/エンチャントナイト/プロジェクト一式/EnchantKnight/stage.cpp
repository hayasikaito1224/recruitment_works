//========================================
// ステージ
// Author:林
//========================================
#include "stage.h"
#include "model.h"
#include "wall.h"
#include "field.h"
#include "texture.h"
#include "model_spawner.h"
#include "Enemy_poyo.h"
//------------------------------------
// コンストラクタ
//------------------------------------
CStage::CStage()
{
}

//------------------------------------
// デストラクタ
//------------------------------------
CStage::~CStage()
{

}

void CStage::Load(const char *cFileName)
{
	char sString[6][255];	// 読み込み用の変数
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nBlockX = 0;
	int nBlockY = 0;
	int nType = 0;
	// ファイル読み込み
	FILE *pFile = fopen(cFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			//壁の読み込み
			while (strcmp(sString[0], "WALLSET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);	
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	
				}
				if (strcmp(sString[1], "BLOCK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d %d", &nBlockX, &nBlockY);	
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f", &size.x, &size.y);
				}
				if (strcmp(sString[1], "END_WALLSET") == 0)
				{
					CWall::Create({ pos.x,pos.y+200.0f,pos.z}, size, { D3DXToRadian(rot.x),D3DXToRadian(rot.y) ,D3DXToRadian(rot.z) }, nBlockX, nBlockY * 4, CTexture::FIELD);
					break;
				}
			}

			// メッシュフィールドの読み込み
			while (strcmp(sString[0], "FIELDSET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	
				}
				if (strcmp(sString[1], "BLOCK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d %d", &nBlockX, &nBlockY);	
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f", &size.x, &size.z);
				}
				if (strcmp(sString[1], "END_FIELDSET") == 0)
				{
					CField::Create(pos, size, { 0.0f,0.0f,0.0f }, nBlockX, nBlockY, CTexture::FIELD);
					break;
				}
			}
			// モデルの読み込み
			while (strcmp(sString[0], "MODELSET") == 0)
			{
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "TYPE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d", &nType);	
				}
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f", &size.x, &size.z);	
				}
				if (strcmp(sString[1], "END_MODELSET") == 0)
				{
					//モデルの生成（位置、回転、モデルの種類、配置するオブジェクトのタイプ（プレイヤーか配置物かを判断している））
					CModel::Create(pos, rot, nType, CModel::TYPE_OBJECT);
					break;
				}
			}
			// 敵の読み込み
			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

	// ファイルを閉じる
	fclose(pFile);


}

