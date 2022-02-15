//----------------------------------
//ポリゴンの処理
//----------------------------------
#ifndef _LIGHT_H
#define _LIGHT_H
#include "main.h"
#include "scene.h"
#include "texture.h"
//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init(D3DXCOLOR Diffuse, D3DXVECTOR3 vecDir);
private:
	D3DLIGHT9 m_light;//ライトの情報
	static int m_nCntLight;//生成数のカウント
	int m_nId;//生成数
};


#endif // !_LIGHT_H
#pragma once
