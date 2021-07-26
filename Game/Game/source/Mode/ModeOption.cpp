#include "ModeOption.h"
#include "../../ResourceServer.h"
#include "ModeTitle.h"

ModeOption::ModeOption() {
    _backHandle = -1;
}

ModeOption::~ModeOption() {
}

bool ModeOption::Initialize() {
    _backHandle = ResourceServer::LoadGraph("png/Common/soundBack.png");

    if (_backHandle == -1) {
        return false;
    }
    return true;
}

bool ModeOption::Process() {
    //仮
    //エスケープキーでタイトルに
    if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {  //Titleに戻る処理
        ModeServer::GetInstance()->Del(this);
        ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
    }

    //マウス長押しクリック　移動させる
    //Getした割合で音量を下げる



    return true;
}

bool ModeOption::Render(){

    DrawGraph(0, 0, _backHandle, TRUE);
    return true;
}

bool ModeOption::Terminate() {
    return true;
}