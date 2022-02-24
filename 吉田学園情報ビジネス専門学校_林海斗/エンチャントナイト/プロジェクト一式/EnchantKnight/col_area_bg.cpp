//----------------------------------
//当たり判定が発生するエリアの表示
//-----------------------------------
#include "col_area_bg.h"
#include "Renderer.h"
#include "manager.h"
#include "Scene3D.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CCol_Area_Bg::CCol_Area_Bg(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bDraw = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_size = { 0.0f,0.0f,0.0f };
	m_fValue = 0.0f;
	m_col = { 1.0,1.0,1.0,1.0 };
}

//=============================================================================
// デストラクタ
//=============================================================================
CCol_Area_Bg::~CCol_Area_Bg()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CCol_Area_Bg::Init()
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
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
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

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CCol_Area_Bg::Uninit(void)
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
void CCol_Area_Bg::Update(void)
{
	m_fValue += 1.0f;
	if (m_fValue >= m_fValueMax)
	{
		m_fValue = m_fValueMax;
		m_bUninit = true;
	}

	if (m_bUninit)
	{
		Uninit();
	}

}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CCol_Area_Bg::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

								//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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

}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CCol_Area_Bg * CCol_Area_Bg::Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fSizeZ,
	const float fAlpha, const int nValueMax, const CTexture::Type texture)
{
	//インスタンス生成
	CCol_Area_Bg *pCol_Area = new CCol_Area_Bg(OBJTYPE_EFFECT);
	if (pCol_Area != NULL)
	{
		pCol_Area->m_pos = pos;
		pCol_Area->m_size.x = fSizeX;
		pCol_Area->m_size.z = fSizeZ;
		pCol_Area->m_col.a = fAlpha;
		pCol_Area->m_Tex = texture;
		pCol_Area->m_rot.y = fRotY;
		pCol_Area->m_fValueMax = nValueMax;
		pCol_Area->Init();
	}
	return pCol_Area;
}
