//-------------------------------
//bumperのクラスの処理
//作成者：林海斗
//-------------------------------
#include "bumper.h"
#include "model_spawner.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#define SPEED_DEFF (1.5)//速度の減衰の速さ
#define KNOCKBACK_SPEED (20.0)//ノックバックの速度
#define CREATE_MODELTYPE (9)//仮のマクロ（allmodel.txtに書いてあるbumperのモデルの番号を参照して）
//----------------------------------------
//コンストラクタ
//----------------------------------------
CBumper::CBumper(OBJTYPE nPriority) :CScene(nPriority)
{
}
//----------------------------------------
//デストラクタ
//----------------------------------------
CBumper::~CBumper()
{
}
//----------------------------------------
//初期化
//----------------------------------------

HRESULT CBumper::Init()
{
	//モデルの生成
	pModelSpawner = CModel_Spawner::Create(m_pos, m_rot, CREATE_MODELTYPE);
	m_fKnockBackSpeed = KNOCKBACK_SPEED;
	return S_OK;
}
//----------------------------------------
//破棄
//----------------------------------------

void CBumper::Uninit()
{
}
//----------------------------------------
//更新
//----------------------------------------

void CBumper::Update()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

	D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = CManager::GetGame()->GetPlayer()->GetPos();				//位置
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = pos - m_pos;			//敵と弾のベクトル
	float fLength = 0.0f;
	fLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
	//モデルの大きさの範囲にプレイヤーが入ったら跳ね返す
	if (fLength <= 100.0f)
	{
		m_bKnockBack = true;
	}

	//ノックバックの判定がオンだったら
	if (m_bKnockBack == true)
	{
		Knockback(pos, PlayerVec);

	}
	//プレイヤーに位置を反映
	pPlayer->SetPlayerPos(pos);

}
//----------------------------------------
//描画
//----------------------------------------

void CBumper::Draw()
{
}
//----------------------------------------
//インスタンス生成
//----------------------------------------

CBumper * CBumper::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBumper *pBumper = new CBumper(OBJTYPE_MODEL);
	if (pBumper != nullptr)
	{
		pBumper->m_pos = pos;
		pBumper->m_rot = rot;
		pBumper->Init();
	}
	return nullptr;
}
//----------------------------------------
//プレイヤーにノックバックさせる処理
//----------------------------------------
void CBumper::Knockback(D3DXVECTOR3 & Playerpos,const D3DXVECTOR3& PlayerVec)
{
	float fAng = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
											 //動き出す方向を求める
	float fSpeed_x = sinf(fAng)*KNOCKBACK_SPEED;
	float fSpeed_z = cosf(fAng)*KNOCKBACK_SPEED;
	//プレイヤーを動かす
	Playerpos.x += fSpeed_x;
	Playerpos.z += fSpeed_z;
	m_fKnockBackSpeed -= SPEED_DEFF;
	//ノックバックスピードがなくなったらノックバックしないようにする
	if (m_fKnockBackSpeed <= 0.0f)
	{
		m_fKnockBackSpeed = KNOCKBACK_SPEED;
		m_bKnockBack = false;
	}


}
