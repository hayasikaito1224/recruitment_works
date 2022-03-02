//----------------------------------
//当たり判定が発生するエリアの表示
//-----------------------------------
#include "col_area.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"

#include "keyboard.h"
#include "Scene3D.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CCol_Area::CCol_Area(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bDraw = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_size = { 0.0f,0.0f,0.0f };
	m_fTime = 0.0f;
	m_col = { 0.7f,0.0,0.0,1.0 };
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
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.01f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.01f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.01f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.01f);
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
	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CCol_Area::Update(void)
{
	MoveVtx();
	
	if (m_bUninit)
	{
		Uninit();
	}

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
	////αテスト
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CCol_Area * CCol_Area::Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fAlpha, const float MaxGauge,
	const int nValueMax, const CTexture::Type texture)
{
	//インスタンス生成
	CCol_Area *pCol_Area = new CCol_Area(OBJTYPE_EFFECT);
	if (pCol_Area != NULL)
	{
		pCol_Area->m_pos = pos;
		pCol_Area->m_size.x = fSizeX;
		pCol_Area->m_col.a = fAlpha;
		pCol_Area->m_Tex = texture;
		pCol_Area->m_rot.y = fRotY;
		pCol_Area->m_fMaxGauge = MaxGauge;
		pCol_Area->m_fTimeMax = nValueMax;
		pCol_Area->Init();
	}
	return pCol_Area;
}
//---------------------------------------------------------------
//頂点の移動
//---------------------------------------------------------------
void CCol_Area::MoveVtx()
{
	m_fTime += 1.0f;
	//頂点を移動する量を計算
	float fAdd = (m_fMaxGauge * 1.0f) / m_fTimeMax;
	m_size.z += fAdd;
	//決められた時間になったら
	if (m_fTime >= m_fTimeMax)
	{
		//時間を最大の時間に合わせる
		m_fTime = m_fTimeMax;
		//Z軸に向かう頂点の最大数を合わせる
		m_size.z = m_fMaxGauge;
		//消去判定をオンにする
		m_bUninit = true;
	}
	//頂点情報の更新
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.z);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	m_pVtxBuff->Unlock();

}
