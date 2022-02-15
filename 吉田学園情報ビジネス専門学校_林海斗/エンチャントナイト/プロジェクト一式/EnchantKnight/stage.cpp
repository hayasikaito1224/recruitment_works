//========================================
// �X�e�[�W
// Author:��
//========================================
#include "stage.h"
#include "model.h"
#include "wall.h"
#include "field.h"
#include "texture.h"
#include "model_spawner.h"
#include "Enemy_poyo.h"
//------------------------------------
// �R���X�g���N�^
//------------------------------------
CStage::CStage()
{
}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CStage::~CStage()
{

}

void CStage::Load(const char *cFileName)
{
	char sString[6][255];	// �ǂݍ��ݗp�̕ϐ�
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nBlockX = 0;
	int nBlockY = 0;
	int nType = 0;
	// �t�@�C���ǂݍ���
	FILE *pFile = fopen(cFileName, "r");
	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// END_SCRIPT���Ă΂��܂Ń��[�v����
		while (1)
		{
			// �P�P���ǂݍ���
			fscanf(pFile, "%s", &sString[0]);
			//�ǂ̓ǂݍ���
			while (strcmp(sString[0], "WALLSET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);	
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	
				}
				if (strcmp(sString[1], "BLOCK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%d %d", &nBlockX, &nBlockY);	
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f", &size.x, &size.y);
				}
				if (strcmp(sString[1], "END_WALLSET") == 0)
				{
					CWall::Create({ pos.x,pos.y+200.0f,pos.z}, size, { D3DXToRadian(rot.x),D3DXToRadian(rot.y) ,D3DXToRadian(rot.z) }, nBlockX, nBlockY * 4, CTexture::FIELD);
					break;
				}
			}

			// ���b�V���t�B�[���h�̓ǂݍ���
			while (strcmp(sString[0], "FIELDSET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	
				}
				if (strcmp(sString[1], "BLOCK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%d %d", &nBlockX, &nBlockY);	
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f", &size.x, &size.z);
				}
				if (strcmp(sString[1], "END_FIELDSET") == 0)
				{
					CField::Create(pos, size, { 0.0f,0.0f,0.0f }, nBlockX, nBlockY, CTexture::FIELD);
					break;
				}
			}
			// ���f���̓ǂݍ���
			while (strcmp(sString[0], "MODELSET") == 0)
			{
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "TYPE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%d", &nType);	
				}
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f", &size.x, &size.z);	
				}
				if (strcmp(sString[1], "END_MODELSET") == 0)
				{
					//���f���̐����i�ʒu�A��]�A���f���̎�ށA�z�u����I�u�W�F�N�g�̃^�C�v�i�v���C���[���z�u�����𔻒f���Ă���j�j
					CModel::Create(pos, rot, nType, CModel::TYPE_OBJECT);
					break;
				}
			}
			// �G�̓ǂݍ���
			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

	// �t�@�C�������
	fclose(pFile);


}

