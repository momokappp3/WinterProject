/*
** ・マップを描画する元
** ・マップを自動生成する
** ・主人公とゴールの設定、移動、穴あけ
** ・主人公から、移動できる歩数範囲をチェック
** ・ゴールから主人公の位置までの最短ルートを調べる
** マウス処理追加
*/
#include "MazeStage.h"
#include "../../../../AppFrame/source/Mode/ModeBase.h"
#include "../../ResourceServer.h"
#include "../ApplicationMain.h"
#include "../Mode/ModeGame.h"
#include "../../../../AppFrame/Types.h"
#include "EffekseerForDXLib.h"

// 3D用
const float BLOCK_SIZE = 80.0f;			// ブロックのサイズ
const float CAMERA_Y = (BLOCK_SIZE / 2.0f);	// カメラの高さ
const float	MOVE_BLOCK_SPEED = 20.0f;		// 1ブロック移動するフレーム数

MazeStage::MazeStage() {

	//_player3DPosi = { 0,0,0 };
	//_lastPlayer3DPosi = { 0,0,0 };

	_is3D = false;
	_isDoorArea = false;
	_isDoorAnim = false;
	_isBoxArea = false;
	_isBoxAnimation = false;
	_isHit = false;

	_effectLoadHandle = -1;
	_effectPlayHandle = -1;
	_effectPlayHandle2 = -1;

	_effectTime = 0;

	_pSoundManeger = nullptr;
}

MazeStage::~MazeStage() {

	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(_effectLoadHandle);

	// Effekseerを終了する。
	Effkseer_End();
}

bool MazeStage::Initialize(std::shared_ptr<SoundManager> sound) {

	_pSoundManeger = sound;

	if (_pSoundManeger == nullptr) {
		return false;
	}

	cg[ECG_CHIP_BLACK] = ResourceServer::LoadGraph("png/ActionUI/stage/chip_black.png");
	cg[ECG_CHIP_WHITE] = ResourceServer::LoadGraph("png/ActionUI/stage/chip_white.png");
	cg[ECG_CHIP_BROWN] = ResourceServer::LoadGraph("png/ActionUI/stage/chip_brown.png");
	cg[ECG_CHIP_RED] = ResourceServer::LoadGraph("png/ActionUI/stage/chip_red.png");
	cg[ECG_CHIP_BLUE] = ResourceServer::LoadGraph("png/ActionUI/stage/chip_blue.png");
	cg[ECG_CHIP_GREEN] = ResourceServer::LoadGraph("png/ActionUI/stage/chip_green.png");
	cg[ECG_STAR] = ResourceServer::LoadGraph("png/ActionUI/stage/star.png");
	cg[ECG_HEART] = ResourceServer::LoadGraph("png/ActionUI/stage/heart.png");

	StageInit();  	   // ステージ初期化
	SearchNoPassage(); // 行き止まりを探す

	// プレイヤー位置を、上端（のひとつ内側）から選択
	while (1) {
		plx = ((rand() % MAZE_W / 2)) * 2 + 1;
		ply = 1;
		if (maze[ply * MAZE_W + plx] == 0) {
			break;
		}
	}

	_pKeyInput.reset(new Input);

	//==============================================
	//宝箱 設置する場所
	//(プレイヤーの二個隣)  (行き止まりに設置する)
	_pStrongBox.reset(new StrongBox);
	_pStrongBox->Init(_pSoundManeger, { BLOCK_SIZE * plx + 180, 0.0f, BLOCK_SIZE * -ply });


	/*
	auto hoge = _noPassageList[0];

	auto x = std::get<0>(hoge);
	auto y = std::get<1>(hoge);
	*/
	//_pStrongBox->Init(_pSoundManeger, { BLOCK_SIZE * plx + 180, 0.0f, BLOCK_SIZE * -ply });



	//==============================================
	//ゴールにドアモデルを設置
	_pDoor.reset(new ModelAnimation);
	_pDoor->Load("model/Action3D/doorAnime.mv1");

	if (_pDoor->GetHandle() == -1) {
		return false;
	}

	_pDoor->AnimationPushBack(0, 1, 0.0f, 3.0f);
	_pDoor->AnimationPushBack(0, 2, 0.0f, 3.0f);

	// ゴール位置を、下端から選択
	glx = ((rand() % (MAZE_W - 1) / 2)) * 2 + 1;
	gly = MAZE_H - 1;
	maze[gly * MAZE_W + glx] = 0;	// 穴を開ける

	//穴を空けた場所の座標を3D座標に変換
	_pDoor->GetTransform().SetPosition({ BLOCK_SIZE * glx, 0.0f, BLOCK_SIZE * -gly });
	_pDoor->GetTransform().SetScale({ 0.9f, 0.65f, 0.0f });

	//==========================
	//エフェクト

	if (Effekseer_Init(8000) == -1) {
		return false;
	}

	_effectLoadHandle = LoadEffekseerEffect("effect/pink.efk", 0.3f);

	if (_effectLoadHandle == -1) {
		return false;
	}

	//エフェクトの座標一旦保留

	SetUseLighting(FALSE);

	return true;
}

void MazeStage::StageInit() {
	// 迷路を生成する
	int x, y;

	// 全部を壁で埋める
	for (y = 0; y < MAZE_H; y++) {
		for (x = 0; x < MAZE_W; x++) {
			maze[y * MAZE_W + x] = 1;
		}
	}

	// startposを決定する。必ず外周より。
	while (1) {
		x = (rand() % (MAZE_W / 2)) * 2 + 1;
		y = (rand() % (MAZE_H / 2)) * 2 + 1;
		if (x == 1 || x == MAZE_W - 2 || y == 1 || y == MAZE_H - 2) { break; }
	}

	// 穴堀スタート
	while (1) {
		int f = 1;
		int vtbl[][2] = {
			{ 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 },
		};
		while (1) {
			int v, nx, ny, i;
			// 乱数で決定した方向に歩いていく。壁の外に出るなら方向決定しなおし
			v = rand() % 4;
			nx = x + vtbl[v][0] * 2;
			ny = y + vtbl[v][1] * 2;
			if (nx < 0 || nx >= MAZE_W || ny < 0 || ny >= MAZE_H) {
				continue;
			}
			// 決めた場所はすでに穴が掘られているか？
			if (maze[ny * MAZE_W + nx] == 0) {
				// 掘られているので終了
				break;
			}
			// 決めた場所と今の場所の間を埋める
			maze[y * MAZE_W + x] = 0;
			for (i = 0; i < 2; i++) {
				x += vtbl[v][0];
				y += vtbl[v][1];
				maze[y * MAZE_W + x] = 0;
			}

			// デバッグ用描画
			// ここを入れると、マップを作っていくさまが見える
		//	GameDraw();

		}
		// 全箇所穴を掘ったか？
		for (y = 1; y < MAZE_H; y += 2) {
			for (x = 1; x < MAZE_W; x += 2) {
				if (maze[y * MAZE_W + x] != 0) {
					f = 0;
				}
			}
		}
		if (f) {
			// 全箇所掘られているので終了
			break;
		}
		// 次のスタート地点を、掘られている場所から選ぶ
		while (1) {
			x = (rand() % (MAZE_W / 2)) * 2 + 1;
			y = (rand() % (MAZE_H / 2)) * 2 + 1;
			if (maze[y * MAZE_W + x] == 0) {
				break;
			}
		}
	}
}

void MazeStage::SearchNoPassage() {
	// 行き止まりを探す
	auto yMax = MAZE_H - 2;
	auto xMax = MAZE_W - 2;
	constexpr auto checkNum = 9;
	std::array<char, checkNum> check = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	constexpr auto typeMax = static_cast<int>(NoPassageType::MAX);

	std::array<std::array<char, checkNum>, typeMax> checkType = {
		std::array<char, checkNum>{
			1, 0, 1,
			1, 0, 1,
			1, 1, 1
		},
		std::array<char, checkNum>{
			1, 1, 1,
			1, 0, 0,
			1, 1, 1
		},
		std::array<char, checkNum>{
			1, 1, 1,
			1, 0, 1,
			1, 0, 1
		},
		std::array<char, checkNum>{
			1, 1, 1,
			0, 0, 1,
			1, 1, 1
		}
	};

	for (auto y = 0; y < yMax; y++) {
		for (auto x = 0; x < xMax; x++) {
			check[0] = maze[y * MAZE_W + x];
			check[1] = maze[y * MAZE_W + x + 1];
			check[2] = maze[y * MAZE_W + x + 2];

			check[3] = maze[(y + 1) * MAZE_W + x];
			check[4] = maze[(y + 1) * MAZE_W + x + 1];
			check[5] = maze[(y + 1) * MAZE_W + x + 2];

			check[6] = maze[(y + 2) * MAZE_W + x];
			check[7] = maze[(y + 2) * MAZE_W + x + 1];
			check[8] = maze[(y + 2) * MAZE_W + x + 2];

			for (auto i = 0; i < typeMax; ++i) {
				auto hit = true;

				// 全てのチェックが OK なら行き止まり
				for (auto j = 0; j < checkNum; ++j) {
					if (check[j] != checkType[i][j]) {
						hit = false;
						break;
					}
				}

				if (hit) {  //一度もヒットしてない
					// 行き止まりの位置とタイプ
					auto noPassage = std::make_tuple(x + 1, y + 1, static_cast<NoPassageType>(i));

					_noPassageList.emplace_back(noPassage);

					break;
				}
			}
		}
	}
}

bool MazeStage::Process() {

	int nModeCnt = _modeCount;  //ゲームクラスから取ってくる
	int f = 0;	// ルート更新時1にする

	if (nModeCnt == 0) {
		f = 1;

		// 3Dモード用設定

		// 1フレームの移動力
		playerSpeed = BLOCK_SIZE / MOVE_BLOCK_SPEED;
		// 向いている方向は2Dで上（3Dで奥）で初期化する
		vDir.x = 0.f;
		vDir.y = 0.f;
		vDir.z = 1.f * playerSpeed;
	}

	if (!_is3D) {
		// 2D
		// Rを押しながらでない場合、主人公の移動
		if (!(_pKeyInput->_key[(KEY_INPUT_R)] == 1)) {

			// 主人公の移動
			if (_pKeyInput->_key[(KEY_INPUT_LEFT)] == 1) { plx--; f = 1; }
			if (_pKeyInput->_key[(KEY_INPUT_RIGHT)] == 1) { plx++; f = 1; }
			if (_pKeyInput->_key[(KEY_INPUT_UP)] == 1) { ply--; f = 1; }
			if (_pKeyInput->_key[(KEY_INPUT_DOWN)] == 1) { ply++; f = 1; }

			// 主人公の移動幅をマップ内に制限
			if (plx < 0) { plx = 0; }
			if (plx >= MAZE_W) { plx = MAZE_W - 1; }
			if (ply < 0) { ply = 0; }
			if (ply >= MAZE_H) { ply = MAZE_H - 1; }

			// スペースを押したらマップの壁をON/OFF
			if (_pKeyInput->_key[(KEY_INPUT_SPACE)] == 1) {
				maze[ply * MAZE_W + plx] = 1 - maze[ply * MAZE_W + plx];
				f = 1;
			}
		}

		// Rを押しながら上下左右でゴールの移動
		if (_pKeyInput->_key[(KEY_INPUT_R)] == 1) {
			// ゴールの移動
			if (_pKeyInput->_key[(KEY_INPUT_LEFT)] == 1) { glx--; f = 1; }
			if (_pKeyInput->_key[(KEY_INPUT_RIGHT)] == 1) { glx++; f = 1; }
			if (_pKeyInput->_key[(KEY_INPUT_UP)] == 1) { gly--; f = 1; }
			if (_pKeyInput->_key[(KEY_INPUT_DOWN)] == 1) { gly++; f = 1; }

			// ゴールの移動幅をマップ内に制限
			if (glx < 0) { glx = 0; }
			if (glx >= MAZE_W) { glx = MAZE_W - 1; }
			if (gly < 0) { gly = 0; }
			if (gly >= MAZE_H) { gly = MAZE_H - 1; }

			// スペースを押したらマップの壁をON/OFF
			if (_pKeyInput->_key[(KEY_INPUT_SPACE)] == 1) {
				maze[gly * MAZE_W + glx] = 1 - maze[gly * MAZE_W + glx];
				f = 1;
			}
		}

		// Z,Xを押したら、ルートサーチカウンタを変更する
		//if (key & PAD_INPUT_1) { route_search--; f = 1; }
		//if (key & PAD_INPUT_2) { route_search++; f = 1; }

		// A,Sを押したら、ルートサーチカウンタを大きく変更する
//		if (key & PAD_INPUT_4) { route_search -= 10; f = 1; }
//		if (key & PAD_INPUT_5) { route_search += 10; f = 1; }

		// A,Sを押したら、ゲームモードを3Dに変更
		if (_pKeyInput->_key[(KEY_INPUT_A)] == 1) {
			_is3D = true;
			// 今のプレイヤー座標を3D用座標に変換する
			_player3DPosi.x = BLOCK_SIZE * plx;
			_player3DPosi.y = 0.f;
			_player3DPosi.z = BLOCK_SIZE * -ply;		// 左手座標だと、2DのYが大きくなるほど、3DのZは手前になる

			_lastPlayer3DPosi = _player3DPosi;
		}

		// ルートサーチカウンタを一定内に制限
		if (route_search < 1) { route_search = 1; }
		if (route_search > 255) { route_search = 255; }

		// Qを押したら、ルートカウンタデバッグ表示
		//if (trg & PAD_INPUT_7) { draw_checkroute = 1; draw_route_c = 1; f = 1; }

		// Dを押したら、ルートカウンタデバッグ表示を変更
		//if (trg & PAD_INPUT_6) { draw_route_c = 1 - draw_route_c; }

		// Wを押したら、ルート生成デバッグ表示
		//if (trg & PAD_INPUT_8) { draw_makeroute = 1; draw_route_f = 1; f = 1; }

		// Cを押したら、ルートフラグデバッグ表示を変更
		//if (trg & PAD_INPUT_3) { draw_route_f = 1 - draw_route_f; }
	}
	else {
		// 3D
		// A,Sを押したら、ゲームモードを3Dに変更
		//if (trg & (PAD_INPUT_4 | PAD_INPUT_5)) {
		if (_pKeyInput->_key[(KEY_INPUT_A)] == 1) {
			_is3D = false;
			// 今のプレイヤー座標を2D用座標に変換する
			plx = _player3DPosi.x / BLOCK_SIZE;
			ply = _player3DPosi.z / -BLOCK_SIZE;
		}

		// 向いている方向のx,zから現在の角度を計算
		float rad = atan2(vDir.z, vDir.x);

		// 左右で角度変更
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) { rad += 1.f / MOVE_BLOCK_SPEED; }
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) { rad -= 1.f / MOVE_BLOCK_SPEED; }

		// 上下で向いている方向に移動
		float fMove = 0.f;		// 0だと移動しない

		if (CheckHitKey(KEY_INPUT_UP) == 1) { fMove = 1.f; }
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) { fMove = -1.f; }

		// 移動倍率をかけて、向いている方向に移動する
		_player3DPosi.x += vDir.x * fMove;
		_player3DPosi.y += vDir.y * fMove;
		_player3DPosi.z += vDir.z * fMove;

		// 角度から向いている方向を計算
		vDir.x = cos(rad) * playerSpeed;
		vDir.z = sin(rad) * playerSpeed;

		if (!CheckPosition(_player3DPosi)) {
			_player3DPosi = _lastPlayer3DPosi;
			vDir = _lastDir;
		}

		_lastPlayer3DPosi = _player3DPosi;
		_lastDir = vDir;
	}

	// 移動したならデータ更新
	if (f) {
		// GetNowTime() で、かかった時間を計測する
		int n = GetNowCount(0);

		search_call_cnt = 0;		// _CheckChipRoute() が再起で呼ばれた回数をカウント
		{
			CheckChipRoute(plx, ply, glx, gly, route_search);
			MakeShortRoute(plx, ply, glx, gly, route_search);
		}
		search_time_ms = GetNowCount(0) - n;
	}

	// デバッグ表示フラグOFF
	draw_checkroute = 0;
	draw_makeroute = 0;

	_pKeyInput->Process();
	_pDoor->Process();
	_pStrongBox->Process();

	return true;
}

bool MazeStage::Render() {

	GameDraw();  //door true

	return true;
}

bool MazeStage::Terminate() {
	return true;
}

void MazeStage::_CheckChipRoute(int x1, int y1, int x2, int y2, int cntmax) {

	// 枠外なら戻る
	if (x1 < 0 || x1 >= MAZE_W || y1 < 0 || y1 >= MAZE_H) { return; }

	// デバッグ用。再起で何回呼ばれたかカウント
	search_call_cnt++;

	// その位置は移動可能か？
	int mvFlg = maze[(y1)*MAZE_W + (x1)];
	if (mvFlg) {
		// 移動不可
		return;
	}
	// 移動可能。歩数を書き込むが、すでに書き込んでいるか？
	if (mazeroute_c[(y1)*MAZE_W + (x1)] != 0 && mazeroute_c[(y1)*MAZE_W + (x1)] >= cntmax) {
		// すでに書き込まれてて、今より歩数残りが大きいので、チェックする意味が無い
		return;
	}
	mazeroute_c[(y1)*MAZE_W + (x1)] = cntmax;
	cntmax--;

	// デバッグ用途中表示
	if (draw_checkroute) { GameDraw(); }

	// 歩数が残っているか？
	if (cntmax <= 0) {
		// 残っていないのでここまで
		return;
	}
	// 目的値に到着か？
	if (x1 == x2 && y1 == y2) {
		// 到着したのでここまで
		return;
	}
	// 上下左右に移動
	_CheckChipRoute(x1, y1 - 1, x2, y2, cntmax);		// 上
	_CheckChipRoute(x1 + 1, y1, x2, y2, cntmax);		// 右
	_CheckChipRoute(x1, y1 + 1, x2, y2, cntmax);		// 下
	_CheckChipRoute(x1 - 1, y1, x2, y2, cntmax);		// 左

	return;
}

// (x1,y1):現在地, (x2,y2):目的地
int MazeStage::CheckChipRoute(int x1, int y1, int x2, int y2, int cntmax) {
	// ルートマップ初期化
	memset(mazeroute_c, 0, sizeof(mazeroute_c));
	// ルートマップ作成
	_CheckChipRoute(x1, y1, x2, y2, cntmax);
	// たどり着けたか？
	if (mazeroute_c[(y2)*MAZE_W + (x2)] == 0) {
		// たどりつけなかった
		return 0;
	}
	// たどりつけた。歩数を返す
	return (cntmax - mazeroute_c[(y2)*MAZE_W + (x2)]) + 1;
}

// mazeroute_c[]を作った後、目的地への最短ルートを得る
// (x1,y1):現在地, (x2,y2):目的地
// 目的地から逆算していく
int MazeStage::MakeShortRoute(int x1, int y1, int x2, int y2, int cntmax) {
	// ルートフラグ初期化
	memset(mazeroute_f, 0, sizeof(mazeroute_f));
	// 本当にいけるの？
	int x, y;
	x = x2; y = y2;
	int n = mazeroute_c[(y)*MAZE_W + (x)];
	if (n == 0) {
		// 行けない
		return 0;
	}
	// 今の歩数値からルート作成
	while (x1 != x || y1 != y) {
		// 上右下左の順
		mazeroute_f[(y)*MAZE_W + (x)] = 1;
		if (n + 1 == mazeroute_c[(y - 1) * MAZE_W + (x)]) { x += 0; y += -1; n++; }
		else if (n + 1 == mazeroute_c[(y)*MAZE_W + (x + 1)]) { x += 1; y += 0; n++; }
		else if (n + 1 == mazeroute_c[(y + 1) * MAZE_W + (x)]) { x += 0; y += 1; n++; }
		else if (n + 1 == mazeroute_c[(y)*MAZE_W + (x - 1)]) { x += -1; y += 0; n++; }

		// デバッグ用途中表示
		if (draw_makeroute) { GameDraw(); }

	}
	mazeroute_f[(y)*MAZE_W + (x)] = 1;
	return (cntmax - mazeroute_c[(y2)*MAZE_W + (x2)]) + 1;
}

void MazeStage::GameDraw() {

	int x, y;

	// 画面クリア
	ClearDrawScreen();

	// 描画速度計測用
	unsigned int tStart = GetNowCount();

	if (!_is3D) {
		// 2D

		// 迷路
		for (y = 0; y < MAZE_H; y++) {
			for (x = 0; x < MAZE_W; x++) {
				if (maze[y * MAZE_W + x] != 0) {
					DrawGraph(x * CHIP_W, y * CHIP_H, cg[ECG_CHIP_BROWN], FALSE);
				}
				else {
					DrawGraph(x * CHIP_W, y * CHIP_H, cg[ECG_CHIP_BLACK], FALSE);
				}
			}
		}

		// カウント
		SetFontSize(16);
		for (y = 0; y < MAZE_H; y++) {
			for (x = 0; x < MAZE_W; x++) {
				if (draw_route_f) {
					if (mazeroute_f[y * MAZE_W + x] != 0) {
						DrawGraph(x * CHIP_W, y * CHIP_H, cg[ECG_CHIP_BLUE], FALSE);
					}
				}
				if (draw_route_c) {
					DrawFormatString(x * CHIP_W, y * CHIP_H, GetColor(128, 128, 128), "%2x", mazeroute_c[y * MAZE_W + x]);
				}
			}
		}

		DrawGraph(glx * CHIP_W, gly * CHIP_H, cg[ECG_HEART], TRUE);  // 2Dゴール
		DrawGraph(plx * CHIP_W, ply * CHIP_H, cg[ECG_STAR], TRUE);	// 2Dプレイヤー


		for (int i = 0; i < _noPassageList.size(); i++) {

			auto hoge = _noPassageList[i];

			auto x = std::get<0>(hoge);
			auto y = std::get<1>(hoge);

			DrawGraph(x * CHIP_W, y * CHIP_H, cg[ECG_CHIP_BLUE], TRUE);
		}

		// 情報表示
		DrawFormatString(0, 720 - 16, GetColor(255, 255, 255), "search_time: %d, search_call: %d", search_time_ms, search_call_cnt);
	}
	else {
		// 3D
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);
		SetUseBackCulling(TRUE);
		SetCameraNearFar(0.1f, 5000.0f);

		VECTOR CamPos;		// カメラの座標
		VECTOR CamTarg;		// カメラの注視点

		// カメラの座標をセット
		CamPos = _player3DPosi;
		CamPos.y = CAMERA_Y;	// カメラの高さに変更する

		// カメラの注視点をセット
		CamTarg = vDir;
		CamTarg = VAdd(CamPos, CamTarg);

		// カメラの位置と向きをセットする
		SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

		//====================================================================
		//エフェクト
		Effekseer_Sync3DSetting();   //エフェクト
				// 定期的にエフェクトを再生する
		if (_effectTime % 100 == 0) {
			// エフェクトを再生する。
			_effectPlayHandle = PlayEffekseer3DEffect(_effectLoadHandle);
			_effectPlayHandle2 = PlayEffekseer3DEffect(_effectLoadHandle);

			// エフェクトの位置をリセットする。
			_attack3DPosi = _player3DPosi;
		}
		//場所設定
		SetPosPlayingEffekseer3DEffect(_effectPlayHandle2, _player3DPosi.x + 200.0f, _player3DPosi.y, _player3DPosi.z);
		SetPosPlayingEffekseer3DEffect(_effectPlayHandle, _attack3DPosi.x, _attack3DPosi.y, _attack3DPosi.z);

		// 迷路
		// 先に最大値分の座標バッファ＆インデックスバッファを用意する
		VERTEX3D* vBuf = new VERTEX3D[MAZE_W * MAZE_H * 8];		// ただし、indexBufがunsigned shortなので、65535を超えないように
		int cntBuf = 0;
		unsigned short* indexBuf = new unsigned short[MAZE_W * MAZE_H * (6 * 4)];
		int cntIndexBuf = 0;

		// バッファを生成する
		for (y = 0; y < MAZE_H; y++) {
			for (x = 0; x < MAZE_W; x++) {
				if (maze[y * MAZE_W + x] != 0) {
					// 壁の場所に箱ポリゴンを表示する
					VERTEX3D v[8];	// 頂点データ(箱を表現するには8頂点必要)
					for (int i = 0; i < 8; i++) {
						float posTbl[][3] = {
							// (0,0,0)を箱の中心として考えて、各頂点がどこにあるか
							{-0.5f,  0.0f, -0.5f},	// 0:左下前
							{ 0.5f,  0.0f, -0.5f},	// 1:右下前
							{-0.5f,  0.0f,  0.5f},	// 2:左下奥
							{ 0.5f,  0.0f,  0.5f},	// 3:右下奥
							{-0.5f,  1.0f, -0.5f},	// 4:左上前
							{ 0.5f,  1.0f, -0.5f},	// 5:右上前
							{-0.5f,  1.0f,  0.5f},	// 6:左上奥
							{ 0.5f,  1.0f,  0.5f},	// 7:右上奥
						};
						v[i].pos = VGet(posTbl[i][0] * BLOCK_SIZE, posTbl[i][1] * BLOCK_SIZE, posTbl[i][2] * BLOCK_SIZE);
						VECTOR	vPos = { x * BLOCK_SIZE, 0, -y * BLOCK_SIZE };
						v[i].pos = VAdd(v[i].pos, vPos);
						v[i].norm = VGet(0.0f, 0.0f, -1.0f);
						v[i].dif = GetColorU8(255, 127 + (x % 2) * 127, 128 + (y % 2) * 127, 255);
						v[i].spc = GetColorU8(0, 0, 0, 0);
						v[i].u = 0.0f;
						v[i].v = 0.0f;
						v[i].su = 0.0f;
						v[i].sv = 0.0f;
					}
					unsigned short index[] = {	// 8つの頂点を使って、どう3角形ポリゴンを描画するか。6面分用意する
						// バックカリングをしているため、右回りを表面とする
						0,4,1,1,4,5,	// 前(0,1,4,5)
						0,2,4,4,2,6,	// 左(0,2,4,6)
						1,5,3,3,5,7,	// 右(1,3,5,7)
					//	0,2,1,1,2,3,	// 下(0,1,2,3)	// 上と下は迷路ゲームには必要無い
					//	4,6,5,5,6,7,	// 上(4,5,6,7)	//
						2,3,6,6,3,7,	// 奥(2,3,6,7)
					};
					int vNum = sizeof(v) / sizeof(v[0]);
					int pNum = sizeof(index) / sizeof(index[0]) / 3;
					//DrawPolygonIndexed3D(v, vNum, index, pNum, DX_NONE_GRAPH, FALSE);

					// バッファに書き込み
					for (int i = 0; i < pNum * 3; i++) {
						indexBuf[cntIndexBuf] = index[i] + cntBuf;		// 頂点番号をvBuf[]にあわせてずらす
						cntIndexBuf++;
					}
					for (int i = 0; i < vNum; i++) {
						vBuf[cntBuf] = v[i];
						cntBuf++;
					}

					VECTOR goal = { static_cast<int>(BLOCK_SIZE * glx), 0.0f, static_cast<int>(BLOCK_SIZE * -gly) };
					VECTOR t = VScale(vDir, 10.0f);  //方向に大きさをプラス
					VECTOR endPlayerPosi = VAdd(_player3DPosi, t);  //始点座標から


					VECTOR vertex1 = { goal.x - 40.0f,goal.y,goal.z };
					VECTOR vertex2 = { goal.x + 40.0f,goal.y,goal.z };
					VECTOR vertex3 = { goal.x,goal.y,goal.z + 40.0f };
					VECTOR vertex3Anime = { goal.x,goal.y,goal.z + 60.0f };

					//door三角形と線分の当たり判定
					_isDoorArea = HitCheck_Line_Triangle(_player3DPosi, endPlayerPosi, vertex1, vertex2, vertex3).HitFlag == 1;

					//doorAnime当たり判定
					if (HitCheck_Line_Triangle(_player3DPosi, endPlayerPosi, vertex1, vertex2, vertex3Anime).HitFlag) {
						_pDoor->PlayAnimation(0);
					}
					_pStrongBox->SetPlayerPosition(_player3DPosi, endPlayerPosi);
					//================
					//雷とプレイヤーの当たり判定

					if (_player3DPosi.x == _attack3DPosi.x) {
						_isHit = true;
					}
					else {
						_isHit = false;
					}


					//debug
					/*
					//ワイヤフレームの描画
					for (int i = 0; i < pNum; i++) {
						DrawLine3D(v[index[i * 3 + 0]].pos, v[index[i * 3 + 1]].pos, GetColor(255, 255, 255));
						DrawLine3D(v[index[i * 3 + 1]].pos, v[index[i * 3 + 2]].pos, GetColor(255, 255, 255));
						DrawLine3D(v[index[i * 3 + 2]].pos, v[index[i * 3 + 0]].pos, GetColor(255, 255, 255));
					}
					*/

					//プレイヤーから伸びた線
					//DrawLine3D(_player3DPosi, endPlayerPosi, GetColor(255, 30, 0));

					//ドアエリアの当たり判定描画(三角)
					//DrawTriangle3D(vertex1, vertex2, vertex3, GetColor(255, 255, 0), FALSE);

					//ドアアニメの当たり判定描画(三角)
					//DrawTriangle3D(vertex1, vertex2, vertex3Anime, GetColor(255, 30, 0), FALSE);
				}
			}
		}

		// バッファのデータをまとめて描画する
		DrawPolygonIndexed3D(vBuf, cntBuf, indexBuf, cntIndexBuf / 3, DX_NONE_GRAPH, FALSE);

		// バッファの解放
		delete[] vBuf;
		delete[] indexBuf;

		//=================================================
		//エフェクト描画

		UpdateEffekseer3D();  //エフェクトの更新
		DrawEffekseer3D();
		_effectTime++;

		_pDoor->Draw();
		_pStrongBox->Draw();
	}

	// 描画速度表示
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", GetNowCount() - tStart);
	//プレイヤーの座標
	//DrawFormatString(20, 50, GetColor(255, 255, 255), "プレイヤー(%.3f,0,%.3f)", _player3DPosi.x, _player3DPosi.z);
}

//3Dで壁が道が判定 trueだったら道
bool MazeStage::CheckPosition(VECTOR position) {

	//3Dの座標を2Dの配列と比べるmaze[y * MAZE_W + x] (3397)
	auto w = static_cast<unsigned int>(position.x / BLOCK_SIZE + 0.5f);
	auto h = static_cast<unsigned int>(-position.z / BLOCK_SIZE + 0.5f);

	return maze[h * MAZE_W + w] == 0;
}
