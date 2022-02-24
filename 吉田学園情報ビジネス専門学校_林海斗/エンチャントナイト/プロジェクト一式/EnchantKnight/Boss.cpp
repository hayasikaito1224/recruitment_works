//=======================================
//�G�̃X�|�i�[�̏���
//=======================================
#include "Boss.h"
#include "Enemy_Purple.h"
#include "player.h"
#include "manager.h"
#include "game.h"
#define BOSS_BATTLE_LENGTH (1000.0f)//�{�X�ƃo�g���ɂȂ鋗��
//=======================================
//�R���X�g���N�^
//=======================================
CBoss::CBoss(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_pEnemy = nullptr;
	m_bGimmickLock = true;
}
//=======================================
//�f�X�g���N�^
//=======================================
CBoss::~CBoss()
{
}
//=======================================
//������
//=======================================
HRESULT CBoss::Init()
{

	return S_OK;
}
//=======================================
//�j��
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
//�X�V
//=======================================
void CBoss::Update()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	if (pPlayer != nullptr&&m_pEnemy!=nullptr)
	{
		//�v���C���[�ƃ{�X�̋����𑪂�
		D3DXVECTOR3 vec = pPlayer->GetPos() - m_pEnemy->GetEnemyPos();
		float Length = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
		if (Length <= BOSS_BATTLE_LENGTH)
		{
			m_bBattle = true;
		}
	}
	//�G�̗̑͂�0�ɂȂ��������
	if (m_pEnemy != nullptr)
	{
		if (m_pEnemy->GetUninit() == true)
		{
			m_pEnemy->Uninit();
			m_pEnemy = nullptr;
			//�M�~�b�N�̉���
			m_bGimmickLock = false;

		}
	}

}
//=======================================
//�`��
//=======================================
void CBoss::Draw()
{
}
//=======================================
//�C���X�^���X����
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
