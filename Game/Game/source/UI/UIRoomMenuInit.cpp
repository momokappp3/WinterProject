#include "UIRoomMenuInit.h"
#include "../../ResourceServer.h"
#include "../../Utility.h"

UIRoomMenuInit::UIRoomMenuInit(){

	_ModeType = RoomGameUIType::Max;

	_pMenuSelectBase = nullptr;
	_pMenuInAndOut = nullptr;
	_pSoundManager = nullptr;

	_start = false;
	_end = false;

	_mouseX = -1;
	_mouseY = -1;

	_num = 0;
}

UIRoomMenuInit::~UIRoomMenuInit() {
}

bool UIRoomMenuInit::Init(std::shared_ptr<SoundManager>& soundManager) {

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

	_ModeType = RoomGameUIType::MenuIinit;

	_pMenuSelectBase.reset(new UI2DSelectBase);
	_pMenuInAndOut.reset(new UIInAndOut);

	_pMenuInAndOut->Init();

	int handle = ResourceServer::LoadGraph("png/RoomUI/menu.png");
	int handle2 = ResourceServer::LoadGraph("png/RoomUI/menu_select.png");


	if (handle == -1 && handle2 == -1) { 
		return false;
	}

	DrawInfo info = { handle,1800,20,true,1980,20 };
	DrawInfo info2 = { handle2,1800,20,true ,1980,20 };

	_pMenuSelectBase->SetDrawInfo(info);
	_pMenuSelectBase->SetDrawInfo(info2);

	//================================================
	//InAndOut動きの処理

	_pMenuSelectBase->SetRadius(50.0f);  //typeをサークルにしている

	DrawInfo infoInit;
	_pMenuSelectBase->GetDrawInfo(1,infoInit);

	_pMenuInAndOut->SetStartPosition(infoInit.startXY);
	_pMenuInAndOut->SetEndPosition(infoInit.xy);
	_pMenuInAndOut->SetNowPosition(infoInit.xy);

	//=====================================================
	//音の処理

	auto onSelect = [this]() {
		//サウンド鳴らす
		_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Select);
	};

	_pMenuSelectBase->SetOnSelect(onSelect);

	return true;
}

void UIRoomMenuInit::Process() {

	_select = _pMenuSelectBase->GetSelect();

	_pMenuSelectBase->SetPoint(0,_pMenuInAndOut->GetNowPosition());
	_pMenuSelectBase->SetPoint(1, _pMenuInAndOut->GetNowPosition());

	_pMenuInAndOut->SetIsEnd(_end);
	_pMenuInAndOut->SetIsStart(_start);

	_end = false;
	_start = false;

	//_pMenuSelectBase.get() スマートポインタから普通のポインタに
	_pMenuSelectBase->SetSelect(Utility::ImageHitDetection(_mouseX, _mouseY, _pMenuSelectBase.get()));

	_pMenuInAndOut->Process();
	_pMenuSelectBase->Process();
	_pSoundManager->Process();

}

void UIRoomMenuInit::Draw() {

	_pMenuSelectBase->Draw();
}