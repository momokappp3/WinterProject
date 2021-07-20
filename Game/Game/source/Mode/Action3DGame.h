/*****************************************************************//**
 * \file   Action3DGame.h
 * \brief  ゲームシーンクラス
 * \author momoka
 * \date   2021 7/19
 *********************************************************************/
#pragma once
#include "../../../../AppFrame/source/appframe.h"
#include <memory>
#include "../Action/MazeStage.h"
#include"../UI/UIPopUp.h"
#include "../../../../AppFrame/Input.h"
#include "../SoundManager.h"
#include "../Action/UI/UIHpGauge.h"
#include "../UI/UITime.h"
#include "../Action/UI/UIItem.h"
#include "../Action/Item.h"
#include "../Action/UI/UIActionPopUp.h"

class Action3DGame : public ModeBase {
public:
	Action3DGame();
	virtual ~Action3DGame();
	virtual bool Initialize();
	virtual bool Process();
	virtual bool Render();
	virtual bool Terminate();
private:
	std::unique_ptr<Input> _pKeyInput;
	std::unique_ptr<MazeStage> _pMazeStage;
	std::unique_ptr<UIActionPopUp> _pUIPopUp;
	std::unique_ptr<UIHpGauge> _pHp;
	//std::unique_ptr<UITime> _pUITime;
	std::unique_ptr<UIItem> _pUIItem;
	std::unique_ptr<Item> _pItem;

	std::unique_ptr<MouseInput> _pMouseInput;

	bool _isBGM;

	int _hpNum;
};