//=======================================
//�G�̃X�|�i�[�̏���
//=======================================
#include "enemyspawner.h"
#include "Enemy_poyo.h"
#include "Enemy_Frame.h"
#include "Enemy_Crystal.h"
//=======================================
//�R���X�g���N�^
//=======================================
CEnemy_Spawner::CEnemy_Spawner(OBJTYPE nPriority):CScene(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_pEnemy.clear();
	m_nNumEnemy = 0;
	m_nEnemyType = 0;
	m_fAreaX = 0.0f;
	m_fAreaZ = 0.0f;
	m_bGimmickLock = true;
}
//=======================================
//�f�X�g���N�^
//=======================================
CEnemy_Spawner::~CEnemy_Spawner()
{
}
//=======================================
//������
//=======================================
HRESULT CEnemy_Spawner::Init()
{
	for (int nCnt = 0; nCnt < m_nNumEnemy; nCnt++)
	{
		//���I�m��
		CEnemy *pTemp = {};
		m_pEnemy.push_back(pTemp);
		std::random_device random;	// �񌈒�I�ȗ���������
		std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
		std::uniform_real_distribution<> randAreaX(-m_fAreaX, m_fAreaX);
		std::uniform_real_distribution<> randAreaZ(-m_fAreaZ, m_fAreaZ);
		float fAreaX = (float)randAreaX(mt);
		float fAreaZ = (float)randAreaZ(mt);
		switch (m_nEnemyType)
		{
		case TYPE_FRAME:
			m_pEnemy[nCnt] = CEnemy_Frame::Create({ m_pos.x + fAreaX,m_pos.y,m_pos.z + fAreaZ }, { 0.0f,0.0f,0.0f });

			break;
		case TYPE_CRYSTAL:
			m_pEnemy[nCnt] = CEnemy_Crystal::Create({ m_pos.x + fAreaX,m_pos.y,m_pos.z + fAreaZ }, { 0.0f,0.0f,0.0f });

			break;
		case TYPE_POYO:
			m_pEnemy[nCnt] = CEnemy_Poyo::Create({ m_pos.x + fAreaX,m_pos.y,m_pos.z + fAreaZ }, { 0.0f,0.0f,0.0f });

			break;

		}

	}
	return S_OK;
}
//=======================================
//�j��
//=======================================
void CEnemy_Spawner::Uninit()
{
	int nSize = (int)m_pEnemy.size();
	for (int nCnt = 0; nCnt < nSize; nCnt++)
	{
		if (m_pEnemy[nCnt] != nullptr)
		{
			if (m_pEnemy[nCnt]->GetUninit() == false)
			{
				m_pEnemy[nCnt]->Uninit();
			}
			m_pEnemy[nCnt] = nullptr;
		}
	}
	Release();
}
//=======================================
//�X�V
//=======================================
void CEnemy_Spawner::Update()
{
	int nSize = (int)m_pEnemy.size();
	auto Begin = m_pEnemy.begin();
	for (int nCnt = 0; nCnt < nSize; nCnt++)
	{
		if (m_pEnemy[nCnt] != nullptr)
		{
			if (m_pEnemy[nCnt]->GetUninit() == true)
			{
				m_pEnemy[nCnt] ->Uninit();

				m_pEnemy[nCnt] = nullptr;
				m_nNumEnemy--;
			}
		}

	}
	//�G���A���̓G���S����������
	if (m_nNumEnemy <= 0)
	{
		//�M�~�b�N�̉���
		m_bGimmickLock = false;
	}
}
//=======================================
//�`��
//=======================================
void CEnemy_Spawner::Draw()
{
}
//=======================================
//�C���X�^���X����
//=======================================
CEnemy_Spawner * CEnemy_Spawner::Create(D3DXVECTOR3 pos, float fAreaX, float fAreaZ, int nType, int nMaxEnemy)
{
	CEnemy_Spawner *pEnemySpawner = NULL;
	pEnemySpawner = new CEnemy_Spawner(OBJTYPE_SKY);
	pEnemySpawner->m_pos = pos;
	pEnemySpawner->m_fAreaX = fAreaX;
	pEnemySpawner->m_fAreaZ = fAreaZ;
	pEnemySpawner->m_nNumEnemy = nMaxEnemy;
	pEnemySpawner->m_nEnemyType = nType;
	pEnemySpawner->Init();
	return pEnemySpawner;
}
