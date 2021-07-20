#include "../../../ResourceServer.h"
#include "Dxlib.h"
#include "UIActionPopUp.h"

UIActionPopUp::UIActionPopUp() {

    _pPopUpBase = nullptr;
    _pOkBSelectBase = nullptr;

	_ok = false;
	_nowMode = false;
	_popString = { _T("初期化"),{0,0},false };
	_fontHandle = -1;
}

UIActionPopUp::~UIActionPopUp() {
}

bool UIActionPopUp::Init() {

	_pKeyInput.reset(new Input);

	//ModeGameのサウンドマネジャーを引き継ぎ
	/*
	if (soundManager != nullptr) {
		bool seTitle = soundManager->LoadSECommon();

		if (!seTitle) {
			return false;
		}
	}
	else {
		return false;
	}
	*/

	//================
	//文字列ハンドル
	_fontHandle = CreateFontToHandle("UD デジタル 教科書体 N-B", 40,1);

	_pPopUpBase.reset(new UI2DBase);
	_pOkBSelectBase.reset(new UI2DSelectBase);

	//=================================================================
	//popupBase
	int handle = ResourceServer::LoadGraph("png/RoomUI/use/popUp.png");

	if (handle == -1) {
		return false;
	}

	DrawInfo info = { handle,600,620,true };
	_pPopUpBase->SetDrawInfo(info);


	//====================================================================
	//OKボタン
	handle = ResourceServer::LoadGraph("png/RoomUI/use/OkB.png");
	int handle2 = ResourceServer::LoadGraph("png/RoomUI/use/OkBSelect.png");

	if (handle == -1 && handle2 == -1) {
		return false;
	}

	info = { handle,842,800,true };
	DrawInfo info2 = { handle2,842,800,true };

	_pOkBSelectBase->SetDrawInfo(info);
	_pOkBSelectBase->SetDrawInfo(info2);

	//_pOkBSelectBase->SetRect();

	//=======================================
	//sound

	auto onSelect = [this]() {
		//サウンド鳴らす
		//_pSoundManager->PlaySECommon(SoundManager::SECommon::Select);
	};

	_pOkBSelectBase->SetOnSelect(onSelect);

	return true;
}

void UIActionPopUp::Process() {

	if (_nowMode) {
		_ok = true;
		if (_pKeyInput->_key[(KEY_INPUT_RETURN) == 1]) {
			
		}
	}

	int select = _ok ? 1 : 0;

	_pOkBSelectBase->SetSelect(select);

	_pKeyInput->Process();
	_pPopUpBase->Process();
	_pOkBSelectBase->Process();
	//_pSoundManager->Process();
}

void UIActionPopUp::Draw() {

	if (_nowMode) {
		_pPopUpBase->Draw();
		_pOkBSelectBase->Draw();

		if (_popString.isDraw) {
			DrawStringToHandle(_popString.xy.x, _popString.xy.y, _popString.string.c_str(),GetColor(33,30,85),_fontHandle);
		}
	}
}