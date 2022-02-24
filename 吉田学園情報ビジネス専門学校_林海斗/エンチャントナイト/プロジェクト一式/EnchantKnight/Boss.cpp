//=======================================
//敵のスポナーの処理
//=======================================
#include "Boss.h"
#include "Enemy_Purple.h"
#include "player.h"
#include "manager.h"
#include "game.h"
#define BOSS_BATTLE_LENGTH (1000.0f)//ボスとバトルになる距離
//=======================================
//コンストラクタ
//=======================================
CBoss::CBoss(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_pEnemy = nullptr;
	m_bGimmickLock = true;
}
//=======================================
//デストラクタ
//=======================================
CBoss::~CBoss()
{
}
//=======================================
//初期化
//=======================================
HRESULT CBoss::Init()
{

	return S_OK;
}
//=======================================
//破棄
//=======================================
void CBoss::Uninit()
{
	if (m_pEnemy != nullptr)
	{
		m_pEnemy->Uninit();
		m_pEnemy = nullptr;
	}
	Release();
}
//=======================================
//更新
//=======================================
void CBoss::Update()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	if (pPlayer != nullptr&&m_pEnemy!=nullptr)
	{
		//プレイヤーとボスの距離を測る
		D3DXVECTOR3 vec = pPlayer->GetPos() - m_pEnemy->GetEnemyPos();
		float Length = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
		if (Length <= BOSS_BATTLE_LENGTH)
		{
			m_bBattle = true;
		}
	}
	//敵の体力が0になったら消す
	if (m_pEnemy != nullptr)
	{
		if (m_pEnemy->GetUninit() == true)
		{
			m_pEnemy->Uninit();
			m_pEnemy = nullptr;
			//ギミックの解除
			m_bGimmickLock = false;

		}
	}

}
//=======================================
//描画
//=======================================
void CBoss::Draw()
{
}
//=======================================
//インスタンス生成
//=======================================
CBoss * CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife)
{
	CBoss *pEnemySpawner = NULL;
	pEnemySpawner = new CBoss(OBJTYPE_SKY);
	pEnemySpawner->m_pos = pos;
	pEnemySpawner->m_rot = rot;
	pEnemySpawner->m_nEnemyType = nType;
	if (pEnemySpawner->m_pEnemy == nullptr)
	{
		switch (nType)
		{
		case Type_Purple:
			pEnemySpawner->m_pEnemy = CEnemy_Purple::Create(pos, rot, nLife);
			break;
		}
	}
	pEnemySpawner->Init();
	return pEnemySpawner;
}
