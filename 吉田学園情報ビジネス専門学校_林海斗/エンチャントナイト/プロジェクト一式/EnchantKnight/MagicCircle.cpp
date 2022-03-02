//=========================================
//魔法陣の処理
//=========================================
#include "main.h"
#include "MagicCircle.h"
#include "manager.h"
#include "renderer.h"
#include "Scene3D.h"
#include "Particle.h"
#include "shadow.h"
#define MAX_ERASE_TIME (60)
//=============================================================================
// コンストラクタ
//=============================================================================
CMagicCircle::CMagicCircle(OBJTYPE nPriority) : CEffect(nPriority)
{
	m_col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	m_size = { 1.0f,1.0f,1.0f };
	m_fRotSpeed = 0.5f;
	m_fEraseMaxTime = MAX_ERASE_TIME;
	m_fEraseTime = 0.0f;
	m_fScaleSpeed = 1.5f;
	m_bErase = false;
	m_pParticle = nullptr;
	m_bSizeAdd = false;
	m_bSizeDef = false;
	m_rot = { D3DXToRadian(90.0f),0.0f,0.0f };
	pShadow = nullptr;
}
//=============================================================================
// デストラクタ
//=============================================================================
CMagicCircle::~CMagicCircle()
{

}

//---------------------------------------------------
//インスタンス生成
//---------------------------------------------------
CMagicCircle * CMagicCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fsize, float fMaxTime, bool bRot, bool bEraseStart,
	D3DXCOLOR col, CTexture::Type type, const float fSpeed)
{//	位置, 移動量, サイズ,色, 小さくなるスピード
 //インスタンス生成
	CMagicCircle *pEffect = new CMagicCircle(OBJTYPE_EFFECT);
	pEffect->m_pos = pos;
	pEffect->m_rot = rot;
	pEffect->m_fMaxSize = fsize;
	pEffect->m_col = col;
	pEffect->m_TexType = type;
	pEffect->m_bEraseStart = bEraseStart;
	pEffect->m_fEraseMaxTime = fMaxTime;
	pEffect->m_bSizeAdd = bEraseStart;
	pEffect->m_fScaleSpeed = fSpeed;
	pEffect->bOnBill(false);
	pEffect->m_bRotX = bRot;
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
HRESULT CMagicCircle::Init()
{
	CEffect::Init();
	//カリングをするようにする
	SetCulling(true);
	if (m_pParticle == nullptr)
	{
		m_pParticle = new CParticle();
	}
	
	return S_OK;
}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
void CMagicCircle::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if (pShadow != nullptr)
	{
		pShadow->Uninit();
		pShadow = nullptr;
	}
	Release();
}
//---------------------------------------------------------
//更新処理
//---------------------------------------------------------
void CMagicCircle::Update()
{
	Setpos(m_pos, m_size);//頂点情報の設定

	SetCol_E(m_col);
	//pShadow->SetCol(m_col);
	//移動処理
	m_pos += m_move;
	if (m_bRotX == false)
	{
		m_rot.y += D3DXToRadian(m_fRotSpeed);

	}
	else
	{
		m_rot.z += D3DXToRadian(m_fRotSpeed);

	}
	//魔法陣を消すとき
	if (m_bEraseStart == true)
	{
		m_fEraseTime++;
		if (m_fEraseTime >= m_fEraseMaxTime)
		{
			//透明にする処理を始める
			m_bColDef = true;
			//さらにサイズを大きく処理を始める
			m_bEraseSizeStart = true;
		}
	}

	//サイズが大きくなる処理
	if (m_bSizeAdd == true)
	{
		m_bSizeDef = false;
		SizeAdd();
	}
	//サイズが小さくなる処理
	if (m_bSizeDef == true)
	{
		m_bSizeAdd = false;
		SizeDef();
	}
	//サイズをよりでかくする処理
	if (m_bEraseSizeStart == true)
	{
		ColSizeAdd();
	}
	//色の値を小さくする判定がオンなら
	if (m_bColDef == true)
	{
		//明るさを下げる
		m_col.a -= 0.03f;
	}
	
	//α値が0以下になったら消える
	if (m_col.a <= 0.0)
	{
		m_bUninit = true;
	}
	//終了判定がオンなら
	if (m_bUninit == true)
	{
		//終了処理を呼ぶ
		Uninit();
	}

}
//---------------------------------------------------------
//描画処理
//---------------------------------------------------------
void CMagicCircle::Draw()
{
	CEffect::Draw();
}

//---------------------------------------------------------
//サイズが大きくなる処理
//---------------------------------------------------------
void CMagicCircle::SizeAdd()
{
	//サイズを増やす
	m_size.x += m_fScaleSpeed;
	m_size.y += m_fScaleSpeed;
	if (m_size.x >= m_fMaxSize)
	{
		m_size.x = m_fMaxSize;
		m_size.y = m_fMaxSize;
		//サイズが大きくなる処理を停止
		m_bSizeAdd = false;
		//消去処理実行の判定がオンだっだら
		if (m_bErase == true)
		{
			//消去処理の実行
			m_bEraseStart = true;
		}
	}
}
//---------------------------------------------------------
//サイズが小さくなる処理
//---------------------------------------------------------
void CMagicCircle::SizeDef()
{
	m_size.x -= m_fScaleSpeed;
	m_size.y -= m_fScaleSpeed;
	if (m_size.x < 0.0f)
	{
		m_size.x = 0.0f;
		m_size.y = 0.0f;
		m_bSizeDef = false;
	}
}
//---------------------------------------------------------
//サイズが大きくするだけの処理
//---------------------------------------------------------
void CMagicCircle::ColSizeAdd()
{
	m_size.x += m_fScaleSpeed;
	m_size.y += m_fScaleSpeed;

}
//---------------------------------------------------------
//サイズを設定
//---------------------------------------------------------
void CMagicCircle::SetSize()
{

	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	m_pVtxBuff->Unlock();

}
