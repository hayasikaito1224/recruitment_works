//========================================
// ステージの配置処理
// Author:林
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"
//------------------------------------------
// モーションクラス
//------------------------------------------
class CStage
{
public:
	//------------------------------------
	// コンストラクタ、デストラクタ
	//------------------------------------
	CStage();
	~CStage();
	void Load(const char *cFileName);

private:
};
#endif _STAGE_H_