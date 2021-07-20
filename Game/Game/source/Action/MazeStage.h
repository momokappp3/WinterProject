/*****************************************************************
 * \file   MazeStage.h
 * \brief  ステージを生成する1人称動かせるクラス
 * \author 真壁先生 momoka
 * \date 2021 5/28
 * details
 *********************************************************************/
#pragma once
#include "Dxlib.h"
#include "../../Model.h"
#include <memory>
#include <tuple>
#include "../../../../AppFrame/Input.h"
#include "../../ModelAnimation.h"
#include "StrongBox.h"

class MazeStage {
public:
	MazeStage();
	virtual ~MazeStage();
	virtual bool Initialize(std::shared_ptr<SoundManager> sound);
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual bool CheckPosition(VECTOR position);

	void GetModeCount(int num) {
		_modeCount = num;
	}

	bool GetIsDoorArea() {
		return _isDoorArea;
	}

	bool GetIsDoorAnim() {
		return _isDoorAnim;
	}

	bool GetIs3D() {
		return _is3D;
	}

	bool GetIsHit() {
		return _isHit;
	}

	int GetDoorHandle() {
		return _pDoor->GetHandle();
	}

	std::unique_ptr<StrongBox>& GetBox() {
		return _pStrongBox;
	}

	//行き止まりのタイプ
	enum class NoPassageType {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
		MAX
	};

private:
	void GameDraw();
	void StageInit();
	void _CheckChipRoute(int x1, int y1, int x2, int y2, int cntmax);
	int CheckChipRoute(int x1, int y1, int x2, int y2, int cntmax);
	int MakeShortRoute(int x1, int y1, int x2, int y2, int cntmax);
	void SearchNoPassage();  //行き止まりと行き止まりのタイプを判定

	std::unique_ptr<ModelAnimation> _pDoor;
	std::unique_ptr<StrongBox> _pStrongBox;
	std::unique_ptr<Input> _pKeyInput;
	std::shared_ptr<SoundManager> _pSoundManeger;
	std::vector<std::tuple<int, int, NoPassageType>> _noPassageList;  //行き止まりの座標とタイプ

	static const int CHIP_W = 16;
	static const int CHIP_H = 16;

	// 画像一覧
	enum {
		ECG_CHIP_BLACK = 0,
		ECG_CHIP_WHITE,
		ECG_CHIP_BROWN,
		ECG_CHIP_RED,
		ECG_CHIP_BLUE,
		ECG_CHIP_GREEN,

		ECG_STAR,
		ECG_HEART,

		_ECG_EOT_	// End of table.
	};

	int	cg[_ECG_EOT_];

	// 迷路の大きさとマップデータ
	// サイズは奇数のみ
	static const int MAZE_W = 79;
	static const int MAZE_H = 43;
	char maze[MAZE_H * MAZE_W];

	// 主人公位置
	int plx, ply;

	// ゴール位置
	int glx, gly;

	// 主人公の移動歩数サーチ数
	int route_search = 8;

	// 指定位置からの歩数情報格納配列
	int  mazeroute_c[MAZE_W * MAZE_H];

	// 指定位置から指定位置までの最短距離ルート格納配列
	char mazeroute_f[MAZE_W * MAZE_H];

	// デバッグ用
	int search_time_ms = 0, search_call_cnt = 0;	// サーチ経過時間,サーチ呼び出し回数
	int draw_route_c = 0;		// mazeroute_c[]を描画するか否か
	int draw_checkroute = 0;	// CheckRoute()中に描画するか否か
	int draw_route_f = 0;		// mazeroute_f[]を描画するか否か
	int draw_makeroute = 0;		// MakeRoute()中に描画するか否か

	VECTOR _lastPlayer3DPosi;
	VECTOR _player3DPosi;		// プレイヤー座標
	VECTOR _lastDir;
	VECTOR vDir;		// 向いている方向
	float playerSpeed;	// 1フレームの移動力

	int _modeCount;

	bool _is3D;
	bool _isDoorArea;
	bool _isDoorAnim;
	bool _isBoxArea;
	bool _isBoxAnimation;
	bool _isHit;

	//===========
	//エフェクト

	int _effectLoadHandle;
	int _effectPlayHandle;
	int _effectPlayHandle2;

	int _effectTime;

	//===============
	//当たり判定
	VECTOR _attack3DPosi;
};
