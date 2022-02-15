#include "main.h"
#include "Particle.h"
#include "effect.h"
#include "texture.h"
#include <random>
#include "camera.h"
#include "Renderer.h"
//�ÓI�����o�ϐ��錾
CParticle::ParticleState CParticle::m_State[MaxParticle] = {};
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	m_nMaxTime = 5;
	m_size = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
	m_col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	m_nEffectStack = 1;
	m_nMaxParticle = 10;
	m_fGravity = 0.1f;
	m_fSpeed = 0.5;
	m_fCircleAng = 360.0f;
	m_nMax_Y = 9;
	m_nMax_X = 9;
	m_fCircleMove = D3DX_PI / 2;
	m_fDefSpeedColorA = 0.01f;
	m_fCircleMoveSpeed = 0.05f;
	m_nTime = 0;
	m_bMagicCircleSwitch = false;
	for (int nCnt = 0; nCnt < MaxParticle; nCnt++)
	{
		m_State[nCnt] = {};
	}
	Load("data/TEXT/ParticleData000.txt");

}

CParticle::~CParticle()
{

}


//-----------------------------------
//�~��ɏo��p�[�e�B�N��
//-----------------------------------

void CParticle::PlayCircleParticle(bool bFixedRadius, float fRadius, D3DXCOLOR col)
{
	std::random_device random;	// �񌈒�I�ȗ���������
	std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
	std::uniform_real_distribution<> randMagicCircle(0.0f, fRadius);
	std::uniform_real_distribution<> randSpeed(0.3, m_fSpeed);

	m_nTime++;
	if (m_nTime > m_nMaxTime)
	{
		m_nTime = 0.0f;

		D3DXVECTOR3 pos;
		float fAng = randAng(mt);
		if (bFixedRadius == false)
		{
			pos = D3DXVECTOR3(
				cosf(fAng)*randMagicCircle(mt) + m_pos.x,
				m_pos.y,
				sinf(fAng)*randMagicCircle(mt) + m_pos.z
			);

		}
		else
		{
			pos = D3DXVECTOR3(
				cosf(fAng)*fRadius + m_pos.x,
				m_pos.y,
				sinf(fAng)*fRadius + m_pos.z
			);

		}
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
			randSpeed(mt),
			0.0f);

		for (int nStack = 0; nStack < m_nEffectStack; nStack++)
		{
			CEffect::Create(pos, move, m_size, col, m_bGravity, m_fGravity, m_fDefSpeedColorA, CTexture::Effect);
		}
	}

}
//-----------------------------------
//����Ƀ����_���ɍL����
//-----------------------------------
void CParticle::RandomCircleParticle(D3DXVECTOR3 pos, D3DXCOLOR col, bool bStop)
{
	if (bStop == false)
	{
		ParticleState state = m_State[RANDOMPARTICLE];
		std::random_device random;	// �񌈒�I�ȗ���������
		std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
		std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
		std::uniform_real_distribution<> randSpeed(state.fSpeed / 2, state.fSpeed);
		std::uniform_real_distribution<> randSizeX(state.size.x / 2, state.size.x);
		std::uniform_real_distribution<> randSizeY(state.size.y / 2, state.size.y);

		float fAng = (float)randAng(mt);
		float fAng2 = (float)randAng(mt);
		D3DXVECTOR3 move = D3DXVECTOR3(
			sinf(fAng)*sinf(fAng2)*randSpeed(mt),
			cosf(fAng)*randSpeed(mt),
			sinf(fAng)*cosf(fAng2)*randSpeed(mt));
		for (int nStack = 0; nStack < state.nEffectStack; nStack++)
		{
			CEffect::Create(pos, move, { (float)randSizeX(mt),(float)randSizeY(mt),0.0f }, col, state.bGravity, state.fGravity, state.fDefSpeedColorA,true, CTexture::GlitterEffect, fAng);
		}
	}
}
//-----------------------------------
//�RD�̃����_���G�t�F�N�g
//-----------------------------------

void CParticle::PlayRandomCircle(D3DXVECTOR3 pos, int nType, int nTexType)
{
	ParticleState state = m_State[nType];

	std::random_device random;	// �񌈒�I�ȗ���������
	std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
	std::uniform_real_distribution<> randSpeed(state.fSpeed / 2, state.fSpeed);
	float fCamera = CRenderer::GetCamera()->GetRot().x;
	for (int nCnt = 0; nCnt < state.nMaxParticle; nCnt++)
	{
		float fAng = (float)randAng(mt);

		float fAng2 = (float)randAng(mt);
		D3DXVECTOR3 move = D3DXVECTOR3(
			sinf(fAng)*sinf(fAng2)*state.fSpeed,
			cosf(fAng)*state.fSpeed,
			sinf(fAng)*cosf(fAng2)*state.fSpeed);

		D3DXVECTOR3 vec = m_pos - (m_pos + move);
		float fAngle = atan2(vec.x, vec.y)*-1;//�G�̌���

		for (int nStack = 0; nStack < state.nEffectStack; nStack++)
		{
			CEffect::Create(pos, move, state.size, state.col, state.bGravity, state.fGravity,
				state.fDefSpeedColorA,true, (CTexture::Type)nTexType, fAngle- fCamera);
		}
	}
}


//-----------------------------------
//�e�L�X�g�̓ǂݍ���
//-----------------------------------
void CParticle::Load(const char * cFileName)
{
	char sString[6][255];	// �ǂݍ��ݗp�̕ϐ�
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
			if (strcmp(sString[0], "TYPE") == 0)
			{
				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
				fscanf(pFile, "%d", &nType);


			}
			// ���b�V���t�B�[���h�̓ǂݍ���
			while (strcmp(sString[0], "PARTICLESET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
				ParticleState& state = m_State[nType];

				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f", &state.size.x, &state.size.y, &state.size.z);
				}
				if (strcmp(sString[1], "MOVE_Y") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f", &state.move.y);	
				}
				if (strcmp(sString[1], "COL") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f %f %f %f", &state.col.r, &state.col.g, &state.col.b, &state.col.a);
				}
				if (strcmp(sString[1], "CANGRAVITY") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%d", &state.bGravity);	
				}
				if (state.bGravity == true)
				{
					if (strcmp(sString[1], "GRAVITY") == 0)
					{
						fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
						fscanf(pFile, "%f", &state.fGravity);
					}
				}
				if (strcmp(sString[1], "STACK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%d", &state.nEffectStack);
				}
				if (strcmp(sString[1], "MAXPARTICLE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%d", &state.nMaxParticle);
				}
				if (strcmp(sString[1], "SPEED") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f", &state.fSpeed);
				}
				if (strcmp(sString[1], "COLDEFSPEED") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
					fscanf(pFile, "%f", &state.fDefSpeedColorA);
				}
				if (strcmp(sString[1], "END_PARTICLESET") == 0)
				{
					break;
				}
			}
			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

	// �t�@�C�������
	fclose(pFile);

}
