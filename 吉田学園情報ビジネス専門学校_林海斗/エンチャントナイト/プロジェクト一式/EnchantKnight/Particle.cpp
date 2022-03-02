//=========================================
//パーティクルの処理
//=========================================
#include "main.h"
#include "Particle.h"
#include "effect.h"
#include "texture.h"
#include <random>
#include "camera.h"
#include "Renderer.h"
//静的メンバ変数宣言
CParticle::ParticleState CParticle::m_State[MaxParticle] = {};
//=========================================
//コンストラクタ
//=========================================
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
	//パーティクルデータを読み込む
	Load("data/TEXT/ParticleData000.txt");

}
//=========================================
//デストラクタ
//=========================================
CParticle::~CParticle()
{

}

//-----------------------------------
//円状に出るパーティクル
//-----------------------------------
void CParticle::PlayCircleParticle(bool bFixedRadius, float fRadius, D3DXCOLOR col)
{
	std::random_device random;	// 非決定的な乱数生成器
	std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
	std::uniform_real_distribution<> randMagicCircle(0.0f, fRadius);
	std::uniform_real_distribution<> randSpeed(0.3, m_fSpeed);
	//タイムを加算
	m_nTime++;
	//時間になったら
	if (m_nTime > m_nMaxTime)
	{
		m_nTime = 0.0f;

		D3DXVECTOR3 pos;

		//ランダムな角度を代入
		float fAng = randAng(mt);

		//一定の半径にエフェクトを出現させないなら
		if (bFixedRadius == false)
		{
			//半径の値をランダムにした位置にする
			pos = D3DXVECTOR3(
				cosf(fAng)*randMagicCircle(mt) + m_pos.x,
				m_pos.y,
				sinf(fAng)*randMagicCircle(mt) + m_pos.z
			);

		}
		else
		{
			//半径の値が一定の位置にする
			pos = D3DXVECTOR3(
				cosf(fAng)*fRadius + m_pos.x,
				m_pos.y,
				sinf(fAng)*fRadius + m_pos.z
			);

		}
		//上方向にランダムな移動量にする
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
			randSpeed(mt),
			0.0f);

		//生成時に数個同じエフェクトを重ねる
		for (int nStack = 0; nStack < m_nEffectStack; nStack++)
		{
			//エフェクトの生成
			CEffect::Create(pos, move, m_size, col, m_bGravity, m_fGravity, m_fDefSpeedColorA, CTexture::Effect);
		}
	}

}
//-----------------------------------
//球状にランダムに広がる
//-----------------------------------
void CParticle::RandomCircleParticle(D3DXVECTOR3 pos, D3DXCOLOR col, bool bStop)
{
	//停止判定がオフなら
	if (bStop == false)
	{
		//RANDOMPARTICLEのステータスを取得
		ParticleState state = m_State[RANDOMPARTICLE];

		std::random_device random;	// 非決定的な乱数生成器
		std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
		std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
		std::uniform_real_distribution<> randSpeed(state.fSpeed / 2, state.fSpeed);
		std::uniform_real_distribution<> randSizeX(state.size.x / 2, state.size.x);
		std::uniform_real_distribution<> randSizeY(state.size.y / 2, state.size.y);

		//ランダムな角度を求める
		float fAng = (float)randAng(mt);
		float fAng2 = (float)randAng(mt);

		//極座標系の式を利用した球状にランダムにひろがる移動量を求める
		D3DXVECTOR3 move = D3DXVECTOR3(
			sinf(fAng)*sinf(fAng2)*randSpeed(mt),
			cosf(fAng)*randSpeed(mt),
			sinf(fAng)*cosf(fAng2)*randSpeed(mt));

		//生成時に数個同じエフェクトを重ねる
		for (int nStack = 0; nStack < state.nEffectStack; nStack++)
		{
			//エフェクトの生成
			CEffect::Create(pos, move, { (float)randSizeX(mt),(float)randSizeY(mt),0.0f }, col, state.bGravity, state.fGravity, state.fDefSpeedColorA,true, CTexture::GlitterEffect, fAng);
		}
	}
}
//-----------------------------------
//３Dのランダムエフェクト
//-----------------------------------
void CParticle::PlayRandomCircle(D3DXVECTOR3 pos, int nType, int nTexType)
{
	//指定したタイプのステータス情報を取得
	ParticleState state = m_State[nType];

	std::random_device random;	// 非決定的な乱数生成器
	std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
	std::uniform_real_distribution<> randSpeed(state.fSpeed / 2, state.fSpeed);

	//カメラの向きの取得
	float fCamera = CRenderer::GetCamera()->GetRot().x;

	//エフェクトを一気に生成する
	for (int nCnt = 0; nCnt < state.nMaxParticle; nCnt++)
	{	
		//ランダムな角度を求める
		float fAng = (float)randAng(mt);
		float fAng2 = (float)randAng(mt);

		//極座標系の式を利用した球状にランダムにひろがる移動量を求める
		D3DXVECTOR3 move = D3DXVECTOR3(
			sinf(fAng)*sinf(fAng2)*state.fSpeed,
			cosf(fAng)*state.fSpeed,
			sinf(fAng)*cosf(fAng2)*state.fSpeed);

		//現在の位置とその先の位置からのベクトルを求める
		D3DXVECTOR3 vec = m_pos - (m_pos + move);

		//敵の向き
		float fAngle = atan2(vec.x, vec.y)*-1;

		//生成時に数個同じエフェクトを重ねる
		for (int nStack = 0; nStack < state.nEffectStack; nStack++)
		{
			//エフェクトの生成
			CEffect::Create(pos, move, state.size, state.col, state.bGravity, state.fGravity,
				state.fDefSpeedColorA,true, (CTexture::Type)nTexType, fAngle- fCamera);
		}
	}
}


//-----------------------------------
//テキストの読み込み
//-----------------------------------
void CParticle::Load(const char * cFileName)
{
	char sString[6][255];	// 読み込み用の変数
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
			if (strcmp(sString[0], "TYPE") == 0)
			{
				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
				fscanf(pFile, "%d", &nType);


			}
			// メッシュフィールドの読み込み
			while (strcmp(sString[0], "PARTICLESET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
				ParticleState& state = m_State[nType];

				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &state.size.x, &state.size.y, &state.size.z);
				}
				if (strcmp(sString[1], "MOVE_Y") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f", &state.move.y);	
				}
				if (strcmp(sString[1], "COL") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f %f", &state.col.r, &state.col.g, &state.col.b, &state.col.a);
				}
				if (strcmp(sString[1], "CANGRAVITY") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d", &state.bGravity);	
				}
				if (state.bGravity == true)
				{
					if (strcmp(sString[1], "GRAVITY") == 0)
					{
						fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
						fscanf(pFile, "%f", &state.fGravity);
					}
				}
				if (strcmp(sString[1], "STACK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d", &state.nEffectStack);
				}
				if (strcmp(sString[1], "MAXPARTICLE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d", &state.nMaxParticle);
				}
				if (strcmp(sString[1], "SPEED") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f", &state.fSpeed);
				}
				if (strcmp(sString[1], "COLDEFSPEED") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
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

	// ファイルを閉じる
	fclose(pFile);

}
