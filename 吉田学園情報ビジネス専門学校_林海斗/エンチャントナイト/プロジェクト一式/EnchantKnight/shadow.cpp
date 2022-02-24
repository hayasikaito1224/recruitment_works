//----------------------------------
//ポリゴンの処理
//-----------------------------------
#include "texture.h"
#include "shadow.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "stage.h"
#include "game.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CShadow::CShadow(OBJTYPE nPriority) :CScene3D(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CShadow::~CShadow()
{

}
void CShadow::BindTexture(CTexture::Type type)
{
	m_pTexture = CManager::GetTexture()->GetTexture(type);
}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CShadow *CShadow::Create(D3DXVECTOR3 pos, float fSize, CTexture::Type texture)
{
	//インスタンス生成
	CShadow *pShadow = new CShadow(OBJTYPE_SHADOW);

	if (pShadow != NULL)
	{
		pShadow->BindTexture(texture);
		pShadow->m_size.x = fSize;
		pShadow->m_size.z = fSize;

		pShadow->Init();
		pShadow->m_pos.y = 0.5f;
		pShadow->SetPos(pos.x, pos.z, D3DXVECTOR3(fSize, 0.0f, fSize));
	}

	return pShadow;

}

HRESULT CShadow::Init(void)
{
	m_fGravitySpeed = SHADOWGRAVITY;
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	// 頂点情報を設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.z);

	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================

void CShadow::Update(void)
{
	m_LastPos = m_pos;
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
																	 //減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//m_mtxWorld._11 = m_size.x;
	//m_mtxWorld._33 = m_size.z;
	////αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをでバスのでーたすとりーむに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

	pDevice->SetTexture(0, m_pTexture);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//------------------------------------------------------------
//頂点座標の設定
//------------------------------------------------------------

void CShadow::SetPos(const float fposX, const float fposZ, D3DXVECTOR3 scale)
{
	CScene::SetScale(scale);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(fposX - scale.x, m_pos.y, fposZ + scale.z);
	pVtx[1].pos = D3DXVECTOR3(fposX + scale.x, m_pos.y, fposZ + scale.z);
	pVtx[2].pos = D3DXVECTOR3(fposX - scale.x, m_pos.y, fposZ - scale.z);
	pVtx[3].pos = D3DXVECTOR3(fposX + scale.x, m_pos.y, fposZ - scale.z);

	m_pVtxBuff->Unlock();

}

void CShadow::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();

}
