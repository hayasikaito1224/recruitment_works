//----------------------------------
//ナンバーの処理
//-----------------------------------
#include "deleteItem.h"
#include "main.h"
#include "scene2D.h"
#include "Renderer.h"
#include "manager.h"
#include "texture.h"
#include "game.h"
#include "Score.h"
#include "player.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CDeleteItem::CDeleteItem()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=============================================================================
// デストラクタ
//=============================================================================
CDeleteItem::~CDeleteItem()
{

}

//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CDeleteItem *CDeleteItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 m_Scale, CTexture::Type type, float fAngle)
{
	//インスタンス生成
	CDeleteItem *pDeleteItem = new CDeleteItem;
	if (pDeleteItem != NULL)
	{
		pDeleteItem->m_pos = pos;
		pDeleteItem->m_Scale = m_Scale;
		pDeleteItem->m_Type = type;
		pDeleteItem->m_fAngle = D3DXToRadian(fAngle);
		pDeleteItem->Init();

	}

	return pDeleteItem;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CDeleteItem::Init(void)
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

																	 // 頂点情報を設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	m_bUninit = false;
	D3DXVECTOR3 posOrigin[4];
	D3DXVECTOR3 Rotatepos[4];

	//スケールサイズ込みの４頂点
	posOrigin[0].x = -m_Scale.x;
	posOrigin[1].x = m_Scale.x;
	posOrigin[2].x = -m_Scale.x;
	posOrigin[3].x = m_Scale.x;

	posOrigin[0].y = -m_Scale.y;
	posOrigin[1].y = -m_Scale.y;
	posOrigin[2].y = m_Scale.y;
	posOrigin[3].y = m_Scale.y;

	//回転の計算
	Rotatepos[0].x = (posOrigin[0].x * cosf(m_fAngle)) - (posOrigin[0].y * sinf(m_fAngle));
	Rotatepos[1].x = (posOrigin[1].x * cosf(m_fAngle)) - (posOrigin[1].y * sinf(m_fAngle));
	Rotatepos[2].x = (posOrigin[2].x * cosf(m_fAngle)) - (posOrigin[2].y * sinf(m_fAngle));
	Rotatepos[3].x = (posOrigin[3].x * cosf(m_fAngle)) - (posOrigin[3].y * sinf(m_fAngle));

	Rotatepos[0].y = (posOrigin[0].x * sinf(m_fAngle)) + (posOrigin[0].y * cosf(m_fAngle));
	Rotatepos[1].y = (posOrigin[1].x * sinf(m_fAngle)) + (posOrigin[1].y * cosf(m_fAngle));
	Rotatepos[2].y = (posOrigin[2].x * sinf(m_fAngle)) + (posOrigin[2].y * cosf(m_fAngle));
	Rotatepos[3].y = (posOrigin[3].x * sinf(m_fAngle)) + (posOrigin[3].y * cosf(m_fAngle));


	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + Rotatepos[0].x, m_pos.y + Rotatepos[0].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + Rotatepos[1].x, m_pos.y + Rotatepos[1].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + Rotatepos[2].x, m_pos.y + Rotatepos[2].y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + Rotatepos[3].x, m_pos.y + Rotatepos[3].y, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pVtxBuff->Unlock();

	m_fSpeed = 7.0f;
	m_fSucRadius = 300.0f;

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CDeleteItem::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CDeleteItem::Update(void)
{
	if (m_bUninit == false)
	{
		m_nTimer++;

		ItemSuction();
		SetPos(m_pos);
	}

	//画面範囲外に行ったら消去判定をtrueにする
	if (m_pos.x <= 0.0f*1.1f ||
		m_pos.x >= SCREEN_WIDTH *1.1f ||
		m_pos.y <= 0.0f *1.1f ||
		m_pos.y >= SCREEN_HEIGHT*1.1f)
	{
		m_bUninit = true;
	}
	if (m_bUninit == true)
	{
		Uninit();
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CDeleteItem::Draw(void)
{
	if (m_bUninit == false)
	{

		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

																		 // 頂点フォーマットの設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
		pDevice->SetTexture(0, CManager::GetTexture()->GetTexture(m_Type));

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}
//------------------------------------------------------------
//頂点座標の設定
//------------------------------------------------------------

void CDeleteItem::SetPos(D3DXVECTOR3 pos)
{

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(pos.x - m_Scale.x, pos.y - m_Scale.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_Scale.x, pos.y - m_Scale.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_Scale.x, pos.y + m_Scale.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_Scale.x, pos.y + m_Scale.y, 0.0f);

	m_pVtxBuff->Unlock();

}

//=============================================================================
//引き寄せ処理
//=============================================================================
void CDeleteItem::ItemSuction(void)
{
	D3DXVECTOR3	PlayerPos = CGame::GetPlayer()->GetPos();				//プレイヤーの位置
	D3DXVECTOR3 ItemVec = PlayerPos - m_pos;			//プレイヤーとアイテムのベクトル
	float fLength = sqrtf((ItemVec.y*ItemVec.y));		//
	float fLength_X = sqrtf((ItemVec.x*ItemVec.x));		//

	m_fAngle = atan2(ItemVec.x, ItemVec.y);//敵の向き
													//範囲内に入ったらプレイヤーに追従する
	if (fLength <= 15.0f&&fLength_X <=15.0f)
	{
		//スコア加算
		m_bUninit = true;
		//CGame::GetScore()->AddScore(DELETEITEM_POINT);

	}

	if (fLength <= m_fSucRadius)
	{
		m_bDoSuction = true;
		m_pos.x += sinf(m_fAngle)*(m_fSpeed);
		m_pos.y += cosf(m_fAngle)*m_fSpeed;
	}

	else
	{
		m_bDoSuction = false;
	}
}
//=============================================================================
//カラー変更
//=============================================================================
void CDeleteItem::SetCol(D3DXCOLOR col)
{

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	m_pVtxBuff->Unlock();

}

//=============================================================================
//位置情報の更新
//=============================================================================

void CDeleteItem::SetTex(D3DXVECTOR2 tex, D3DXVECTOR2 tex1, D3DXVECTOR2 tex2, D3DXVECTOR2 tex3)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = tex;
	pVtx[1].tex = tex1;
	pVtx[2].tex = tex2;
	pVtx[3].tex = tex3;

	m_pVtxBuff->Unlock();

}
