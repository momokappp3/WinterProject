/*****************************************************************//**
 * \file   UIPopUp.h
 * \brief  popUp表示選択クラス
 * \author momoka
 * \date   2021 6/14
 * \ditaile SetNowMode->(true/false)をtureにすることで表示
 *          SetPopString->({})で文字列指定
 *********************************************************************/
#pragma once
#include "../../UI/UI2DBase.h"
#include "../../UI/UI2DSelectBase.h"
#include <memory>
#include "../../../../../AppFrame/Types.h"
#include "../../../../../AppFrame/Input.h"

class UIActionPopUp {
public:
    UIActionPopUp();
    virtual ~UIActionPopUp();

    bool Init();
    void Process();
    void Draw();

    int GetOk() {
        return _ok;
    }

    bool GetNowMode() {
        return _nowMode;
    }

    void SetNowMode(bool nowMode) {
        _nowMode = nowMode;
    }

    void SetPopString(PopString string) {
        _popString = string;
    }

private:
    std::unique_ptr<UI2DBase> _pPopUpBase;
    std::unique_ptr<UI2DSelectBase> _pOkBSelectBase;
    std::unique_ptr<Input> _pKeyInput;

    bool _ok;
    bool _nowMode;  

    PopString _popString;

    int _key[256];
    int _fontHandle;
};