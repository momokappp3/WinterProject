#include "RoomGameUI.h"
#include "../ApplicationMain.h"
#include "../Mode/ModeTitle.h"
#include "../Mode/RoomGame.h"
#include "../../Utility.h"

const int DRUNK_TIME = 10;

RoomGameUI::RoomGameUI() {

    _type = RoomGameUIType::Max;

    _pUIRoomMenuInit = nullptr;
    _pUIRoomMenu = nullptr;
    _pUIRoomSetting = nullptr;
    _pUIRoomItem = nullptr;
    _pUIRoomStory = nullptr;
    _pSoundManager = nullptr;

    _pUIPopUp = nullptr;
    _pDrunkTime = nullptr;
    _pScriptEngin = nullptr;
    _pMouseInput = nullptr;

    _faceInfo = {0, 0.0, 0.0, 0, 0,0};

    _pCloselBScript = nullptr;
    _pUpButton = nullptr;
    _pDownButton = nullptr;

    _sakeItem = false;
    _goTitle = false;
    _goAction = false;

    _scriptClose = false;
    _upB = false;
    _downB = false;

    _okFlag = false;
    _isFaceInfo = false;

    _pPlayerInfo = nullptr;

    _giveCoin = 0;
}

RoomGameUI::~RoomGameUI() {
}

bool RoomGameUI::Init(std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<PlayerInfo>& playerInfo) {

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

    if (playerInfo == nullptr) {
        return false;
    }

    _pPlayerInfo = playerInfo;

    _type = RoomGameUIType::MenuIinit;

    _pUIRoomMenuInit.reset(new UIRoomMenuInit);
    _pUIRoomMenu.reset(new UIRoomMenu);
    _pUIRoomSetting.reset(new UIRoomSetting);
    _pUIRoomItem.reset(new UIRoomItem);
    _pUIRoomStory.reset(new UIRoomStory);

    _pUIPopUp.reset(new UIPopUp);
    _pDrunkTime.reset(new UITime);
    _pScriptEngin.reset(new amg::ScriptEngine);
    _pMouseInput.reset(new MouseInput);

    _pCloselBScript.reset(new UI2DSelectBase);
    _pUpButton.reset(new UI2DSelectBase);
    _pDownButton.reset(new UI2DSelectBase);

    if (!_pUIRoomMenu->Init(_pSoundManager, _pPlayerInfo) || !_pUIRoomMenuInit->Init(_pSoundManager) || !_pUIRoomItem->Init(_pSoundManager) ||
        !_pUIRoomSetting->Init(_pSoundManager) || !_pUIRoomStory->Init(_pSoundManager) || !_pUIPopUp->Init(_pSoundManager, {1130,330})) {
        return false;
    }

    //xボタン
    int handle = ResourceServer::LoadGraph("png/RoomUI/closeButton.png");
    int handle2 = ResourceServer::LoadGraph("png/RoomUI/closeButton_select.png");

    if (handle == -1 && handle2 == -1) {
        return false;
    }

    DrawInfo info = { handle,412,270,true };
    DrawInfo info2 = { handle2,412,270,true };

    _pCloselBScript->SetDrawInfo(info);
    _pCloselBScript->SetDrawInfo(info2);
    _pCloselBScript->SetRadius(20);

    //上矢印
    handle = ResourceServer::LoadGraph("png/RoomUI/use/arrowUp.png");
    handle2 = ResourceServer::LoadGraph("png/RoomUI/use/arrowUpSelect.png");

    if (handle == -1 && handle2 == -1) {
        return false;
    }

    info = { handle,1700,400,false};
    info2 = { handle2,1700,400,false };

    _pUpButton->SetDrawInfo(info);
    _pUpButton->SetDrawInfo(info2);
    _pUpButton->SetRect();

    //下矢印
    handle = ResourceServer::LoadGraph("png/RoomUI/use/arrowDown.png");
    handle2 = ResourceServer::LoadGraph("png/RoomUI/use/arrowDownSelect.png");

    if (handle == -1 && handle2 == -1) {
        return false;
    }

    info = { handle,1700,515,false };
    info2 = { handle2,1700,515,false };

    _pDownButton->SetDrawInfo(info);
    _pDownButton->SetDrawInfo(info2);
    _pDownButton->SetRect();


    if (!_pScriptEngin->Initialize("source/excel/test.json",_pPlayerInfo)) {
        return false;
    }

    if (!_pDrunkTime->Init(99, 2)) {
        return false;
    }

    _pScriptEngin->SetState(amg::ScriptEngine::ScriptState::END);

    _giveCoin = 2500;

    auto onSelect = [this]() {
        //サウンド鳴らす
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Select);
    };

    _pUpButton->SetOnSelect(onSelect);
    _pDownButton->SetOnSelect(onSelect);
    _pCloselBScript->SetOnSelect(onSelect);

    return true;
}

void RoomGameUI::Process() {

    _pMouseInput->Process();
    _pUIPopUp->Process();
    /*
    if (_pScriptEngin->IsFavor()) {
        
        int getNowFavor = 0;
        
        getNowFavor = _pScriptEngin->GetFavor();  //分子9900まで

        
        += getNowFavor / 100;
        _molecule = getNowFavor % 100;
        
    }
    
    if (_molecule >= 9900) {
        _molecule = 9900;
    }
    */

    //_pUIRoomMenu->SetFavor(_favor);
    //_pUIRoomMenu->SetMolecule(_pScriptEngin->IsFavor());  //???????
    /*
    if (_pScriptEngin->IsFavor()) {

        int getNowFavor = 0;

        getNowFavor = _pPlayerInfo->GetFavor();

        //_favor += getNowFavor;
        _pPlayerInfo->SetFavor(getNowFavor, true);
    }
    */

    if (_pScriptEngin->IsGetFace()) {
        _isFaceInfo = true;
        _faceInfo = _pScriptEngin->GetFace();
    }

    if (_type == RoomGameUIType::Setting) {
        SettingProcess();
    }

    if (_type == RoomGameUIType::Item) {
        ItemProcess();
    }

    if (_type == RoomGameUIType::Story) {
        StoryProcess();
    }

    if (_type == RoomGameUIType::MenuIinit) {
        _pUIRoomMenuInit->SetMousePoint(_pMouseInput->GetXNum(), _pMouseInput->GetYNum());
    }

    if (_type == RoomGameUIType::Menu && _type!=RoomGameUIType::Item && _type != RoomGameUIType::Story && 
        _type != RoomGameUIType::Item && _pScriptEngin->GetState() == amg::ScriptEngine::ScriptState::END) {

        _pUIRoomMenu->SetMousePoint(_pMouseInput->GetXNum(), _pMouseInput->GetYNum());
    }

    if (_type == RoomGameUIType::Setting && _type != RoomGameUIType::Item && _type != RoomGameUIType::Story &&
        _pMouseInput->GetYNum() && _pScriptEngin->GetState() == amg::ScriptEngine::ScriptState::END) {

        _pUIRoomSetting->SetMousePoint(_pMouseInput->GetXNum(), _pMouseInput->GetYNum());
        _pUIRoomMenu->SetSelectSetting(0);  //音が他のところをクリックしても鳴るため
    }

    if (_type == RoomGameUIType::Item && _type != RoomGameUIType::Story && _pScriptEngin->GetState() == amg::ScriptEngine::ScriptState::END) {

        _pUIRoomItem->SetMousePoint(_pMouseInput->GetXNum(), _pMouseInput->GetYNum());
        _pUIRoomItem->SetMouseLeft(_pMouseInput->GetLeft());
        _pUIRoomMenu->SetSelectItem(0);   //音が他のところをクリックしても鳴るため
    }

    if (_type == RoomGameUIType::Story && !_pUIPopUp->GetNowMode() &&
        _type != RoomGameUIType::Item && _pScriptEngin->GetState() == amg::ScriptEngine::ScriptState::END) {

        _pUIRoomStory->SetMousePoint(_pMouseInput->GetXNum(), _pMouseInput->GetYNum());
        _pUIRoomMenu->SetSelectSetting(0);  //音が他のところをクリックしても鳴るため
    }

    _pUIPopUp->SetMouse(_pMouseInput->GetXNum(), _pMouseInput->GetYNum());
    _pUIPopUp->SetTrgLeft(_pMouseInput->GetTrgLeft());

    //menuを押したとき処理
    if (_pUIRoomMenuInit->GetSelect() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomMenuInit->SetEnd(true);  //endの座標まで持っていく
        _pUIRoomMenu->SetStart(true);  //startの座標までもっていく
        _type = RoomGameUIType::Menu;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //cancelを押したとき処理
    if (_pUIRoomMenu->GetSelectCancel() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomMenu->SetEnd(true);  //外の座標までもっていく
        _pUIRoomMenuInit->SetStart(true);  //中の座標まで持っていく
        _type = RoomGameUIType::MenuIinit;

        _pUIRoomStory->SetAddImageSize(1);
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
    }

    _pUIRoomSetting->Process();
    //settingを押したとき処理
    if (_pUIRoomMenu->GetSelectSetting() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomSetting->SetStart(true);  //設定を開く
        _pUIRoomSetting->SetNowMode(true);
        _type = RoomGameUIType::Setting;
         _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    _pUIRoomItem->Process();
    //itemを押したとき処理
    if (_pUIRoomMenu->GetSelectItem() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomItem->SetStart(true);  //設定を開く
        _pUIRoomItem->SetNowMode(true);
        _type = RoomGameUIType::Item;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }
    
    //酒の下げる処理
    if (_sakeItem && _pDrunkTime->GetNowTime() == 1) {
        _sakeItem = false;
        _pPlayerInfo->SetMentalNum(30, false);
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::BarDown,100);
    }
    

    if (_pDrunkTime->GetEndNow()) {
        _sakeItem = false;
        _pPlayerInfo->SetMentalNum(30, false);
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::BarDown, 100);
    }

    _pUIRoomStory->Process();
    //storyを押したとき処理
    if (_pUIRoomMenu->GetSelectStory() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomStory->SetStart(true);  //設定を開く
        _pUIRoomStory->SetNowMode(true);
        _type = RoomGameUIType::Story;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //スクリプトエンジンxボタンの処理
    if (_pScriptEngin->GetState() != amg::ScriptEngine::ScriptState::END) {

        _scriptClose = _pCloselBScript->GetSelect();
        _pCloselBScript->SetSelect(Utility::ImageHitDetection(_pMouseInput->GetXNum(), _pMouseInput->GetYNum(), _pCloselBScript.get()));

        if (_scriptClose && !_pMouseInput->GetTrgLeft()) {
            _scriptClose = false;
        }

        if (_scriptClose) {
            _pScriptEngin->SetState(amg::ScriptEngine::ScriptState::END);
            _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
        }
    }

    if (_type == RoomGameUIType::Item && _pUIPopUp->GetNowMode() && _pUIRoomItem->GetNowItemType() == 0) {
        //上ボタンの処理
        _upB = _pUpButton->GetSelect();
        _pUpButton->SetSelect(Utility::ImageHitDetection(_pMouseInput->GetXNum(), _pMouseInput->GetYNum(), _pUpButton.get()));

        if (_pMouseInput->GetLeft() && _pUpButton->GetSelect() == 1) {
            if (_giveCoin >= 1000 && _giveCoin < 999999 ) {  //渡すお金をあげる
                _giveCoin += 10;
            }
        }

        //下ボタンの処理
        _downB = _pDownButton->GetSelect();
        _pDownButton->SetSelect(Utility::ImageHitDetection(_pMouseInput->GetXNum(), _pMouseInput->GetYNum(), _pDownButton.get()));

        if (_pDownButton->GetSelect() == 1 && _pMouseInput->GetLeft()) {
            if (_giveCoin < 999999 && _giveCoin >= 1000) {  //渡すお金を下げる
                _giveCoin -= 10;
                if (_giveCoin < 1000) {
                    _giveCoin = 1000;
                    _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
                }
            }
        }
        _pUIPopUp->SetPopString(_pUIRoomItem->GetMoneyString());
    }

    if (_giveCoin != _pUIRoomItem->GetGiveCoin()) {  //1回しかセットしない
        _pUIRoomItem->SetGiveCoin(_giveCoin);
    }

    //ストーリーが始まっているなら全て閉じ状態   スクリプトエンジン閉じると立ち上がるので直す
    if (_pScriptEngin->GetState() != amg::ScriptEngine::ScriptState::END) {
        _pUIRoomItem->StrinRoomlFalse();
        _pUIRoomStory->StrinRoomlFalse();
        _pUIPopUp->SetNowMode(false);
        _pUIRoomStory->SetEnd(true);
        _pUIRoomStory->SetNowMode(false);
        _type = RoomGameUIType::Menu;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
    }

    _pUIRoomMenuInit->Process();
    _pDrunkTime->Process();
    _pUIRoomMenu->Process();
    _pScriptEngin->Update();
    _pCloselBScript->Process();
    _pUpButton->Process();
    _pDownButton->Process();
    _pMouseInput->LastProcess();
    _pSoundManager->Process();
    _pPlayerInfo->Process();

    _downB = false;
    _upB = false;
}

void RoomGameUI::SettingProcess() {

    if (_pUIRoomSetting->GetComandSelect(0) && _pMouseInput->GetTrgLeft()) {   //0=外出
        //外出モードに変更の処理
        _goAction = true;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);
    }

    if (_pUIRoomSetting->GetComandSelect(1) && _pMouseInput->GetTrgLeft()) {  //1 = title
        //タイトルモードに変更の処理
        _goTitle = true;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);
    }

    if (_pUIRoomSetting->GetClose() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomSetting->SetEnd(true);
        _pUIRoomSetting->SetNowMode(false);
        _type = RoomGameUIType::Menu;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
    }
}

void RoomGameUI::ItemProcess() {

    //金
    if (_pUIRoomItem->GetComandSelect(0) && _pMouseInput->GetTrgLeft()) {
        _pUIPopUp->SetNowMode(true);
        _pUIRoomItem->SetMoneyStringDraw(true);
        _pUIPopUp->SetPopString(_pUIRoomItem->GetMoneyString());
        _pUIRoomItem->SetNowItemType(0);

        _pUpButton->SetDraw(0, true);
        _pUpButton->SetDraw(1, true);
        _pDownButton->SetDraw(0, true);
        _pDownButton->SetDraw(1, true);

        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    if (_pUIPopUp->GetOk() && _pUIPopUp->GetTrgLeft() && _pUIRoomItem->GetNowItemType() == 0 && !_okFlag) {  //二回目に来ている
        if ( _pPlayerInfo->GetCoin() >= _giveCoin) {
            _pPlayerInfo->SetCoin(_giveCoin, false);
            _okFlag = true;
            _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);

            //_pUIRoomMenu->PlusMentalNum(10);  //とりあえず

            _pPlayerInfo->SetMentalNum(10, true);

            _kaneOK = true;

        }
        else {
            _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Mistake);
            _kaneNo = true;
        }
    }

    //酒
    if (_pUIRoomItem->GetComandSelect(1) && _pMouseInput->GetTrgLeft()) {
        _pUIPopUp->SetNowMode(true);
        _pUIRoomItem->SetTequilaStringDraw(true);
        _pUIRoomItem->SetNowItemType(1);

        _pUIPopUp->SetPopString(_pUIRoomItem->GetTequilaString());
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    if (_pUIPopUp->GetOk() && _pUIPopUp->GetTrgLeft() && _pDrunkTime->GetNowTime() == 0 &&
        _pUIRoomItem->GetNowItemType() == 1) {
        _sakeItem = true;
        _pDrunkTime->SetStart(DRUNK_TIME);
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);

        _pPlayerInfo->SetMentalNum(30, true);
    }

    if (_pUIRoomItem->GetClose() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomItem->SetEnd(true);
        _pUIRoomItem->SetNowMode(false);
        _type = RoomGameUIType::Menu;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
    }
}

void RoomGameUI::StoryProcess() {

    //0Story
    if (_pUIRoomStory->GetComandSelect(0) && _pMouseInput->GetTrgLeft()) { 
        _pUIPopUp->SetNowMode(true);
        _pUIRoomStory->SetStory0StringDraw(true);

        _pUIPopUp->SetPopString(_pUIRoomStory->GetStory0String());
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //1Story
    if (_pUIRoomStory->GetComandSelect(1) && _pMouseInput->GetTrgLeft()) { 
        _pUIPopUp->SetNowMode(true);

        _pUIRoomStory->SetStory1StringDraw(true);

        _pUIPopUp->SetPopString(_pUIRoomStory->GetStory1String());
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //2Story
    if (_pUIRoomStory->GetComandSelect(2) && _pMouseInput->GetTrgLeft()) {  //1 = title
        _pUIPopUp->SetNowMode(true);

        _pUIRoomStory->SetStory2StringDraw(true);

        _pUIPopUp->SetPopString(_pUIRoomStory->GetStory2String());
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //3Story
    if (_pUIRoomStory->GetComandSelect(3) && _pMouseInput->GetTrgLeft()) {  //1 = title
        _pUIPopUp->SetNowMode(true);

        _pUIRoomStory->SetStory3StringDraw(true);

        _pUIPopUp->SetPopString(_pUIRoomStory->GetStory3String());
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //4Story
    if (_pUIRoomStory->GetComandSelect(4) && _pMouseInput->GetTrgLeft()) {  //1 = title
        _pUIPopUp->SetNowMode(true);

        _pUIRoomStory->SetStory4StringDraw(true);

        _pUIPopUp->SetPopString(_pUIRoomStory->GetStory4String());
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
    }

    //PopUpOK
    if (_pUIRoomStory->GetStringType() == 0 && _pUIPopUp->GetOk() && _pUIPopUp->GetTrgLeft()) {
        _pScriptEngin->ReInitialize();
        _pScriptEngin->SetState(amg::ScriptEngine::ScriptState::PARSING);
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);
    }

    //PopUpClose
    if (_pUIRoomStory->GetClose() == 1 && _pMouseInput->GetTrgLeft()) {
        _pUIRoomStory->SetEnd(true);
        _pUIRoomStory->SetNowMode(false);
        _type = RoomGameUIType::Menu;
        _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
    }
}

void RoomGameUI::Draw() {

    _pMouseInput->Draw();
    _pUIRoomSetting->Draw();
    _pUIRoomStory->Draw();
    _pUIRoomMenuInit->Draw();
    _pUIRoomMenu->Draw();

    _pUIRoomItem->Draw();

    if (_pUIPopUp->GetNowMode()) {
        if (_pUIPopUp->GetClose() && _pUIPopUp->GetTrgLeft() || _pUIPopUp->GetOk() && _pUIPopUp->GetTrgLeft()) {
            if (_pUIPopUp->GetClose()) {
                _pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::Cancel);
            }
            _pUIPopUp->SetNowMode(false);
            _pUIRoomItem->StrinRoomlFalse();
            _pUIRoomStory->StrinRoomlFalse();
            _pUpButton->SetDraw(0,false);
            _pUpButton->SetDraw(1, false);
            _pDownButton->SetDraw(0,false);
            _pDownButton->SetDraw(1, false);
            _okFlag = false;
        }
        _pUIPopUp->Draw();
    }

    if (_pScriptEngin->GetState() != amg::ScriptEngine::ScriptState::END) {
        _pScriptEngin->Render();
        _pCloselBScript->Draw();
    }

    _pDrunkTime->Draw();
    _pUpButton->Draw();
    _pDownButton->Draw();

   //DrawFormatString(20, 780, GetColor(255, 165, 0), "GameUI数字:%d",_pPlayerInfo->GetMentalNum());
   // DrawFormatString(20, 880, GetColor(255, 165, 0),"GameUIber:%d",_molecule);
}

void RoomGameUI::Terminate() {
    _pScriptEngin->Destroy();
}