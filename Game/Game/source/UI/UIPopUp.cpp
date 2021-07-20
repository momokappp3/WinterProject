#include "UIPopUp.h"
#include "../../ResourceServer.h"
#include "../../Utility.h"

UIPopUp::UIPopUp() {

    _pPopUpBase = nullptr;
    _pOkBSelectBase = nullptr;
    _pCloselBBase = nullptr;
	_pSoundManager = nullptr;

	_ok = -1;
	_close = -1;

	_mouse = { 0,0 };

	_nowMode = false;

	_popString = { _T("初期化"),{0,0},false };
}

UIPopUp::~UIPopUp() {
}

bool UIPopUp::Init(std::shared_ptr<SoundManager>& soundManager, Point base) {

	if (soundManager != nullptr) {
		bool seTitle = soundManager->LoadSECommon();

		if (!seTitle) {
			return false;
		}
	}
	else {
		return false;
	}

	_pSoundManager = soundManager;

	_pPopUpBase.reset(new UI2DBase);
	_pOkBSelectBase.reset(new UI2DSelectBase);
	_pCloselBBase.reset(new UI2DSelectBase);

	//=================================================================
	//popupBase
	int handle = ResourceServer::LoadGraph("png/RoomUI/use/popUp.png");

	if (handle == -1) {
		return false;
	}

	//DrawInfo info = { handle,1130,330,true };
	DrawInfo info = { handle,base.x,base.y,true };
	_pPopUpBase->SetDrawInfo(info);


	//====================================================================
	//OKボタン
	handle = ResourceServer::LoadGraph("png/RoomUI/use/OkB.png");
	int handle2 = ResourceServer::LoadGraph("png/RoomUI/use/OkBSelect.png");

	if (handle == -1 && handle2 == -1) {
		return false;
	}

	info = { handle,1400,550,true };
	DrawInfo info2 = { handle2,1400,550,true };

	_pOkBSelectBase->SetDrawInfo(info);
	_pOkBSelectBase->SetDrawInfo(info2);

	_pOkBSelectBase->SetRect();

	//====================================================================
	//close

	handle = ResourceServer::LoadGraph("png/RoomUI/closeButton.png");
	handle2 = ResourceServer::LoadGraph("png/RoomUI/closeButton_select.png");

	if (handle == -1 && handle2 == -1) {
		return false;
	}

	info = { handle,1140,340,true };
	info2 = { handle2,1140,340,true };

	_pCloselBBase->SetDrawInfo(info);
	_pCloselBBase->SetDrawInfo(info2);

	_pCloselBBase->SetRect();

	//=======================================
	//sound

	auto onSelect = [this]() {
		//サウンド鳴らす
		_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Select);
	};

	_pCloselBBase->SetOnSelect(onSelect);
	_pOkBSelectBase->SetOnSelect(onSelect);

	return true;
}

void UIPopUp::Process() {

	_ok = _pOkBSelectBase->GetSelect();
	_pOkBSelectBase->SetSelect(Utility::ImageHitDetection(_mouse.x, _mouse.y, _pOkBSelectBase.get()));

	_close = _pCloselBBase->GetSelect();
	_pCloselBBase->SetSelect(Utility::ImageHitDetection(_mouse.x, _mouse.y, _pCloselBBase.get()));

	_pPopUpBase->Process();
	_pOkBSelectBase->Process();
	_pCloselBBase->Process();
	_pSoundManager->Process();
}

void UIPopUp::Draw() {

	if (_popString.isDraw && _nowMode) {

		_pPopUpBase->Draw();
		_pOkBSelectBase->Draw();
		_pCloselBBase->Draw();
		DrawFormatString(_popString.xy.x, _popString.xy.y, GetColor(255, 255, 255),
						 _popString.string.c_str());
	}
}