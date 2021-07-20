#include "../../../../AppFrame/source/appframe.h"
#include "../ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"
#include "../../../../AppFrame/MouseInput.h"
//#include "ModeClear.h"
#include "Action3DGame.h"

Action3DGame::Action3DGame() {

	_pMazeStage = nullptr;
	_pHp = nullptr;
	_pUIPopUp = nullptr;
	//_pUITime = nullptr;
	_pUIItem = nullptr;
	_pItem = nullptr;
	_pKeyInput = nullptr;

	_pMouseInput = nullptr;

	_isBGM = false;
}

Action3DGame::~Action3DGame() {
}

bool Action3DGame::Initialize() {

	if (!ModeBase::Initialize()) {
		return false;
	}

	if (_pSoundManager != nullptr) {
		bool seGame = _pSoundManager->LoadSECommon();

		if (!seGame) {
			return false;
		}
	}
	else {
		return false;
	}

	_pKeyInput.reset(new Input);
	_pMazeStage.reset(new MazeStage);
	_pUIPopUp.reset(new UIActionPopUp);
	_pHp.reset(new UIHpGauge);
	//_pUITime.reset(new UITime);
	_pItem.reset(new Item);
	_pUIItem.reset(new UIItem);
	_pMouseInput.reset(new MouseInput);

	if (!_pMazeStage->Initialize(_pSoundManager) || !_pUIPopUp->Init() || !_pHp->Init() || /*!_pUITime->Init() ||*/
		!_pItem->Init(_pSoundManager) || !_pUIItem->Init()) {
		return false;
	}

	_pHp->InitHP(2000);

	_pUIPopUp->SetNowMode(false);

	return true;
}

bool Action3DGame::Process() {

	//ゲームのBGM
	if (!_isBGM) {
		if (_pSoundManager != nullptr) {
			_pSoundManager->PlayBgm(SoundManager::BGM::ActionGame);
			_isBGM = true;
			//_pUITime->SetStart(3, 20);
		}
	}

	//=======================================================
	//test

	if (_pKeyInput->_key[(KEY_INPUT_R)] == 1) {
		_pItem->SetItem(ITEM::Barrier);
	}

	if (_pKeyInput->_key[(KEY_INPUT_T)] == 1) {
		_pItem->SetItem(ITEM::Portion);
	}

	if (_pKeyInput->_key[(KEY_INPUT_Y)] == 1) {
		_pItem->SetItem(ITEM::Through);
	}

	_pUIItem->SetUpperItem(_pItem->GetUpperItem());
	_pUIItem->SetMiddleItem(_pItem->GetMiddleItem());
	_pUIItem->SetDownItem(_pItem->GetDownItem());

	_pMazeStage->GetModeCount(GetModeCount());

	//攻撃が当たったら
	if (_pMazeStage->GetIsHit()) {
		_pHp->SetHp(1500);
	}

	//エスケープキーでタイトルに
	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {  //Titleに戻る処理
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	}

	//=================================================
	//DoorのPopUp表示クリアに行く処理
	if (_pMazeStage->GetIsDoorArea()) {
		_pUIPopUp->SetNowMode(true);
		_pUIPopUp->SetPopString({ "ここから出る" ,808,693,true });
		// DrawFormatString(500, 500, GetColor(0, 128, 128), "HIT");
		if (_pUIPopUp->GetOk()) {
			//popUpOKならクリアに行く
			if (_pKeyInput->_key[(KEY_INPUT_RETURN)] == 1) {
				ModeServer::GetInstance()->Del(this);
				//ModeServer::GetInstance()->Add(new ModeClear(), 1, "clear");
				_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
			}
		}
	}
	else {
		_pUIPopUp->SetNowMode(false);
	}

	//=======================================================
	//宝箱のポップアップ&アイテム

	if (_pMazeStage->GetBox()->GetIsPopUp()) {
		_pUIPopUp->SetNowMode(true);
		if (_pMazeStage->GetBox()->GetIsItem()) {

			switch (_pMazeStage->GetBox()->GetItemNum()) {
			case ITEM::Barrier:
				_pUIPopUp->SetPopString({ "バリア" ,833,703,true });
				break;
			case ITEM::Portion:
				_pUIPopUp->SetPopString({ "回復薬" ,833,703,true });
				break;
			case ITEM::Through:
				_pUIPopUp->SetPopString({ "すり抜け" ,833,703,true });
				break;
			default:
				break;
			}

			if (_pKeyInput->_key[(KEY_INPUT_RETURN)] == 1) {
				_pMazeStage->GetBox()->SetIsItem(false);
				_pItem->SetItem(_pMazeStage->GetBox()->GetItemNum());
				_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK);
			}
		}
		else {
			_pUIPopUp->SetPopString({ "何も入っていない" ,833,703,true });
		}
	}

	//_pUITime->Process();
	_pUIPopUp->Process();
	_pHp->Process();
	_pMouseInput->Process();
	_pKeyInput->Process();
	ModeBase::Process();
	_pMazeStage->Process();  //×
	_pItem->Process();

	return true;
}

bool Action3DGame::Render() {
	ModeBase::Render();
	_pMazeStage->Render();
	//_pUITime->Draw();
	_pItem->Draw();
	;
	if (_pMazeStage->GetIs3D()) {
		_pHp->Draw();
		_pUIItem->Draw();
		_pUIPopUp->Draw();
	}

	_pMouseInput->Draw();
	return true;
}

bool Action3DGame::Terminate() {
	ModeBase::Terminate();
	_pMazeStage->Terminate();
	return true;
}