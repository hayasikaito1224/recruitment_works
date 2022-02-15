//---------------------------------------------------
//背景の処理
//---------------------------------------------------
#include"bg.h"
#include "manager.h"
#include "Renderer.h"
#include "scene2D.h"
#include "keyboard.h"



//--------------------------
//コンストラクト
//----------------------------

CBg::CBg(OBJTYPE nPriority):CScene(nPriority)
{
	m_bScroll = false;
	m_nPriority = nPriority;
}

//--------------------------
//デストラクト
//----------------------------
CBg::~CBg()
{

}
void CBg::SetCol(D3DXCOLOR col)
{
	m_pScene2D->SetCol(col);
}
//--------------------------
//初期化
//----------------------------
HRESULT CBg::Init()
{
	m_pScene2D = new CScene2D(m_nPriority);

	m_pScene2D->Init(m_Type);
	m_pScene2D->SetPos(D3DXVECTOR3(0.0f,0.0f, 0.0f),
							D3DXVECTOR3(SCREEN_WIDTH,0.0f, 0.0f),
							D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f),
							D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	m_pScene2D->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
	return S_OK;

}

//--------------------------
//終了
//----------------------------
void CBg::Uninit(void)
{

	m_pScene2D->Uninit();
	m_pScene2D = NULL;
	Release();
}

//--------------------------
//更新
//----------------------------
void CBg::Update(void)
{

	if (m_bScroll == true)
	{
		m_fSpeed -= 0.01f;

		m_pScene2D->SetTex(0.0f, m_fSpeed);

	}

}

//--------------------------
//描画
//----------------------------
void CBg::Draw(void)
{

}

//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CBg *CBg::Create(CTexture::Type type, OBJTYPE nPriority, bool bScrol)
{
	//インスタンス生成
	CBg *pBg = new CBg(nPriority);
	if (pBg != NULL)
	{
		pBg->m_Type = type;
		pBg->m_bScroll = bScrol;
		pBg->Init();
	}
	return pBg;
}
