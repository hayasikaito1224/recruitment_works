//========================================
// モーションの実装
// Author:村元翼
//========================================
#ifndef _MOTION_H_
#define _MOTION_H_
#include "main.h"
class CModel;

//----------------------------
// マクロ定義
//----------------------------
#define MAX_PARTS (20)			// モデル(パーツ)の最大数
#define MAX_KEY (2)				// キーの最大数
#define MAX_MOTION (10)			// モーションの最大数
#define MAX_LIMIT (15)
//------------------------------------------
// モーションクラス
//------------------------------------------
class CMotion
{
public:

	//----------------------------
	// 構造体
	//----------------------------
	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
		MOTIONTYPE_MOVE,		// ムーブモーション
		MOTIONTYPE_ATTACK_0,		// アタックモーション
		MOTIONTYPE_ATTACK_1,		// アタックモーション
		MOTIONTYPE_ATTACK_2,		// アタックモーション
		MOTIONTYPE_JUMP,		// ジャンプモーション
		MOTIONTYPE_MAX

	}MOTIONTYPE;

	// キーの構造体
	typedef struct
	{
		float fPosX;	//位置x
		float fPosY;	//位置y
		float fPosZ;	//位置z
		float fRotX;	//向きx
		float fRotY;	//向きy
		float fRotZ;	//向きz

	}KEY;

	// キー情報の構造体
	typedef struct
	{
		float nFrame;					// 再生フレーム
		std::vector<KEY> aKey;		// 各パーツのキー要素(モデルによってパーツ数が変わる)(何パーツ目の位置と回転が入っているかの情報)	
		//			↑パーツの最大数(総数)
	}KEY_INFO;

	// モーション情報の構造体
	typedef struct
	{
		bool bLoop;					// ループするかどうか
		int nMaxKey;				// キーの総数
		std::vector<KEY_INFO> aKeyInfo;// キー情報[キーの最大数](モーションの何番目の動きが入っているかの情報)
	}MOTION_INFO;

	//------------------------------------
	// コンストラクタ、デストラクタ
	//------------------------------------
	CMotion();
	~CMotion();

	//------------------------------------
	// プロトタイプ宣言
	//------------------------------------
	HRESULT Init(void);								// 初期化
	void PlayMotion(int nNumParts, CModel **apModel, int& motionType, int& motionTypeLast);							// モーションの再生

	void PlayMotion(const int nNumParts,CModel **apModel, int& motionType, int& motionTypeLast,
		bool& bStop,bool& bAttack,bool& bDelay, bool bMotionReset);							// モーションの再生
	void NoLoopPlayMotion(const int nNumParts, CModel **apModel, int& motionType, int& motionTypeLast,
		bool& bStop);							// モーションの再生
	void MotionLoad(const char *sMotionFileName);							// モーションテキストを読み込む関数
	void Drawtext();
	std::vector<MOTION_INFO> GetMotionInfo() { return m_aMotionInfo; }
private:
protected:
	std::vector<MOTION_INFO> m_aMotionInfo;// モーション情報[モーションタイプ数](現在何をしているかのモーションの情報が入っている)
	MOTIONTYPE m_MotionType;					// 現在のモーションタイプ
	MOTIONTYPE m_MotionTypeLast;				// 前回のモーションタイプ
	int m_NumKey;								// 現在のキー番号
	float m_motionCounter;						// モーションカウンター
	int m_Timer;

};
#endif _MOTION_H_