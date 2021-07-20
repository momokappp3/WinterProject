/*****************************************************************
 * \file   Item.h
 * \brief  アイテム所持クラス内部情報
 * \author momoka
 * \date   2021 6/21
 *********************************************************************/
#pragma once
#include <vector>
#include "../../../../AppFrame/Input.h"
#include <memory>
#include "../../../../AppFrame/Types.h"
#include "../SoundManager.h"

//上方向に動いたか　下方向に動いたか
class Item{
public:
    Item();
    virtual ~Item();

    bool Init(std::shared_ptr<SoundManager> sound);
    void Process();
    void Draw();  //debug用

    void SetItem(ITEM item) {

        if (_vItem.size() == 1) {
            _vItem.push_back(item);
            _vItem.push_back(ITEM::None);
        }
        else if( _vItem.size() == 3 && _vItem[2] == ITEM::None){
            _vItem.pop_back();
            _vItem.push_back(item);
        }
        else {
            _vItem.push_back(item);
        }
    }

    int GetItemSize() {
        return _vItem.size();
    }

    ITEM GetUpperItem() {
        
        if (_vItem.size() >= 2) {
            return _vItem[(_itemSelect + 1 )% _vItem.size()];
        }
        return ITEM::None;
    }

    ITEM GetMiddleItem() {
        if (_vItem.size() == 0) {
            return ITEM::None;
        }
        return _vItem[_itemSelect];
    }

    ITEM GetDownItem() {
        if (_vItem.size() >= 2) {
            return _vItem[(_itemSelect + (_vItem.size() - 1)) % _vItem.size()];
        }
        return ITEM::None;
    }

private:
    std::unique_ptr<Input> _pInput;
    std::shared_ptr<SoundManager> _pSoundManager;

    std::vector<ITEM> _vItem;  //現在入っているアイテム

    int _itemSelect;  //_vItemの何番目か
};