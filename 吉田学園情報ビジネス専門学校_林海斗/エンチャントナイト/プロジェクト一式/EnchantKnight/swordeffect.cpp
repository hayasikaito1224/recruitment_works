#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"	
#include "Scene3D.h"
#include "swordeffect.h"
CSwordEffect::CSwordEffect(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMax_Vtx_X = 0;
	m_nMax_Vtx_Z = 1;
	m_nVtx_Num_Max = 0;
	m_nIdx_Num_Max = 0;
	m_nPoly_Num_Max = 0;
	VtxPos.clear();
	m_bDraw = true;
}

CSwordEffect::~CSwordEffect()
{

}

//------------------------------------------------------------------------------------------------
//インスタンス生成処理(位置、大きさ、角度、X軸のブロック数、Z軸のブロック数、貼るテクスチャの種類)
//-------------------------------------------------------------------------------------------------
CSwordEffect *CSwordEffect::Create(D3DXVECTOR3 Startpos, D3DXVECTOR3 Endpos,D3DXVECTOR3 pos, int nNum_X,CTexture::Type Tex)
{
	//インスタンス生成
	CSwordEffect *pField = new CSwordEffect(OBJTYPE_EFFECT);
	pField->VtxPos.resize(nNum_X);
	for (int nVtxpos = 0; nVtxpos < (int)pField->VtxPos.size(); nVtxpos++)
	{
		pField->VtxPos[nVtxpos] = { 0.0f,0.0f,0.0f };
	}
	pField->m_pos = pos;
	pField->m_nMax_Vtx_X = (float)nNum_X;
	pField->m_pTexture = CManager::GetTexture()->GetTexture(Tex);

	if (pField != NULL)
	{
		pField->Init();
	}

	return pField;
}

void CSwordEffect::SetMoveTex(float fSpeedX, float fSpeedY)
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
	for (int nCntZ = 0; nCntZ < m_nMax_Vtx_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * nCntX) + m_MoveTexX, 0.0f + (1.0f * nCntZ) + m_MoveTexX);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
void CSwordEffect::SetCol(float fAlpha)
{
	//-----------------------------
	//頂点情報
	//-----------------------------
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntZ = 0; nCntZ < m_nMax_Vtx_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].col = D3DXCOLOR(1.0, 1.0, 1.0, fAlpha);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
//-----------------------------------------------------
//剣の始点位置の設定
//-----------------------------------------------------
void CSwordEffect::SetSwordPos(D3DXVECTOR3 Startpos, D3DXVECTOR3 Endpos)
{
	VtxPos[1] = Startpos;
	VtxPos[0] = Endpos;
}

//-----------------------------------------------------
//剣の終点位置の設定
//-----------------------------------------------------
void CSwordEffect::SetLastSwordPos(D3DXVECTOR3 Startpos, D3DXVECTOR3 Endpos)
{
	m_LastStartpos = Startpos;
	m_LastEndpos = Endpos;
	VtxPos[m_nCntLastPos]= m_LastStartpos;
	VtxPos[m_nCntLastPos - 1]= m_LastEndpos;
	m_nCntLastPos -= 2;
	if (m_nCntLastPos < 2)
	{
		m_nCntLastPos = (int)VtxPos.size() - 1;
	}
	
}

void CSwordEffect::Drawtext()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[3200];
	int nNum = 0;
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();

	nNum = sprintf(&str[0], "\n\n\n");
	for (int nCnt = 0; nCnt < (int)VtxPos.size(); nCnt++)
	{
		nNum += sprintf(&str[nNum], " [VtxPos%d] X%.2f,Y%.2f,Z%.2f \n",nCnt, VtxPos[nCnt].x, VtxPos[nCnt].y, VtxPos[nCnt].z);
	}


	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// テキスト描画
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

HRESULT CSwordEffect::Init(void)
{
	m_nVtx_Num_Max = (m_nMax_Vtx_X + 1)*(m_nMax_Vtx_Z + 1);
	m_nIdx_Num_Max = (m_nMax_Vtx_X + 1)*(m_nMax_Vtx_Z + 1) + (m_nMax_Vtx_Z - 1)*(m_nMax_Vtx_X + 3);
	m_nPoly_Num_Max = 2 * m_nMax_Vtx_X * m_nMax_Vtx_Z + (m_nMax_Vtx_Z * 4) - 4;
	m_nCntLastPos = (int)VtxPos.size() - 1;
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	//																 //テクスチャ画像の設定

	////頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVtx_Num_Max,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_3D,
	//	D3DPOOL_MANAGED,
	//	&m_pVtxBuff,
	//	NULL);
	////-----------------------------
	////頂点情報
	////-----------------------------
	//VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//						//頂点バッファをロックし、頂点データへのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//int nVtxNum = 0;
	//for (int nCntY = 0; nCntY < m_nMax_Vtx_Z + 1; nCntY++, nVtxNum++)
	//{
	//	pVtx[nVtxNum].pos = D3DXVECTOR3(
	//		VtxPos[nCntY].x,
	//		VtxPos[nCntY].y,
	//		VtxPos[nCntY].z);

	//	pVtx[nVtxNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//	pVtx[nVtxNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * 1) + m_MoveTexX, 0.0f + (1.0f*nCntY) + m_MoveTexX);

	//}

	//nVtxNum = 0;
	////頂点バッファをアンロックする
	//m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSwordEffect::Uninit(void)
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
// 更新処理
//=============================================================================
void CSwordEffect::Update(void)
{

}
//=======================================================================
//描画処理
//=======================================================================
void CSwordEffect::Draw()
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

	//																 //頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVtx_Num_Max,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_3D,
	//	D3DPOOL_MANAGED,
	//	&m_pVtxBuff,
	//	NULL);
	////-----------------------------
	////頂点情報
	////-----------------------------
	//VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//						//頂点バッファをロックし、頂点データへのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//int nVtxNum = 0;
	//for (int nCntX = 0; nCntX < (int)VtxPos.size() - 1; nCntX += 2)
	//{
	//	for (int nCntY = 0; nCntY < m_nMax_Vtx_Z + 1; nCntY++, nVtxNum++)
	//	{
	//		pVtx[nVtxNum].pos = D3DXVECTOR3(
	//			VtxPos[nCntX + nCntY].x - m_pos.x,
	//			VtxPos[nCntX + nCntY].y - m_pos.y,
	//			VtxPos[nCntX + nCntY].z - m_pos.z);

	//		pVtx[nVtxNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//		pVtx[nVtxNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//		pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f/ (int)VtxPos.size())*nCntX, 0.0f + (1.0f*nCntY));


	//	}

	//}

	//nVtxNum = 0;
	////頂点バッファをアンロックする
	//m_pVtxBuff->Unlock();

	////頂点バッファをでバスのでーたすとりーむに設定
	//pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

	//pDevice->SetTexture(0, m_pTexture);
	//if (m_bDraw == true)
	//{
	//	//ポリゴンの描画
	//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,					//プリミティブの種類
	//		0,									//インデックスバッファの開始地点から最初のインデックスまでのオフセット
	//		m_nVtx_Num_Max,	//描画で使用される最小の頂点番号
	//		(m_nMax_Vtx_X + 1)*(m_nMax_Vtx_Z + 1),		//頂点の数
	//		0,	//インデックス配列の読み取りを開始する位置
	//		m_nPoly_Num_Max);	//三角形ポリゴンの数

	//}

}

