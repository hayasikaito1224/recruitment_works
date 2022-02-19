//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 羽鳥太一作
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "texture.h"
#include "Renderer.h"
#include "manager.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// テクスチャの生成
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する

	D3DXCreateTextureFromFile(pDevice,
		"",	// 無し
		&m_pTexture[None]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/text_texture.png",//仮テクスチャ
		&m_pTexture[Text]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Flag.png",//仮テクスチャ
		&m_pTexture[GOAL]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",//数字
		&m_pTexture[TIME]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/fade.png",//数字
		&m_pTexture[FADE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky_loop001.png",//空
		&m_pTexture[SKY]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",//影
		&m_pTexture[SHADOW]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Fire.png",//火
		&m_pTexture[FIRE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MagicCircle_TypeB.png",//魔法陣
		&m_pTexture[MagicCircle_TypeB]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Ring001.png",//ring
		&m_pTexture[RingEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/HeelEffect.jpg",//回復系の画像
		&m_pTexture[LifeEffect]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",//エフェクト
		&m_pTexture[Effect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GlitterEffect000.png",//エフェクト
		&m_pTexture[GlitterEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/hiteffect000.jpg",//エフェクト
		&m_pTexture[HitEffect]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Trajectory000.png",//エフェクト
		&m_pTexture[SwordEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GaugeBer.png",//ゲージ
		&m_pTexture[GAUGEBER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Field000.jpg",//床	
		&m_pTexture[FIELD]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Map_Player000.png",//床	
		&m_pTexture[Map_Player]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MapEnemy.png",//床	
		&m_pTexture[Map_Enemy]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Purpose000.png",//床	
		&m_pTexture[Map_Purpose]);
	//============================================================
	//COMMAND関係
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Attack000.png",//床	
		&m_pTexture[COMMAND_ATTACK]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Magic000.png",//床	
		&m_pTexture[COMMAND_MAGIC]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Enhance000.png",//床	
		&m_pTexture[COMMAND_ENHANCE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/CommandBG.png",//コマンドの背景
		&m_pTexture[COMMAND_BG]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Blaze000.png",//ブレイズ
		&m_pTexture[BLAZE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Icecle000.png",//アイシクル
		&m_pTexture[ICECLE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Heel000.png",//ヒール
		&m_pTexture[HEEL]);
	//============================================================
	//ボタン
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/A_button.png",//Aボタン
		&m_pTexture[A_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/B_button.png",//Aボタン
		&m_pTexture[B_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/X_button.png",//Aボタン
		&m_pTexture[X_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/L_button.png",//Aボタン
		&m_pTexture[L_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/SkillGauge000.png",//ゲージ
		&m_pTexture[SKILLGAUGE]);

	//============================================================
	//タイトル
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title000.png",//タイトル背景
		&m_pTexture[Title]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Please Enter.png",//ゲームスタート
		&m_pTexture[GameStart]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/operation_method000.png",//チュートリアル(ゲーム中に表示する操作方法)
		&m_pTexture[Operation]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Cursor.png",		  //カーソル
		&m_pTexture[Cursol]);

	//============================================================
	//ゲームオーバー
	//============================================================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/FAILED000.png",//ゲームオーバーロゴ
		&m_pTexture[Gameover000]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Result000.png",//ゲームクリア背景
		&m_pTexture[GameClear000]);
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Retry000.png",//リトライ
		&m_pTexture[Retry]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titleback.png",//タイトルに戻る
		&m_pTexture[TitleBack001]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Gameback.png",//ゲームに戻る
		&m_pTexture[GameBack]);
	//============================================================
	//ゲーム画面
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial000.png",//チュートリアル画面
		&m_pTexture[Operation01]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/targettext000.png",//目的地表示用の
		&m_pTexture[TargetText]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/stagecursor.png",//ロックオン用
		&m_pTexture[RockOn]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SkillOK000.png",//ロックオン用
		&m_pTexture[SkillOk]);

}

//=============================================================================
// 終了
//=============================================================================
void CTexture::Uniinit(void)
{
	for (int nCntTexture = 0; nCntTexture < Max; nCntTexture++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}