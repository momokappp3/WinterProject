#include "Item.h"
#include "Dxlib.h"


Item::Item() {

    _vItem.clear();
    _itemSelect = 0;
    _pInput = nullptr;
    _pSoundManager = nullptr;
}

Item::~Item() {
}

bool Item::Init(std::shared_ptr<SoundManager> sound) {

    _pSoundManager = sound;

    if (_pSoundManager == nullptr) {
        return false;
    }

    _pInput.reset(new Input);

    auto onSelect = [this]() {
        //サウンド鳴らす
        _pSoundManager->PlaySECommon(SoundManager::SECommon::Select);
    };

    return true;
}

void Item::Process(){

    if (!_vItem.empty()) {
        _pInput->Process();

        if (_vItem.size() > 1) {
            if (_pInput->_key[(KEY_INPUT_Z)] == 1) {
                _itemSelect = (_itemSelect + 1) % _vItem.size(); //(ループする)
                _pSoundManager->PlaySECommon(SoundManager::SECommon::Select);
            }

            if (_pInput->_key[(KEY_INPUT_C)] == 1) {
                _itemSelect = (_itemSelect + (_vItem.size() - 1)) % _vItem.size();
                _pSoundManager->PlaySECommon(SoundManager::SECommon::Select);
            }
        }
    }
}

void Item::Draw(){

    if (!_vItem.empty()) {
        DrawFormatString(20, 100, GetColor(255, 255, 255), "ItemSize::%d", _vItem.size());
        DrawFormatString(20, 120, GetColor(255, 255, 255), "ItemSelect::%d", _itemSelect);

        DrawFormatString(20, 140, GetColor(255, 255, 255), "Item::%d", static_cast<int>(_vItem[_itemSelect]));
       // DrawFormatString(20, 160, GetColor(255, 255, 255), "ItemUp::%d", static_cast<int>(_vItem[(_itemSelect + 1) % _vItem.size()]));
        //DrawFormatString(20, 180, GetColor(255, 255, 255), "ItemDown::%d", static_cast<int>(_vItem[(_itemSelect + (_vItem.size() - 1)) % _vItem.size()]));
    }
}