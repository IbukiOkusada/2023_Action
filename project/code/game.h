//===============================================
//
// ゲーム画面の管理処理 [game.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _GAME_H_		// このマクロが定義されていない場合
#define _GAME_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"
#include <mutex>

// 前方宣言
class CScore;
class CTime;
class CPlayer;
class CMapCamera;
class CFileLoad;
class CEditor;
class CPause;
class CObject2D;
class CClient;
class CMeshDome;
class CCountDown;

// マクロ定義
#define NUM_FILTER	(2)

//===============================================
// ゲームクラスの定義(派生クラス)
//===============================================
class CGame : public CScene
{
public:

	enum STATE
	{
		STATE_TIMEATTACK = 0,
		STATE_MULTI,
		STATE_END,
		STATE_MAX
	};

public:

	// メンバ関数
	CGame();	// コンストラクタ
	~CGame();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetState(STATE state) { m_state = state; }
	void SendPosition(const D3DXVECTOR3& pos);
	void SendRotation(const D3DXVECTOR3& rot);
	void SendDamage(const int nDamage);
	void SendLife(const int nLife);
	void SendSetUp(void);
	void SendGoal(void);

	// メンバ関数(ポインタ)
	CScore *GetScore(void);
	CTime *GetTime(void);
	CPlayer *GetPlayer(void);
	CFileLoad *GetFileLoad(void);
	CPause *GetPause(void);
	CEditor *GetEditor(void);
	CMapCamera *GetMapCamera(void) { return m_pMapCamera; }

private:

	void GimmickSet(void);

	// TCP通信用関数
	void Online(void);
	void ByteCheck(char *pRecvData, int nRecvByte);
	void OnlineEnd(void);
	void AddressLoad(char *pAddrss);

	CFileLoad *m_pFileLoad;		// ファイル読み込みのポインタ
	CScore *m_pScore;		// スコアのポインタ
	CTime *m_pTime;		// タイムのポインタ
	CPlayer *m_pPlayer;	// プレイヤーのポインタ
	CPause *m_pPause;		// ポーズのポインタ
	CEditor *m_pEditor;	// エディターのポインタ
	CMapCamera *m_pMapCamera;		// ミニマップ用カメラ
	CMeshDome *m_pMeshDome;		// メッシュドームのポインタ
	CObject2D *m_pStart;	// スタート時の文字
	CClient *m_pClient;	// クライアントのポインタ
	char m_aAddress[30];	// 接続先サーバーのアドレス
	static STATE m_state;	// 状態
	int m_nSledCnt;		// 現在動作しているスレッド数
	CCountDown *m_pCountDown;	// カウントダウンのポインタ
	WSADATA m_wsaData;
	std::mutex m_mutex;
	bool m_bEnd;
};

#endif