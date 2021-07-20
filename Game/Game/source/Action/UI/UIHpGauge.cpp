#include "UIHpGauge.h"
#include "../../../ResourceServer.h"
#include "Dxlib.h"

UIHpGauge::UIHpGauge() {

    _point = { 0,0 };
    _leftUp = { 0,0 };
    _rightDown = { 0,0 };

    _pTween = nullptr;

    _hp = 0;
    _nowHp = 0;
    _nextHp = 0;

    _handle = -1;
    _imageWidth = 0;
    _imageHeight = 0;
}

UIHpGauge::~UIHpGauge() {
}

bool UIHpGauge::Init() {

    int handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/hp.png");
    _handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/hpInner.png");

    _point = { 20,20 };

    _pTween.reset(new Tween);

    if (handle == -1 || _handle == -1) {
        return false;
    }

    if (GetGraphSize(_handle, &_imageWidth, &_imageHeight) == -1) {
        return false;
    }

    DrawInfo info = { handle,{_point.x,_point.y, }, true };
    _vDrawInfo.push_back(info);

    //==========================
    //’†‚Ìhp

    _leftUp = _point;

    _rightDown = _leftUp;
    _rightDown.x += _imageWidth;
    _rightDown.y += _imageHeight;

    return true;
}

void UIHpGauge::Process() {
    _pTween->Process();

    if (_pTween->IsStart()) {
        _rightDown = _pTween->GetPosition();
    }

    if (_pTween->IsEnd()) {
        _nowHp = _nextHp;
    }
}

void UIHpGauge::Draw() {

    UI2DBase::Draw();

    SetDrawArea(_leftUp.x, _leftUp.y, _rightDown.x, _rightDown.y);
    DrawGraph(_point.x, _point.y, _handle, TRUE);
    SetDrawArea(0, 0, 1920, 1080);
}


void UIHpGauge::SetHp(int hp) {

    if (hp == _nowHp) {
        return;
    }

    _nextHp = hp;

    Point next = { 0,0 };

    GetHpArea(_nextHp, next);

    Point now = { 0,0 };

    GetHpArea(_nowHp, now);

    int diff = std::abs(_nowHp - _nextHp);

    _pTween->Reset();
    _pTween->SetTween(now, next, diff, Tween::Type::SineStart);
}

void UIHpGauge::GetHpArea(int hp, Point& rightDown) {
    /*
    int hpPixel = _imageWidth - Parameter::_HpImageMarginLeft - Parameter::_HpImageMarginRight;
    float rate = static_cast<float>(hp) / static_cast<float>(_hp);
    float hpRate = std::round(static_cast<float>(hpPixel) * rate);
    int nowHp = static_cast<int>(hpRate);
    */

    float rate = static_cast<float>(hp) / static_cast<float>(_hp);
    float hpRate = std::round(static_cast<float>(_imageWidth) * rate);
    int nowHp = static_cast<int>(hpRate);

    rightDown = _leftUp;
    rightDown.x += nowHp;
    rightDown.y += _imageHeight;
}
