//----------------------------------------------
//ゲージの処理
//Author::林海斗
//--------------------------------------------
#include "main.h" 
#include "manager.h"
#include "renderer.h"
#include "circlegauge.h"
#include "fade.h"
CCircleGauge::CCircleGauge(OBJTYPE nPriority) : CScene2D(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	m_col = { 1.0,1.0,1.0,1.0 };
	m_fMaxGauge = 395.0f;
}

CCircleGauge::~CCircleGauge()
{

}

CCircleGauge *CCircleGauge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 scale, const D3DXCOLOR col,
	const float MaxGauge, const int nValue)
{
	CCircleGauge *pGauge = NULL;
	pGauge = new CCircleGauge;
	pGauge->m_pos = pos;
	pGauge->m_Scale = scale;
	pGauge->m_fMaxGauge = MaxGauge;
	pGauge->m_col = col;
	pGauge->m_Initcol = col;
	pGauge->Init();
	pGauge->BindTexture(CTexture::SKILLGAUGE);
	pGauge->m_fValueMax = nValue;
	pGauge->m_fValue = nValue;

	return pGauge;
}

HRESULT CCircleGauge::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

void CCircleGauge::Uninit()
{
	CScene2D::Uninit();
}

void CCircleGauge::Update()
{

	SetScalePos(m_pos, m_Scale);

	//ゲージが最大になったら最大値にする
	if (m_Scale.y >= m_fMaxGauge)
	{
		m_Scale.y = m_fMaxGauge;
		m_col = { m_Initcol.r,m_Initcol.g,m_Initcol.b,1.0f };

	}
	//ゲージをマイナスまでいかせない
	else if (m_Scale.y <= 0.0f)
	{
		m_Scale.y = 0.0f;
	}
	else
	{
		m_col = m_Initcol;
	}
	CScene2D::SetCol(m_col);
}

void CCircleGauge::Draw()
{
	CScene2D::Draw();
}
//--------------------------------------------------------------------
//ゲージのリセット
//--------------------------------------------------------------------
void CCircleGauge::ResetGauge()
{
	m_Scale.y = 0.0f;
	m_fValue = 0;

}

//--------------------------------------------------------------------
//ゲージの減少用
//--------------------------------------------------------------------
void CCircleGauge::SetGauge(float Set)
{
	m_fValue -= Set;
	float fAdd = (m_fMaxGauge * Set) / m_fValueMax;
	m_Scale.y -= fAdd;

	if (m_fValue >= m_fValueMax)
	{
		m_fValue = m_fValueMax;
		m_Scale.y = m_fMaxGauge;
	}
}

//--------------------------------------------------------------------
//スケールのpos＋側だけ増やすため
//--------------------------------------------------------------------
void CCircleGauge::SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{

	CScene::SetPos(pos);
	CScene::SetScale(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(pos.x - scale.x, pos.y - scale.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + scale.x, pos.y - scale.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - scale.x, pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + scale.x, pos.y, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0f-(1.0f / m_fMaxGauge)* scale.y);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f-(1.0f / m_fMaxGauge)* scale.y);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();

}