/*****************************************************************//**
 * \file   UIHpGauge.cpp
 * \brief  ëÃóÕÉQÅ[ÉWÇÃï`âÊìÆÇ´
 * \author momoka
 * \date   2021 6/16
 *********************************************************************/
#pragma once
#include "../../UI/UI2DBase.h"
#include "../../../../../AppFrame/Tween.h"

class UIHpGauge :public UI2DBase {
public:
    UIHpGauge();
    virtual ~UIHpGauge();

    bool Init()override;
    void Process()override;
    void Draw()override;

    void InitHP(int hp) {
        _hp = hp;
        _nowHp = hp;
        _nextHp = hp;
    }

    void SetHp(int hp);

    int GetHp() {
        return _nowHp;
    }

private:
    void GetHpArea(int hp, Point& rightDown);

    Point _point;
    Point _leftUp;
    Point _rightDown;

    std::unique_ptr<Tween> _pTween;

    int _hp;
    int _nowHp;
    int _nextHp;

    int _handle;
    int _imageWidth;
    int _imageHeight;
};
