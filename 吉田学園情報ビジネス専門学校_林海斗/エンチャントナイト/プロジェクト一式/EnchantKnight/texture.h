//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexture
{
public:
	CTexture();	// デフォルトコンストラクタ
	~CTexture();// デフォルトデストラクタ
	typedef enum
	{
		None = -1,		// テクスチャ無し
		Text,			// 仮テクスチャ
		TIME,			// タイムナンバー
		FADE,			
		GOAL,
		GAUGEBER,//ゲージ
		SKILLGAUGE,//必殺技ようのゲージ
		SHADOW,			//影
		FIELD,			//床
		SKY,			//空
		FIRE,
		COMMAND_ATTACK,
		COMMAND_MAGIC,
		COMMAND_ENHANCE,
		COMMAND_BG,//コマンドの背景
		A_BUTTON,
		B_BUTTON,
		X_BUTTON,
		L_BUTTON,
		BLAZE,
		ICECLE,
		HEEL,
		Map_Player,
		Map_Enemy,
		Map_Purpose,	//目的地に指すやつ
		Title,			//タイトル背景
		GameStart,		//ゲームスタート
		Gameover000,	//ゲームオーバーロゴ
		GameClear000,	//ゲームクリア背景
		Cursol,			//カーソル
		Retry,			//リトライ
		TitleBack001,		//タイトルに戻る
		GameBack,		//ゲームに戻る
		Operation,		//チュートリアル
		Operation01,
		Effect,			//エフェクト
		GlitterEffect,			//キラキラエフェクト
		MagicCircle_TypeB,			//キラキラエフェクト
		RingEffect,			//キラキラエフェクト
		LifeEffect,
		HitEffect,
		SwordEffect,			//エフェクト
		TargetText,//目的地表示用の
		RockOn,//ロックオン用
		SkillOk,//ロックオン用
		Max,			// テクスチャの最大数
	} Type;

	void Init(void);	// テクスチャの生成
	LPDIRECT3DTEXTURE9 GetTexture(Type Type) { return m_pTexture[Type]; }	// テクスチャの割り当て(テクスチャの種類) { return 選んだテクスチャを返す }
	void Uniinit(void);	// 終了

private:
	LPDIRECT3DTEXTURE9	m_pTexture[Max];	//テクスチャ
};

#endif