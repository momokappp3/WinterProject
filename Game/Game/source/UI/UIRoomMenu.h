#pragma once
#include <memory>
#include "UI2DBase.h"
#include "UI2DSelectBase.h"
#include "UIInAndOut.h" 
#include "../../../../AppFrame/Tween.h"
#include "../MentalGauge.h"
#include "UINumber.h"
#include "../LevelUpGauge.h"
#include "../PlayerInfo.h"

class UIRoomMenu{
public:
    UIRoomMenu();
    virtual ~UIRoomMenu();

    bool Init(std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<PlayerInfo>& playerInfo);
    void Process();
    void Draw();

    void SetStart(bool start) {
        _start = start;
    }

    void SetEnd(bool end) {
        _end = end;
    }

    void SetMousePoint(int x, int y) {  //使用クラスで受け取る
        _mouseX = x;
        _mouseY = y;
    }

    int GetSelectCancel() {
        return _cancel;
    }

    int GetSelectSetting() {
        return _setting;
    }

    void SetSelectSetting(bool num) {
        _setting = num;
    }

    int GetSelectStory() {
        return _story;
    }

    int GetSelectItem() {
        return _item;
    }

    void SetSelectItem(int num) {
        _item = num;
    }
    
private:

    bool DrawInit();
    void MoveInit();
    void MoveProcess();

    //描画
    std::unique_ptr <UI2DSelectBase> _pSettingSelectBase;
    std::unique_ptr<UI2DSelectBase> _pCancelSelectBase;
    std::unique_ptr<UI2DSelectBase> _pBstorySelectBase;
    std::unique_ptr<UI2DSelectBase> _pBitemSelectBase;

    std::unique_ptr<LevelUpGauge> _pBarPinkBase;
    std::unique_ptr<MentalGauge> _pBarRedBase;

    std::unique_ptr<UI2DBase> _pCircleBase;

    std::unique_ptr<UI2DBase> _pCoinBaseBase;
    std::unique_ptr<UINumber> _pCoinNum;

    std::unique_ptr<UINumber> _pTrustNum;

    //アニメーション出し入れ
    std::unique_ptr<UIInAndOut> _pSettingInAndOut;
    std::unique_ptr<UIInAndOut> _pCancelInAndOut;
    std::unique_ptr<UIInAndOut> _pBstoryInAndOut;
    std::unique_ptr<UIInAndOut> _pBitemInAndOut;

    std::unique_ptr<UIInAndOut> _pBarPinkInAndOut;
    std::unique_ptr<UIInAndOut> _pBarRedInAndOut;

    std::unique_ptr<UIInAndOut> _pCircleInAndOut;

    std::unique_ptr<UIInAndOut> _pCoinBaseInAndOut;
    std::unique_ptr<UIInAndOut> _pCoinNumInAndOut;
    std::unique_ptr<UIInAndOut> _pTrustNumInAndOut;

    std::shared_ptr<SoundManager> _pSoundManager;
    std::shared_ptr<PlayerInfo> _pPlayerInfo;

    bool _start;
    bool _end;

    int _cancel;
    int _setting;
    int _story;
    int _item;

    int _mouseX;
    int _mouseY;

    int _lastMolecule;
    int _lastTotalFavor;
    int _lastMentalNum;
};