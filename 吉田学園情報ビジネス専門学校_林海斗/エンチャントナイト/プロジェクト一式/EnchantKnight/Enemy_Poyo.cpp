#include "Enemy_poyo.h"
#include "layer_structure.h"
#include "motion.h"
#define POYO_LIFE (10)//“G‚Ì–½
//#define MAX_ENEMY_DEFENSE (2)//“G‚Ì–hŒä—Í
#define POYO_POWER (10)//UŒ‚—Í

CEnemy_Poyo::CEnemy_Poyo(OBJTYPE nPriority) : CEnemy(nPriority)
{
	m_nPower = POYO_POWER;
	m_nLife = POYO_LIFE;
}

CEnemy_Poyo::~CEnemy_Poyo()
{

}

CEnemy_Poyo *CEnemy_Poyo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy_Poyo *pPoyo = NULL;
	pPoyo = new CEnemy_Poyo(OBJTYPE_ENEMY);
	pPoyo->m_pos = pos;
	pPoyo->m_rot = rot;
	pPoyo->Init();
	return pPoyo;
}

HRESULT CEnemy_Poyo::Init()
{
	CEnemy::Init();
	//ƒtƒ@ƒCƒ‹“Ç‚Ýž‚Ý
	CLayer_Structure *pLayer = NULL;
	pLayer = new CLayer_Structure;
	pLayer->SetLayer_Structure("data/TEXT/EnemyParts000.txt", m_pPartsModel);
	CModel *pmodel = new CModel;
	//UŒ‚”»’è—p‚Ì‹ó‚ÌƒIƒuƒWƒFƒNƒg¶¬
	m_pAttackModel.push_back(pmodel);
	m_pAttackModel[0] = CModel::Create({ 0.0f,0.0f,55.0f }, { 0.0f,0.0f,0.0f }, 0, CModel::TYPE_OBJECT);
	//‚µ‚Á‚Û‚ÉeŽqŠÖŒW‚ð‚Â‚¯‚é
	m_pAttackModel[0]->SetParent(m_pPartsModel[3]);
	m_pAttackModel[0]->SetNumParent(3);

	if (m_pMotion == NULL)
	{
		m_pMotion = new CMotion;
		m_MotionType = 0;
		m_pMotion->MotionLoad("data/TEXT/poyo_motion.txt");

	}

	return S_OK;
}

void CEnemy_Poyo::Uninit()
{
	CEnemy::Uninit();
	// ƒ‚ƒfƒ‹”jŠü

	Release();
}

void CEnemy_Poyo::Update()
{
	CEnemy::Update();


}

void CEnemy_Poyo::Draw()
{
	CEnemy::Draw();
}
