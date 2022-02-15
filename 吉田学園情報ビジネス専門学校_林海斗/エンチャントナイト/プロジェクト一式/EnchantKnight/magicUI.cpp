//=============================================
//プレイヤーが操作するコマンド関係の処理
//=============================================
#include "magicUI.h"
#include "Polygon.h"
#define NEXTMOVE (50.0)
#define MOVESPEED (8.0)
#define COLSPEED (0.1)
#define MAX_COL (0.7)
#define ALPHA_MIN (0.4)
#define ALPHA_MAX (1.0)
#define BUTTON_POSX (160.0)
#define BUTTON_SIZE (30.0)

//=============================================================================
// コンストラクタ
//=============================================================================
CMagicUI::CMagicUI(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bSelect = false;
	m_bDraw = true;
	m_pCommandBG = nullptr;
	m_pMagic = nullptr;
	m_bAlphaFadeIn = false;
	m_pButton = nullptr;
}
//=============================================================================
// デストラクタ
//=============================================================================
CMagicUI::~CMagicUI()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMagicUI::Init()
{
	if (m_pCommandBG == nullptr)
	{
		m_pCommandBG = CPolygon::Create(m_pos, m_size, CTexture::COMMAND_BG);
	}
	if (m_pMagic == nullptr)
	{
		m_pMagic = CPolygon::Create(m_pos, m_size, (CTexture::Type)m_nTexType);
	}
	if (m_pButton == nullptr)
	{
		CTexture::Type ButtonTexType = CTexture::None;
		switch (m_nTexType)
		{
		case CTexture::BLAZE:
			ButtonTexType = CTexture::B_BUTTON;
			break;
		case CTexture::ICECLE:
			ButtonTexType = CTexture::X_BUTTON;
			break;
		case CTexture::HEEL:
			ButtonTexType = CTexture::A_BUTTON;
			break;
		}
		m_pButton = CPolygon::Create({ m_pos.x - (float)BUTTON_POSX,m_pos.y,m_pos.z }, { BUTTON_SIZE ,BUTTON_SIZE ,0.0f}, ButtonTexType);

	}
	//ポリゴンの生成
	m_col = { 1.0,1.0,1.0,1.0 };
	m_InitPos = m_pos;//初期の位置を保存
	return S_OK;
}
//=============================================================================
// 破棄
//=============================================================================
void CMagicUI::Uninit()
{
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->Uninit();
		m_pCommandBG = nullptr;
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->Uninit();
		m_pMagic = nullptr;
	}
	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		m_pButton = nullptr;
	}
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CMagicUI::Update()
{
	//選択していたら
	if (m_bSelect)
	{
		//移動の加算
		m_fMove += MOVESPEED;
		//移動量が次の移動先に行ったら
		if (m_fMove >= NEXTMOVE)
		{
			m_fMove = NEXTMOVE;
		}
		//位置に移動量を加算
		m_pos.x = m_InitPos.x + m_fMove;
	}
	else
	{
		m_pos.x -= MOVESPEED;
		m_fMove = 0.0;
		if (m_InitPos.x >= m_pos.x)
		{
			m_pos.x = m_InitPos.x;
		}

	}
	if (m_bPush)
	{
		m_col.r -= (float)COLSPEED;
		m_col.g -= (float)COLSPEED;
		m_col.b -= (float)COLSPEED;
		if (m_col.r <= MAX_COL)
		{
			m_col.r = MAX_COL;
			m_col.g = MAX_COL;
			m_col.b = MAX_COL;
		}
	}
	else
	{
		m_col.r += (float)COLSPEED;
		m_col.g += (float)COLSPEED;
		m_col.b += (float)COLSPEED;
		if (m_col.r >= 1.0)
		{
			m_col.r = 1.0;
			m_col.g = 1.0;
			m_col.b = 1.0;

		}

	}
	//α値のフェード処理
	AlphaFade();
	//位置情報の更新
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->SetPos(m_pos);
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->SetPos(m_pos);
	}
	if (m_pButton != nullptr)
	{
		m_pButton->SetPos({ m_pos.x - (float)BUTTON_POSX,m_pos.y,m_pos.z });
	}

	//色の更新
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->SetCol(m_col);
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->SetCol(m_col);
	}
	if (m_pButton != nullptr)
	{
		m_pButton->SetCol(m_col);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CMagicUI::Draw()
{
	//描画の切り替え
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->SetDraw(m_bDraw);
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->SetDraw(m_bDraw);
	}
	if (m_pButton != nullptr)
	{
		m_pButton->SetDraw(m_bDraw);
	}
}
//=============================================================================
//インスタンス生成処理
//=============================================================================
CMagicUI * CMagicUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexType)
{
	//インスタンス生成
	CMagicUI *pCommand = new CMagicUI(CScene::OBJTYPE_UI);
	if (pCommand)
	{
		pCommand->m_nTexType = nTexType;
		pCommand->m_pos = pos;
		pCommand->m_size = size;
		pCommand->Init();
	}
	return pCommand;
}
//=============================================================================
//α値のフェード処理
//=============================================================================

void CMagicUI::AlphaFade()
{
	if (m_bAlphaFadeIn)
	{
		m_col.a -= 0.05f;
		if (m_col.a < ALPHA_MIN)
		{
			m_col.a = ALPHA_MIN;
		}
	}
	else
	{
		m_col.a += 0.05f;
		if (m_col.a >= ALPHA_MAX)
		{
			m_col.a = ALPHA_MAX;
		}
	}
}
