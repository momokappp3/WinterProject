#include "UIRoomSetting.h"
#include "../../ResourceServer.h"
#include "../../Utility.h"

UIRoomSetting::UIRoomSetting() {

	_pTween = nullptr;

    _pBackImageBase = nullptr;
    _pOutBSelectBase = nullptr;
    _pTitleBSelectBase = nullptr;
	_pSoundManager = nullptr;

}

UIRoomSetting::~UIRoomSetting() {
}

bool UIRoomSetting::Init(std::shared_ptr<SoundManager>& soundManager) {

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

    _pBackImageBase.reset(new UI2DBase);
    _pOutBSelectBase.reset(new UI2DSelectBase);
    _pTitleBSelectBase.reset(new UI2DSelectBase);
	_pTween.reset(new Tween);

	//コマンドvectorに使う分だけfalseをpushBack
	_vComandSelect.push_back(false);
	_vComandSelect.push_back(false);

	_closePoint = {1780,460};
	UIUseBase::Init();

	int handle = ResourceServer::LoadGraph("png/RoomUI/use/useSettingBase.png");

	if (handle == -1) {
		return false;
	}

	DrawInfo info = { handle,1775,95,true};
	_pBackImageBase->SetDrawInfo(info);

	//====================================================================
	//外出ボタン
	handle = ResourceServer::LoadGraph("png/RoomUI/use/useBOut.png");
	int handle2 = ResourceServer::LoadGraph("png/RoomUI/use/useBOutSelect.png");

	if (handle == -1 && handle2 == -1) {
		return false;
	}

	info = { handle,1780,145,true};
	DrawInfo info2 = { handle2,1780,145,true };

	_pOutBSelectBase->SetDrawInfo(info);
	_pOutBSelectBase->SetDrawInfo(info2);

	_pOutBSelectBase->SetRect();

	//=============================================================
	//Titleボタン
	handle = ResourceServer::LoadGraph("png/RoomUI/use/useBTitle.png");
	handle2 = ResourceServer::LoadGraph("png/RoomUI/use/useBTitleSelect.png");

	if (handle == -1 && handle2 == -1) {
		return false;
	}

	info = { handle,1780,245,true };
	info2 = { handle2,1780,245,true };

	_pTitleBSelectBase->SetDrawInfo(info);
	_pTitleBSelectBase->SetDrawInfo(info2);
	
	_pTitleBSelectBase->SetRect();

	//===================================
	//sound
	auto onSelect = [this]() {
		//サウンド鳴らす
		_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Select);
	};

	_pTitleBSelectBase->SetOnSelect(onSelect);
	_pOutBSelectBase->SetOnSelect(onSelect);
	_pCloselBBase->SetOnSelect(onSelect);

	return true;
}

void UIRoomSetting::Process() {

	UIUseBase::Process();

	_vComandSelect[0] = _pOutBSelectBase->GetSelect();
	_pOutBSelectBase->SetSelect(Utility::ImageHitDetection(_mouseX, _mouseY, _pOutBSelectBase.get()));

	_vComandSelect[1] =_pTitleBSelectBase->GetSelect();
	_pTitleBSelectBase->SetSelect(Utility::ImageHitDetection(_mouseX, _mouseY, _pTitleBSelectBase.get()));

	_pBackImageBase->Process();
	_pOutBSelectBase->Process();
	_pTitleBSelectBase->Process();
	_pSoundManager->Process();
	UIUseBase::Process();
}

void UIRoomSetting::Draw() {

	if (_nowMode) {
		_pBackImageBase->Draw();
		_pOutBSelectBase->Draw();
		_pTitleBSelectBase->Draw();
		UIUseBase::Draw();
	}
}