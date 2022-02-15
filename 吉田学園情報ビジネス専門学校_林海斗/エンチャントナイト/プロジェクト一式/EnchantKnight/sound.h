//＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
// サウンド処理 [sound.h]
// Author : 林海斗
//＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//===========================================================
// サウンドクラス
//===========================================================
class CSound
{
public:
	CSound();
	~CSound();

	//=================================
	// サウンドファイル
	//=================================
	typedef enum
	{
		//--------------------------
		// ＢＧＭ
		//--------------------------
		SOUND_LABEL_BGM_TITLE = 0,			// タイトルのBGM
		SOUND_LABEL_BGM_GAME,				// ゲームのBGM

		//--------------------------
		// ＳＥ
		//--------------------------
		SOUND_LABEL_SE_ENEMY_DEATH,			// 敵の消滅
		SOUND_LABEL_SE_SWORD_ATTACK,		// 剣攻撃
		SOUND_LABEL_SE_DAMAGE,				// ダメージ音
		SOUND_LABEL_SE_WALK,				// 歩く音
		SOUND_LABEL_SE_SELECT,				// カーソル音
		SOUND_LABEL_SE_ENTER,				// 決定音
		SOUND_LABEL_SE_WIN,				// 勝利
		SOUND_LABEL_SE_LOSE,				// 敗北
		SOUND_LABEL_SE_FIRE,				// 火魔法
		SOUND_LABEL_SE_ICE,				// 氷魔法
		SOUND_LABEL_SE_HEEL,				// 回復
		SOUND_LABEL_SE_ENEMYWALK,				// 敵の移動音

		SOUND_LABEL_MAX

	} SOUND_LABEL;

	//=============================================================================
	// プロトタイプ宣言
	//=============================================================================
	HRESULT InitSound(void);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	void ControllVoice(SOUND_LABEL label, float fVolume) { m_apSourceVoice[label]->SetVolume(fVolume); }

private:
	//=============================================================================
	// パラメータ構造体定義
	//=============================================================================
	typedef struct
	{
		char *filename;		// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;

	// 各音素材のパラメータ
	static PARAM m_aParam[SOUND_LABEL_MAX];

	//=============================================================================
	// プロトタイプ宣言
	//=============================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//=============================================================================
	// グローバル変数
	//=============================================================================
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

};
#endif
