//-------------------------------------------------------------------
//フェードの処理
//-------------------------------------------------------------------

#include "fade.h"
#include "bg.h"
#include "texture.h"
#include "manager.h"
//静的メンバ変数宣言
CFade::FADE					CFade::m_fade = CFade::FADE_IN;
CManager::MODE				CFade::m_modeNext = CManager::MODE_TITLE;
float						CFade::m_colorAlpha = 1.0f;
CBg							*CFade::m_pBg = NULL;

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CFade::CFade(OBJTYPE nPriority) :CScene(nPriority)
{

}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CFade::~CFade()
{
}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CFade *CFade::Create(CTexture::Type type, CManager::MODE modeNext)
{
	//インスタンス生成
	CFade *pFade = new CFade;
	if (pFade != NULL)
	{
		pFade->m_modeNext = modeNext;
		pFade->m_Type = type;
		pFade->Init();
	}
	return pFade;
}

//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CFade::Init(void)
{
	m_pBg = CBg::Create(CTexture::Text, OBJTYPE_FADE,false);

	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------

void CFade::Uninit(void)
{
	Release();
}

//フェードの設定処理
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_colorAlpha = 0.0f;
}

//フェードの取得処理
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}
//フェードの更新処理

void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_colorAlpha));

		//フェード画面を消す
		if (m_fade == FADE_IN)
		{
			m_colorAlpha -= 0.05f;
			if (m_colorAlpha <= 0.0f)
			{
				m_colorAlpha = 0.0f;
				m_fade = FADE_NONE;
			}

		}
		//フェード画面を出す
		else if (m_fade == FADE_OUT)
		{
			m_colorAlpha += 0.01f;

			if (m_colorAlpha >= 1.0f)
			{
				m_colorAlpha = 1.0f;
				m_fade = FADE_IN;
				//モードの設定
				CManager::SetMode(m_modeNext);
			}

		}

	}

}
void CFade::Draw(void)
{
}
