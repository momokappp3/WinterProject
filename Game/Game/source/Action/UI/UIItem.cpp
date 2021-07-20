#include "UIItem.h"
#include "../../../ResourceServer.h"
#include "Dxlib.h"

UIItem::UIItem() {
	_upperItem = 0;
	_middleItem = 0;
	_downItem = 0;
}

UIItem::~UIItem() {
}

bool UIItem::Init() {

	//Base
	int handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/itemBase.png");

	if (handle == -1) {
		return false;
	}

	DrawInfo info = { handle,10,750,true };  //è„ÇÃBase
	SetDrawInfo(info);

	info = { handle,100,850,true };  //ê^ÇÒíÜÇÃBace
	SetDrawInfo(info);

	info = { handle,142,964,true };  //â∫ÇÃBase
	SetDrawInfo(info);

	//òg
	handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/itemWaku3.png");

	if (handle == -1) {
		return false;
	}

	info = { handle,100,850,true };
	SetDrawInfo(info);

	//============================================================
	//itemÇÃâÊëú 4Ç©ÇÁ
	handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/baria.png");

	if (handle == -1) {
		return false;
	}

	info = { handle,0,0,false };
	SetDrawInfo(info);

	handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/portion.png");

	if (handle == -1) {
		return false;
	}

	info = { handle,0,0,false };
	SetDrawInfo(info);

	handle = ResourceServer::LoadGraph("png/ActionUI/gameUI/surinuke.png");

	if (handle == -1) {
		return false;
	}

	info = { handle,0,0,false };
	SetDrawInfo(info);


	return true;
}

void UIItem::Process() {


}

void UIItem::Draw() {

	for (int i = 0; i < 3; i++) {
		DrawGraph(_vDrawInfo[i].xy.x, _vDrawInfo[i].xy.y, _vDrawInfo[i].handle, TRUE);
	}

	//itemÇÃï`âÊ
	if (_upperItem != 0) {
		DrawGraph(_vDrawInfo[0].xy.x, _vDrawInfo[0].xy.y, _vDrawInfo[_upperItem].handle, TRUE);
	}
	if (_middleItem != 1) {
		DrawGraph(_vDrawInfo[1].xy.x, _vDrawInfo[1].xy.y, _vDrawInfo[_middleItem].handle, TRUE);
	}
	if (_downItem != 2) {
		DrawGraph(_vDrawInfo[2].xy.x, _vDrawInfo[2].xy.y, _vDrawInfo[_downItem].handle, TRUE);
	}

	//òg
	DrawGraph(_vDrawInfo[3].xy.x, _vDrawInfo[3].xy.y, _vDrawInfo[3].handle, TRUE);
}
