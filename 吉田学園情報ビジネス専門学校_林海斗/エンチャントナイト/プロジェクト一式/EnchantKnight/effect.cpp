//=============================================================================
// エフェクトの処理
//=============================================================================
#include "main.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "Scene3D.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(OBJTYPE nPriority) : CScene(nPriority)
{
	m_col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	m_fDefSpeedA = 0.01f;
	m_bOnBillbord = true;
	m_bCulling = false;
	m_bAddColA = true;
	m_bUninit = false;
	m_bScale = false;
	m_fScaleSpeed = 1.0f;
	m_bDefScale = false;
	m_bScaleX = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//---------------------------------------------------
//インスタンス生成
//---------------------------------------------------
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 m_move, D3DXVECTOR3 size, D3DXCOLOR col, bool bGravity,
	float fGravity, float fDefSpeed, bool bIsBill, CTexture::Type type, float fAng, bool bScele, bool bScaleX, bool bDef)
{//	位置, 移動量, サイズ,色, 小さくなるスピード
	//インスタンス生成
	CEffect *pEffect = new CEffect(OBJTYPE_EFFECT);
	pEffect->m_pos = pos;
	pEffect->m_size = { size.x*5,size.y*5,size.z*5};
	pEffect->m_initsize = { size.x * 5,size.y * 5,size.z * 5 };

	pEffect->m_fDefSpeedA = fDefSpeed;
	pEffect->m_move = m_move;
	pEffect->m_col = col;
	pEffect->m_TexType = type;
	pEffect->m_bGravity = bGravity;
	pEffect->m_fGravity = fGravity;
	pEffect->m_fAng = fAng;
	pEffect->m_bOnBillbord = bIsBill;
	pEffect->m_bScale = bScele;
	pEffect->m_bScaleX = bScaleX;
	pEffect->m_bDefScale = bDef;
	if (pEffect != NULL)
	{
		//画像読み込み
		pEffect->Init();
	}

	return pEffect;
}

//----------------------------------------------------
//初期化処理
//-----------------------------------------------------
HRESULT CEffect::Init()
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
	m_pTexture = CManager::GetTexture()->GetTexture(m_TexType);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

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
	Setpos(m_pos, m_size);//頂点情報の設定

	return S_OK;
}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
void CEffect::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}
//---------------------------------------------------------
//更新処理
//---------------------------------------------------------
void CEffect::Update()
{
	//色情報の更新
	SetCol_E(m_col);
	//X軸側が大きくなる判定なら
	if (m_bScaleX)
	{
		//フレーム回ルごとにサイズを大きくなる
		m_size.x += m_fScaleSpeed*3.0f;
		//m_size.y +=m_fScaleSpeed/2.0f;
		if (m_size.x >= m_initsize.x*20.0f)
		{
			m_bScale = false;
		}

	}
	//サイズが大きくなる判定なら
	if (m_bScale==true)
	{
		//フレーム回ルごとにサイズを大きくなる
		m_size.x +=m_fScaleSpeed;
		m_size.y +=m_fScaleSpeed;
		if (m_size.x >= m_initsize.x*5.0f)
		{
			//縮小の開始
			m_bDefScale = true;
			m_bScale = false;
		}
	}
	//縮小判定なら
	if (m_bDefScale)
	{
		m_size.x -= m_fScaleSpeed*1.2f;
		m_size.y -= m_fScaleSpeed*1.2f;
		if (m_size.y <= 0.0f)
		{
			m_size.y = 0.0f;
		}
		if (m_size.x <= 0.0f)
		{
			m_size.x = 0.0f;
		}
	}
	//頂点情報の設定
	Setpos(m_pos, m_size);

	//移動処理
	m_pos += m_move;
	//不透明になる処理
	if (m_bAddColA == true)
	{
		AddColA();
	}

	//明るさを下げる
	m_col.a -= m_fDefSpeedA;

	//重力判定がオンなら
	if (m_bGravity == true)
	{
		//エフェクトに重力を掛ける
		m_move.y -= m_fGravity;
	}
	//α値が0以下になったら消える
	if (m_col.a <= 0.0)
	{
		//消去判定をオンにする
		m_bUninit = true;
	}
	//消去判定がオンなら
	if (m_bUninit == true)
	{
		//終了処理を呼び出す
		Uninit();

	}
}
//---------------------------------------------------------
//更新処理
//---------------------------------------------------------
void CEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxTrans, mtxRot;				//位置計算用マトリックス
	D3DXMATRIX mtxScale;				//スケール計算用マトリックス

	//ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//上から書き込むことをしないようにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	if (m_bCulling == true)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングをする
	}
	D3DXMatrixIdentity(&m_mtxWorld);//ワールドマトリックスの初期化

	if (m_bOnBillbord == true)
	{
		pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);//ビューマトリックスを取得

		//ポリゴンをカメラに対して正面に向ける
		//逆行列を求める	
		m_mtxWorld._11 = m_mtxView._11;
		m_mtxWorld._12 = m_mtxView._21;
		m_mtxWorld._13 = m_mtxView._31;
		m_mtxWorld._21 = m_mtxView._12;
		m_mtxWorld._22 = m_mtxView._22;
		m_mtxWorld._23 = m_mtxView._32;

	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//開始する頂点のインデックス
		2);					//描画するプリミティブ数
	//カリングする判定なら
	if (m_bCulling == true)
	{
		// カリングをする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//ライトの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//元に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


}
//---------------------------------------------------------
//色の設定
//---------------------------------------------------------
void CEffect::SetCol_E(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();

}
//---------------------------------------------------------
//頂点座標の更新
//---------------------------------------------------------
void CEffect::Setpos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	//サイズを反映
	m_size = scale;

	D3DXVECTOR3 posOrigin[4];
	D3DXVECTOR3 Rotatepos[4];

	//スケールサイズ込みの４頂点
	posOrigin[0].x = -m_size.x;
	posOrigin[1].x = m_size.x;
	posOrigin[2].x = -m_size.x;
	posOrigin[3].x = m_size.x;

	posOrigin[0].y = -m_size.y;
	posOrigin[1].y = -m_size.y;
	posOrigin[2].y = m_size.y;
	posOrigin[3].y = m_size.y;

	//回転の計算
	Rotatepos[0].x = (posOrigin[0].x * cosf(m_fAng)) + (posOrigin[0].y * sinf(m_fAng));
	Rotatepos[1].x = (posOrigin[1].x * cosf(m_fAng)) + (posOrigin[1].y * sinf(m_fAng));
	Rotatepos[2].x = (posOrigin[2].x * cosf(m_fAng)) + (posOrigin[2].y * sinf(m_fAng));
	Rotatepos[3].x = (posOrigin[3].x * cosf(m_fAng)) + (posOrigin[3].y * sinf(m_fAng));

	Rotatepos[0].y = (posOrigin[0].x * sinf(m_fAng)) - (posOrigin[0].y * cosf(m_fAng));
	Rotatepos[1].y = (posOrigin[1].x * sinf(m_fAng)) - (posOrigin[1].y * cosf(m_fAng));
	Rotatepos[2].y = (posOrigin[2].x * sinf(m_fAng)) - (posOrigin[2].y * cosf(m_fAng));
	Rotatepos[3].y = (posOrigin[3].x * sinf(m_fAng)) - (posOrigin[3].y * cosf(m_fAng));

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(Rotatepos[0].x, Rotatepos[0].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Rotatepos[1].x, Rotatepos[1].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Rotatepos[2].x, Rotatepos[2].y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Rotatepos[3].x, Rotatepos[3].y, 0.0f);

	m_pVtxBuff->Unlock();

}
//---------------------------------------------------------
//出現時に不透明になっていく処理
//---------------------------------------------------------
void CEffect::AddColA()
{
	//出現時に不透明になっていく処理
	m_fAddA += 0.1f;

	//α値が1.0fになったら
	if (m_fAddA >= 1.0f)
	{
		m_fAddA = 1.0f;
		//不透明処理をしないようにする
		m_bAddColA = false;
	}

	//色情報の更新
	SetCol_E({ m_col.r,m_col.g,m_col.b,m_fAddA });
}



