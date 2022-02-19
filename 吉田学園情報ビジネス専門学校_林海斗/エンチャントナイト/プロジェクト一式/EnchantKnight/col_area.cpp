//----------------------------------
//ポリゴンの処理
//-----------------------------------
#include "col_area.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
#include "texture.h"
#include "keyboard.h"
#include "Scene3D.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CCol_Area::CCol_Area()
{
	m_bDraw = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCol_Area::~CCol_Area()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CCol_Area::Init()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

																	 // 頂点情報を設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//テクスチャ画像の設定
	m_pTexture = CManager::GetTexture()->GetTexture(m_Tex);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
	m_vtx[0].vtx = pVtx[0].pos;
	m_vtx[1].vtx = pVtx[1].pos;
	m_vtx[2].vtx = pVtx[2].pos;
	m_vtx[3].vtx = pVtx[3].pos;

	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CCol_Area::Uninit(void)
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
void CCol_Area::Update(void)
{
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CCol_Area::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

								//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//m_mtxWorld._11 = m_size.x;
	//m_mtxWorld._33 = m_size.z;
	//αテスト
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
	if (m_bDraw)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);

	}
	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CCol_Area *CCol_Area::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, CTexture::Type texture, D3DXCOLOR col)
{
	//インスタンス生成
	CCol_Area *pCol_Area = new CCol_Area();
	if (pCol_Area != NULL)
	{
		pCol_Area->m_pos = pos;
		pCol_Area->m_size = scale;
		pCol_Area->m_col = col;
		pCol_Area->m_Tex = texture;
		pCol_Area->m_rot = rot;
		pCol_Area->Init();
	}
	return pCol_Area;
}
