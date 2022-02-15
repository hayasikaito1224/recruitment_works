#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"	
#include "Scene3D.h"
#include "field.h"

CField::CField(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_FieldData.m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	m_pIdxBuffWall = NULL;
	m_FieldData.m_nMax_Vtx_X = 0;
	m_FieldData.m_nMax_Vtx_Z = 0;
	m_nVtx_Num_Max = 0;
	m_nIdx_Num_Max = 0;
	m_nPoly_Num_Max = 0;
	m_fHalfWidth = 0.0f;
	m_fHalfDepth = 0.0f;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

}

CField::~CField()
{

}

//------------------------------------------------------------------------------------------------
//インスタンス生成処理(位置、大きさ、角度、X軸のブロック数、Z軸のブロック数、貼るテクスチャの種類)
//-------------------------------------------------------------------------------------------------
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nNum_X, int nNum_Z, CTexture::Type Tex)
{
	//インスタンス生成
	CField *pField = new CField();

	pField->m_pos = pos;
	pField->m_FieldData.m_InitSize = size;
	pField->m_FieldData.m_size = D3DXVECTOR3(size.x*nNum_X, size.y, size.z*nNum_Z);
	float radion = D3DXToRadian(rot.y);
	pField->m_rot.y = radion;
	pField->m_fHalfWidth = pField->m_FieldData.m_size.x / 2.0f;
	pField->m_fHalfDepth = pField->m_FieldData.m_size.z / 2.0f;
	pField->m_FieldData.m_nMax_Vtx_X = nNum_X;
	pField->m_FieldData.m_nMax_Vtx_Z = nNum_Z;
	pField->m_pTexture = CManager::GetTexture()->GetTexture(Tex);

	if (pField != NULL)
	{
		pField->Init();
	}

	return pField;
}

void CField::SetSize(D3DXVECTOR3 size)
{
	m_FieldData.m_size = size;
}

void CField::SetMoveTex(float fSpeedX, float fSpeedY)
{
	m_MoveTexX = fSpeedX;
	m_MoveTexZ = fSpeedY;
	//-----------------------------
	//頂点情報
	//-----------------------------
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntZ = 0; nCntZ < m_FieldData.m_nMax_Vtx_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_FieldData.m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * nCntX) + m_MoveTexX, 0.0f + (1.0f * nCntZ) + m_MoveTexX);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
void CField::SetCol(float fAlpha)
{
	//-----------------------------
	//頂点情報
	//-----------------------------
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntZ = 0; nCntZ < m_FieldData.m_nMax_Vtx_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_FieldData.m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].col = D3DXCOLOR(1.0,1.0,1.0, fAlpha);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

HRESULT CField::Init(void)
{
	m_nVtx_Num_Max = (m_FieldData.m_nMax_Vtx_X + 1)*(m_FieldData.m_nMax_Vtx_Z + 1);
	m_nIdx_Num_Max = (m_FieldData.m_nMax_Vtx_X + 1)*(m_FieldData.m_nMax_Vtx_Z + 1) + (m_FieldData.m_nMax_Vtx_Z - 1)*(m_FieldData.m_nMax_Vtx_X + 3);
	m_nPoly_Num_Max = 2 * m_FieldData.m_nMax_Vtx_X * m_FieldData.m_nMax_Vtx_Z + (m_FieldData.m_nMax_Vtx_Z * 4) - 4;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
																	 //テクスチャ画像の設定

																	 //角度から円周率に変換する
	//頂点バッファの生成
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
	//-----------------------------
	//頂点情報
	//-----------------------------
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntZ = 0; nCntZ < m_FieldData.m_nMax_Vtx_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_FieldData.m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{

			pVtx[nVtxNum].pos = D3DXVECTOR3(
				-m_fHalfWidth + (((m_fHalfWidth * 2) / m_FieldData.m_nMax_Vtx_X)*nCntX),
				0.0f,
				m_fHalfDepth  - (((m_fHalfDepth * 2) / m_FieldData.m_nMax_Vtx_Z)*nCntZ));

			pVtx[nVtxNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nVtxNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * nCntX)+ m_MoveTexX, 0.0f + (1.0f * nCntZ)+ m_MoveTexX);
			if (m_vtxMax.x <= pVtx[nVtxNum].pos.x)
			{
				m_vtxMax.x = pVtx[nVtxNum].pos.x;
			}
			//ｘの最小値の比較
			else if (m_vtxMin.x >= pVtx[nVtxNum].pos.x)
			{
				m_vtxMin.x = pVtx[nVtxNum].pos.x;
			}
			//yの最大値の比較
			if (m_vtxMax.y <= pVtx[nVtxNum].pos.y)
			{
				m_vtxMax.y = pVtx[nVtxNum].pos.y;
			}
			//yの最小値の比較
			else if (m_vtxMin.y >= pVtx[nVtxNum].pos.y)
			{
				m_vtxMin.y = pVtx[nVtxNum].pos.y;
			}
			//zの最大値の比較
			if (m_vtxMax.z <= pVtx[nVtxNum].pos.z)
			{
				m_vtxMax.z = pVtx[nVtxNum].pos.z;
			}
			//zの最小値の比較
			else if (m_vtxMin.z >= pVtx[nVtxNum].pos.z)
			{
				m_vtxMin.z = pVtx[nVtxNum].pos.z;
			}

		}
	}
	nVtxNum = 0;
	//左奥（上面）
	m_vtx[0].vtx = D3DXVECTOR3(
		m_vtxMin.x,
		m_vtxMin.y,
		m_vtxMax.z);
	//右奥（上面）
	m_vtx[1].vtx = D3DXVECTOR3(
		m_vtxMax.x,
		m_vtxMin.y,
		m_vtxMax.z);
	//左前（上面）
	m_vtx[2].vtx = D3DXVECTOR3(
		m_vtxMin.x,
		m_vtxMin.y,
		m_vtxMin.z);
	//右前（上面）
	m_vtx[3].vtx = D3DXVECTOR3(
		m_vtxMax.x,
		m_vtxMin.y,
		m_vtxMin.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	//----------------------------
	//インデックス情報
	//----------------------------
	WORD *pIdx;			//インデックス情報へのポインタ

						//インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);
	//角被り以外の頂点
	for (int nCntZ = 0; nCntZ < m_FieldData.m_nMax_Vtx_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_FieldData.m_nMax_Vtx_X + 1; nCntX++)
		{
			pIdx[(nCntX * 2) + 0 + ((m_FieldData.m_nMax_Vtx_X + 2) * 2) * nCntZ] = ((m_FieldData.m_nMax_Vtx_X + 1) + nCntX) + ((m_FieldData.m_nMax_Vtx_X + 1) * nCntZ);
			pIdx[(nCntX * 2) + 1 + ((m_FieldData.m_nMax_Vtx_X + 2) * 2) * nCntZ] = (0 + nCntX) + ((m_FieldData.m_nMax_Vtx_X + 1) * nCntZ);
		}
	}
	//角被りの頂点
	for (int nCntZ = 0; nCntZ < m_FieldData.m_nMax_Vtx_Z - 1; nCntZ++)
	{
		pIdx[(((m_FieldData.m_nMax_Vtx_X + 1) * 2 + 0) * (nCntZ + 1)) + (2 * nCntZ)] = m_FieldData.m_nMax_Vtx_X + ((m_FieldData.m_nMax_Vtx_X + 1) * nCntZ);
		pIdx[(((m_FieldData.m_nMax_Vtx_X + 1) * 2 + 1) * (nCntZ + 1)) + (1 * nCntZ)] = (m_FieldData.m_nMax_Vtx_X * 2 + 2) + ((m_FieldData.m_nMax_Vtx_X + 1) * nCntZ);
	}
	//インデックスバッファをアンロックする
	m_pIdxBuffWall->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CField::Uninit(void)
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
void CField::Update(void)
{

}
//=======================================================================
//描画処理
//=======================================================================
void CField::Draw()
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
	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,					//プリミティブの種類
		0,									//インデックスバッファの開始地点から最初のインデックスまでのオフセット
		m_nVtx_Num_Max,	//描画で使用される最小の頂点番号
		(m_FieldData.m_nMax_Vtx_X + 1)*(m_FieldData.m_nMax_Vtx_Z + 1),		//頂点の数
		0,	//インデックス配列の読み取りを開始する位置
		m_nPoly_Num_Max);	//三角形ポリゴンの数

}

