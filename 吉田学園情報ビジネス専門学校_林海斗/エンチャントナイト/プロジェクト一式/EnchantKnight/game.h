//--------------------------------------
//タイトルの処理
//--------------------------------------
#ifndef _GAME_H
#define _GAME_H
#include "main.h"
#include "scene.h"
#define MAX_MAGICCOMMAND (3)
#define MAX_COMMAND (3)
#define MAX_SPAWNER (3)
#define PLAYER_CP (100)		//マナの多さ

//前方宣言
class CBg;
class CPlayer;
class CScene3D;
class CModel_Spawner;
class CScore;
class CField;
class CMeshSphere;
class CEffectSphere;
class CParticle;
class CEnemy_Poyo;
class CPolygon;
class CStage;
class CCamera;
class CGoal;
class CCommandUI;
class CMagicUI;
class CGauge;
class CEnemy_Spawner;
class CCircleGauge;

class CGame
{
public:
	typedef enum
	{
		LEVEL_EASY=0,
		LEVEL_NORMAL,
		LEVEL_HARD,
		LEVEL_MAX
	}LEVEL;

	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(LPD3DXMATRIX mtrix);

	static CPlayer *GetPlayer(void) { return m_Player; }
	static CModel_Spawner *GetModel(void) { return m_pModel; }
	static CScore			*GetScore(void) { return m_pScore; }
	static CMeshSphere		*GetMeshSphere(void) { return m_pMeshSphere; }
	static CPolygon		*GetCursol(void) { return m_Cursol; }
	static CGoal		*GetGoal(void) { return m_Goal; }
	static CParticle		*GetParticle(void) { return m_Particle; }
	static CGauge		*GetHPGauge() { return m_pHPGauge; }
	static CGauge		*GetMPGauge() { return m_pMPGauge; }
	static CCircleGauge		*GetCPGauge() { return m_pCPGauge; }
	static CEnemy_Spawner		*GetEnemySpawner(int nNum) { return m_pEnemySpawner[nNum]; }

	static CMagicUI	*GetMagicCommand(int nType) { return m_pMagicCommand[nType]; }

private:
	static CGauge *m_pHPGauge;
	static CGauge *m_pMPGauge;
	static std::vector<CPolygon*> m_pCStock;
	static CEnemy_Spawner	*m_pEnemySpawner[MAX_SPAWNER];
	static CCircleGauge	*m_pCPGauge;

	static CCommandUI	*m_pCommand;
	static CMagicUI	*m_pMagicCommand[MAX_MAGICCOMMAND];
	static CScore		*m_pScore;
	static CBg			*m_pBg;
	static CPlayer		*m_Player;
	static CModel_Spawner *m_pModel;
	static CField		*m_pField;//床
	static CField		*m_pSea;//床
	static CPolygon		*m_Polygon;
	static CPolygon		*m_Cursol;
	static CMeshSphere	*m_pMeshSphere;
	static CEffectSphere *m_pEffectSphere;
	static CStage		*m_pStage;
	static CGoal		*m_Goal;
	static CParticle	*m_Particle;
	static int			m_nChancePoint;											//チャンスポイント
	static int			m_nChanceStock;
	bool m_bPush;
	bool m_bEnd;
	int m_nCntDelay;//ディレイのカウンタ
	float				m_fAlpha;
	bool				m_bNextMode;//二重押し防止変数
	int					m_nTimer;//カウンター
	CScene3D			*m_pFlore;//床
};


#endif //_RESULT_H