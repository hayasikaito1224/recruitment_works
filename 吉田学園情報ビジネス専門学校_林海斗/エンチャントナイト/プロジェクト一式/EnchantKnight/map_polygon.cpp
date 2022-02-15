//----------------------------------
//ポリゴンの処理
//-----------------------------------
#include "texture.h"
#include "map_polygon.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "stage.h"
#include "game.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMap_Polygon::CMap_Polygon(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_bDraw = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMap_Polygon::~CMap_Polygon()
{

}
void CMap_Polygon::BindTexture(CTexture::Type type)
{
	m_pTexture = CManager::GetTexture()->GetTexture(type);
}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CMap_Polygon *CMap_Polygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, CTexture::Type texture)
{
	//インスタンス生成
	CMap_Polygon *pMap_Polygon = new CMap_Polygon(OBJTYPE_MAP_UI);

	if (pMap_Polygon != NULL)
	{
		pMap_Polygon->BindTexture(texture);
		pMap_Polygon->m_Scale = scale;
		pMap_Polygon->Init();
		pMap_Polygon->m_pos.y = 50.0f;
		pMap_Polygon->SetPos(pos.x,pos.z);
	}

	return pMap_Polygon;

}

HRESULT CMap_Polygon::Init(void)
{
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
	pVtx[0].pos = D3DXVECTOR3(-m_Scale.x, 0.0f, m_Scale.z);
	pVtx[1].pos = D3DXVECTOR3(m_Scale.x, 0.0f, m_Scale.z);
	pVtx[2].pos = D3DXVECTOR3(-m_Scale.x, 0.0f, -m_Scale.z);
	pVtx[3].pos = D3DXVECTOR3(m_Scale.x, 0.0f, -m_Scale.z);

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
void CMap_Polygon::Uninit(void)
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

void CMap_Polygon::Update(void)
{
	
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CMap_Polygon::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	if (m_bDraw == true)
	{
									//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);
		m_mtxWorld._11 = m_Scale.x;
		m_mtxWorld._33 = m_Scale.z;
		////αテスト
		//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		//ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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
		//ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}


}

//------------------------------------------------------------
//頂点座標の設定
//------------------------------------------------------------

void CMap_Polygon::SetPos(const float PosX, const float PosZ)
{
	CScene::SetScale(D3DXVECTOR3(PosX, m_pos.y, PosZ));
	m_pos.x = PosX;
	m_pos.z = PosZ;

}

void CMap_Polygon::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();

}
