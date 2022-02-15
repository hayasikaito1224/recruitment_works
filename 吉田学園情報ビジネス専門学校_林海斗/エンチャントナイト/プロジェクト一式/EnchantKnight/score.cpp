#include "main.h"
#include "manager.h"
#include "keyboard.h"
#include "scene.h"
#include "Scene2D.h"
#include "score.h"
#include "texture.h"

int CScore::m_nScore;
CScore::CScore(OBJTYPE nPriority) : CScene(nPriority)
{

}

CScore::~CScore()
{

}

CScore *CScore::Create(D3DXVECTOR3 pos, float fsize)
{
	CScore *pTime = NULL;
	pTime = new CScore;
	pTime->Init(pos, fsize);
	return pTime;
}

HRESULT CScore::Init(D3DXVECTOR3 pos, float fsize)
{
	for (int nCntTime = 0; nCntTime < MAX_SCORE; nCntTime++)
	{
		m_apScene2D[nCntTime] = CScene2D::Create(D3DXVECTOR3(pos.x + nCntTime * 80, pos.y, pos.z), fsize, CTexture::TIME);
		m_apScene2D[nCntTime]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	m_nScore = 300;
	return S_OK;
}

HRESULT CScore::Init(void)
{
	return 0;
}

void CScore::Uninit()
{
	
}

void CScore::Update()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		AddScore(10);
	}
	SetScore();
}

void CScore::Draw()
{
	for (int nCntTime = 0; nCntTime < MAX_SCORE; nCntTime++)
	{
		m_apScene2D[nCntTime]->Draw();
	}
}

void CScore::SetScore()
{
	int aScore[MAX_SCORE];
	aScore[0] = m_nScore % 1000 / 100;
	aScore[1] = m_nScore % 100 / 10;
	aScore[2] = m_nScore % 10 / 1;

	for (int nCntTime = 0; nCntTime < MAX_SCORE; nCntTime++)
	{
		m_apScene2D[nCntTime]->SetTex(0, aScore[nCntTime], 0.1f);
	}
}

