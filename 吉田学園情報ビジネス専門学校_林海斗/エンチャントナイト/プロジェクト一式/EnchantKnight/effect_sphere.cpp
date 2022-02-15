#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"	
#include "Scene3D.h"
#include "effect_sphere.h"

CEffectSphere::CEffectSphere(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pIdxBuffWall = NULL;
	m_nMax_Vtx_X = 0;
	m_nMax_Vtx_Y = 0;
	m_nVtx_Num_Max = 0;
	m_nIdx_Num_Max = 0;
	m_nPoly_Num_Max = 0;
	m_fHalfWidth = 0.0f;
	m_fHalfDepth = 0.0f;
	m_bUninit = false;
}

CEffectSphere::~CEffectSphere()
{

}

//------------------------------------------------------------------------------------------------
//インスタンス生成処理(位置、大きさ、角度、X軸のブロック数、Z軸のブロック数、貼るテクスチャの種類)
//-------------------------------------------------------------------------------------------------
CEffectSphere *CEffectSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNum_X, int nNum_Z, float fRadius, float fNor, CTexture::Type Tex)
{
	//インスタンス生成
	CEffectSphere *pEffectSphere = new CEffectSphere();

	pEffectSphere->m_pos = pos;
	pEffectSphere->m_fNor = fNor;
	float radion = D3DXToRadian(rot.y);
	pEffectSphere->m_rot = D3DXVECTOR3(rot.x, radion,rot.z) ;
	pEffectSphere->m_fHalfWidth = pEffectSphere->m_size.x / 2.0f;
	pEffectSphere->m_fHalfDepth = pEffectSphere->m_size.z / 2.0f;
	pEffectSphere->m_nMax_Vtx_X = (float)nNum_X;
	pEffectSphere->m_nMax_Vtx_Y = (float)nNum_Z;
	pEffectSphere->m_pTexture = CManager::GetTexture()->GetTexture(Tex);
	pEffectSphere->m_radius = fRadius;
	if (pEffectSphere != NULL)
	{
		pEffectSphere->Init();
		//メッシュの設定
		pEffectSphere->SetMesh(pos, D3DXCOLOR(1.0,1.0,1.0,1.0), fRadius, 0.0f);
	}

	return pEffectSphere;
}

void CEffectSphere::SetMesh(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, float tex_rotX)
{
																	 //頂点バッファの生成
	//-----------------------------
	//頂点情報
	//-----------------------------
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	float f2pi = D3DX_PI * 2;
	for (int nCntY = 0; nCntY < m_nMax_Vtx_Y + 1; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].pos = D3DXVECTOR3(
				cosf(((D3DX_PI * 2) / m_nMax_Vtx_X)*nCntX)*(sinf((f2pi / m_nMax_Vtx_X*nCntY) + (D3DX_PI/2))*fRadius)+ pos.x,
				sinf(((D3DX_PI * 2) / m_nMax_Vtx_X)*nCntY)*m_radius + pos.y,
				sinf(((D3DX_PI * 2) / m_nMax_Vtx_X)*nCntX)*(sinf((f2pi / m_nMax_Vtx_X*nCntY) + (D3DX_PI/2 ))*fRadius)+ pos.z
			);

			pVtx[nVtxNum].nor = D3DXVECTOR3(0.0f, m_fNor, 0.0f);
			pVtx[nVtxNum].col = col;
			pVtx[nVtxNum].tex = D3DXVECTOR2((4.0f / m_nMax_Vtx_X)*nCntX + tex_rotX, (1.0f / m_nMax_Vtx_Y)*nCntY);

		}
	}
	nVtxNum = 0;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	//----------------------------
	//インデックス情報
	//----------------------------
	WORD *pIdx;			//インデックス情報へのポインタ

						//インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);
	//角被り以外の頂点
	for (int nCntY = 0; nCntY < m_nMax_Vtx_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nMax_Vtx_X + 1; nCntX++)
		{
			pIdx[(nCntX * 2) + 0 + ((m_nMax_Vtx_X + 2) * 2) * nCntY] = ((m_nMax_Vtx_X + 1) + nCntX) + ((m_nMax_Vtx_X + 1) * nCntY);
			pIdx[(nCntX * 2) + 1 + ((m_nMax_Vtx_X + 2) * 2) * nCntY] = (0 + nCntX) + ((m_nMax_Vtx_X + 1) * nCntY);
		}
	}
	//角被りの頂点
	for (int nCntY = 0; nCntY < m_nMax_Vtx_Y - 1; nCntY++)
	{
		pIdx[(((m_nMax_Vtx_X + 1) * 2 + 0) * (nCntY + 1)) + (2 * nCntY)] = m_nMax_Vtx_X + ((m_nMax_Vtx_X + 1) * nCntY);
		pIdx[(((m_nMax_Vtx_X + 1) * 2 + 1) * (nCntY + 1)) + (1 * nCntY)] = (m_nMax_Vtx_X * 2 + 2) + ((m_nMax_Vtx_X + 1) * nCntY);
	}
	//インデックスバッファをアンロックする
	m_pIdxBuffWall->Unlock();

}
HRESULT CEffectSphere::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	m_nVtx_Num_Max = (m_nMax_Vtx_X + 1)*(m_nMax_Vtx_Y + 1);
	m_nIdx_Num_Max = (m_nMax_Vtx_X + 1)*(m_nMax_Vtx_Y + 1) + (m_nMax_Vtx_Y - 1)*(m_nMax_Vtx_X + 3);
	m_nPoly_Num_Max = 2 * m_nMax_Vtx_X * m_nMax_Vtx_Y + (m_nMax_Vtx_Y * 4) - 4;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVtx_Num_Max,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD)* m_nIdx_Num_Max,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffWall,
		NULL);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffectSphere::Uninit(void)
{

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuffWall != NULL)
	{
		m_pIdxBuffWall->Release();
		m_pIdxBuffWall = NULL;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffectSphere::Update(void)
{
	//if (m_col.a <= 0.0f)
	//{
	//	m_bUninit = true;
	//}
	if (m_bUninit == true)
	{
		Uninit();
	}
}
//=======================================================================
//描画処理
//=======================================================================
void CEffectSphere::Draw()
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
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffWall);

	pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

	pDevice->SetTexture(0, m_pTexture);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//カリングをする
														  //ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,					//プリミティブの種類
		0,									//インデックスバッファの開始地点から最初のインデックスまでのオフセット
		m_nVtx_Num_Max,	//描画で使用される最小の頂点番号
		(m_nMax_Vtx_X + 1)*(m_nMax_Vtx_Y + 1),		//頂点の数
		0,	//インデックス配列の読み取りを開始する位置
		m_nPoly_Num_Max);	//三角形ポリゴンの数
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリングをする

}

