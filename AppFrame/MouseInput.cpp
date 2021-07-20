#include "MouseInput.h"
#include "DxLib.h"
#include "../Game/Game/ResourceServer.h"

MouseInput::MouseInput() {

	SetMouseDispFlag(TRUE);

	_startPosition = { 0,0,0 };
	_endPosition = { 0,0,0 };

	_isRight = false;
	_isLeft = false;

	_isTrgLeft = false;

	_handle = ResourceServer::LoadGraph("png/arrow_cursor.png");

	_posX = 0;
	_posY = 0;

	_lastLeft = 0;
}

MouseInput::~MouseInput() {
}

void MouseInput::Process(){

	//_oldLeft = _isLeft;

	//マウスポインタの座標を取得
	GetMousePoint(&_posX, &_posY);

	// マウスポインタがある画面上の座標に該当する３Ｄ空間上の Near 面の座標を取得
	_startPosition = ConvScreenPosToWorldPos(VGet(_posX, _posY, 0.0f));

	// マウスポインタがある画面上の座標に該当する３Ｄ空間上の Far 面の座標を取得
	_endPosition = ConvScreenPosToWorldPos(VGet(_posX, _posY, 1.0f));


	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		_isRight = true;
	}

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		_isLeft = true;
	}

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && _lastLeft == 0) {
		_isTrgLeft = true;
	}
	else {
		_isTrgLeft = false;
	}

	_lastLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
}

void MouseInput::LastProcess() {


}

void MouseInput::Draw() {

	DrawGraph(_posX, _posY,_handle, TRUE);

	DrawFormatString(20, 500, GetColor(255, 255, 255), "マウスの座標( %d, %d )", _posX, _posY);

	if (_isRight) {
		//DrawString(20, 580, "マウス右ボタンが押されています", GetColor(255, 165, 0));
	}

	if (_isLeft) {
		//DrawString(20, 550, "マウス左ボタンが押されています", GetColor(255, 165, 0));
	}

	_isRight = false;
	_isLeft = false;

}

void MouseInput::TitleDraw() {

	DrawFormatString(20, 500, GetColor(255, 255, 255), "マウスの座標( %d, %d )", _posX, _posY);

	if (_isRight) {
		//DrawString(20, 580, "マウス右ボタンが押されています", GetColor(255, 165, 0));
	}

	if (_isLeft) {
		//DrawString(20, 550, "マウス左ボタンが押されています", GetColor(255, 165, 0));
	}

	_isRight = false;
	_isLeft = false;
}