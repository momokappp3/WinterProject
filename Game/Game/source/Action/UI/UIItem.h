/*********************************************************************
 * \file   UIItem.h
 * \brief  アイテム関係の表示クラス
 * \author momoka
 * \date   2021 6/17
 * \details 表示だけに特化
 *********************************************************************/
#pragma once
#include "../../UI/UI2DBase.h"
#include "../Item.h"

class UIItem : public UI2DBase {
public:
    UIItem();
    virtual ~UIItem();

    bool Init();
    void Process();
    void Draw();

    void SetUpperItem(ITEM item) {
        _upperItem = static_cast<int>(item);
    }

    void SetMiddleItem(ITEM item) {
        _middleItem = static_cast<int>(item);
    }

    void SetDownItem(ITEM item) {
        _downItem = static_cast<int>(item);
    }

private:

    int _upperItem;
    int _middleItem;
    int _downItem;
};