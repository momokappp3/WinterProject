#include "Effect3D.h"
#include "EffekseerForDXLib.h"
#include "../AppFrame/source/Mode/ModeBase.h"
#include "../AppFrame/source/Mode/ModeServer.h"


/*
Effect3D::Effect3D() {

	//_point = { 0,-150 };

	_effectResourceHandle = -1;
	_playingEffectHandle = -1;

	//_isLoop = false;
	//_isEnd = false;
}

Effect3D::~Effect3D() {
}

/*
//ハンドルと拡大率をセット
bool Effect3D::Init(const TCHAR* fileName, float magnification, bool isLoop) {

	_effectResourceHandle = LoadEffekseerEffect(fileName, magnification);

	if (_effectResourceHandle == -1) {
		return false;
	}

	_isLoop = isLoop;

	return true;
}

void Effect3D::Start() {

	if (_effectResourceHandle == -1) {
		return;
	}

	// エフェクトを再生する。
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	DrawEffekseer3D_Begin();

}

void Effect3D::Stop() {

	if (_playingEffectHandle == -1) {
		return;
	}

	StopEffekseer3DEffect(_playingEffectHandle);
	DrawEffekseer3D_End();

	_playingEffectHandle = -1;
}

void Effect3D::Process() {

	if (_playingEffectHandle == -1) {
		return;
	}

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	ProcessPoint();

	// 再生中のエフェクトを移動する。
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, _point.x, _point.y, 0);

	if (_isLoop) {
		//再生中か
		if (IsEffekseer3DEffectPlaying(_playingEffectHandle) == -1) {
			_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
			DrawEffekseer3D_Begin();
		}
	}
	else {
		StopEffekseer3DEffect(_playingEffectHandle);
		DrawEffekseer3D_End();
		// 3Dを表示する。
		DrawCapsule3D(
			VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);
	}

}

void Effect3D::ProcessPoint() {
}

void Effect3D::Delete() {

	Stop();

	if (_effectResourceHandle != -1) {
		DeleteEffekseerEffect(_effectResourceHandle);
	}
}

bool Effect3D::Init(const TCHAR* fileName, float magnification) {

	_effectResourceHandle = LoadEffekseerEffect("fileName", magnification);

	return true;
}

bool Effect3D::Terminate() {

	// エフェクトリソースを削除する。
	DeleteEffekseerEffect(_effectResourceHandle);
	return true;
}

bool Effect3D::Process() {

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	// モードカウンタを使って60fpsでエフェクトを生成
	if (ModeBase::GetModeCount() % 60 == 0) {
		// エフェクトを再生する。
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);

		// エフェクトの位置をリセットする。
		_position_x = 0.0f;
	}

	// 再生中のエフェクトを移動する。
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, _position_x, 50.f, 0);
	_position_x += 0.2f;

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	return true;
}


void Effect3D::Draw() {
	Effekseer_Sync3DSetting();  // DXライブラリのカメラとEffekseerのカメラを同期する
	DrawEffekseer3D();  // Effekseerにより再生中のエフェクトを描画する。
}

*/