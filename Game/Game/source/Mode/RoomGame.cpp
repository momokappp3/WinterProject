#include "../Mode/RoomGame.h"
#include <string>
#include "ModeTitle.h"
#include "Action3DGame.h"

const VECTOR novelCameraPositionEnd = { 0.006049f,21.895990f,-10.059654f };
const VECTOR novelCameraPositionStart = { 0.1f, 10.0f, -20.0f };
const VECTOR novelCameraTargetStart = { 0.0f, 10.0f, 0.0f };
const VECTOR novelCameraTargetEnd = { 0.006048f, 11.331523f, 12.522373f };

RoomGame::RoomGame() {

	_pCamera = nullptr;
	_pCharaModel = nullptr;
	_pMouseInput = nullptr;
	_pOnnaModel = nullptr;
	_pOnnaRedModel = nullptr;
	_pInput = nullptr;
	_pVectorTweenPotion = nullptr;
	_pVectorTweenTarget = nullptr;
	_pRoomGameUI = nullptr;
	_pFace = nullptr;
	_pSoundManager = nullptr;

	_pPlayerInfo = nullptr;

	//_favor = 0;
	//_molecule = 0;
	//_coin = 0;
}

RoomGame::~RoomGame() {
}

bool RoomGame::Initialize() {

	if (!ModeBase::Initialize()) {
		return false;
	}

	_pCamera.reset(new Camera);
	_pRoomModel.reset(new Model);
	_pInput.reset(new Input);
	_pOnnaModel.reset(new AnimationBase);
	_pOnnaRedModel.reset(new AnimationBase);
	_pVectorTweenPotion.reset(new VectorTween);
	_pVectorTweenTarget.reset(new VectorTween);
	_pRoomGameUI.reset(new RoomGameUI);
	_pMouseInput.reset(new MouseInput);
	_pFace.reset(new Face);
	_pSoundManager.reset(new SoundManager);

	_pPlayerInfo.reset(new PlayerInfo);

	_pPlayerInfo->SetCoin(5000, true);
	_pPlayerInfo->SetMentalNum(50, true);

	if (!_pRoomGameUI->Init(_pSoundManager,_pPlayerInfo)||!_pSoundManager->Init()){  //falseに来てる×
		return false;	
	}

	_pSoundManager->LoadSERoomGame();

	//_pCamera->SetPosition(0.0f, 10.0f, -20.0f);
	//_pCamera->SetTarget(0.0f, 10.0f, 0.0f);
	//_pCamera->SetNearFar(0.1f, 800.0f);

	_pCamera->SetPosition(0.006049f, 21.895990f, -10.059654f);
	_pCamera->SetTarget(0.006048f, 11.331523f, 12.522373f);
	_pCamera->SetNearFar(0.1f, 800.0f);

	_pOnnaModel->Load("model/onna/onna.pmx");
	_pOnnaRedModel->Load("model/onna_red/onna_red.pmx");
	_pRoomModel->Load("model/room/roomkagu.pmx");

	_pFace->Init(_pOnnaModel->GetHandle());

	return true;
}

bool RoomGame::Process() {

	if (_pMouseInput == nullptr) {
		return false;
	}
	_pMouseInput->Process();

	_pOnnaModel->Play(true, 0, 0.3f);
	_pOnnaRedModel->Play(true, 0, 0.3f);

	//FaceInfo info = _pRoomGameUI->GetFaceInfo();
	//_pFace->SetInfo(info.faceNum, info.min, info.max, info.tweenFrame,info.stopTime);


	//ここで落ちている boolをUIクラスでGetにしたらtrueにSetの時falseにすることで一回だけSetさせて解決
	
	if (_pRoomGameUI != nullptr && _pRoomGameUI->GetIsFaceInfo()) {
		FaceInfo info = _pRoomGameUI->GetFaceInfo();

		_pFace->SetInfo(info.faceNum, info.min, info.max, info.tweenFrame, info.stopTime);
	}

	//第一引数:表情番号　2:表情の最小変化　3:表情の最大変化 4:変化速度 5:StopTime
	if (CheckHitKey(KEY_INPUT_G)) {
		_pFace->SetInfo(6, 0.0, 1.0, 90);
	}
	if (CheckHitKey(KEY_INPUT_J)) {
		_pFace->SetInfo(6, 0.0, 0.4, 50,300);
	}
	if (CheckHitKey(KEY_INPUT_K)) {  //眠たげ
		_pFace->SetInfo(18, 0.0, 1.0, 120,800);
	}

	if (_pInput->_key[(KEY_INPUT_Q)] == 1) {

		_pVectorTweenPotion->SetVectorTween(novelCameraPositionEnd, novelCameraPositionStart, 45,VectorTween::Type::SineStart);
		_pVectorTweenTarget->SetVectorTween(novelCameraTargetEnd, novelCameraTargetStart, 45, VectorTween::Type::SineStart);
	}

	if (_pInput->_key[(KEY_INPUT_W)] == 1) {

		//カメラ移動
		_pVectorTweenPotion->SetVectorTween(novelCameraPositionStart, novelCameraPositionEnd, 45, VectorTween::Type::SineStart);
		_pVectorTweenTarget->SetVectorTween(novelCameraTargetStart, novelCameraTargetEnd, 45, VectorTween::Type::SineStart);
	}

	if (_pVectorTweenPotion->IsStart() && !_pVectorTweenPotion->IsEnd()) {

		VECTOR CameraPositionNow = _pVectorTweenPotion->GetPosition();
		float pX = CameraPositionNow.x, pY = CameraPositionNow.y, pZ = CameraPositionNow.z;

		_pCamera->SetPosition(pX, pY, pZ);
	}

	if (_pVectorTweenTarget->IsStart() && !_pVectorTweenTarget->IsEnd()) {

		VECTOR CameraTargetNow = _pVectorTweenTarget->GetPosition();
		float tX = CameraTargetNow.x, tY = CameraTargetNow.y, tZ = CameraTargetNow.z;

		_pCamera->SetTarget(tX, tY, tZ);
	}

	if (_pRoomGameUI->GetSakeItem()) {
		_pOnnaRedModel->Process();
	}
	else {
		_pOnnaModel->Process();
	}



	if (_pRoomGameUI->GetKaneOK()) {
		_pRoomGameUI->SetKaneOK(false);
		_pFace->SetInfo(1, 0.0, 0.4, 90);
	}
	if (_pRoomGameUI->GetKaneNO()) {
		_pRoomGameUI->SetKaneNo(false);
		_pFace->SetInfo(2, 0.0, 0.3, 50);
	}
	if (_pRoomGameUI->GetGoTitle()) {
		_pRoomGameUI->SetGoTitle(false);

		ModeServer::GetInstance()->Del(this);  // このモードを削除予約
		ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");  // 次の
		_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);
	}
	if (_pRoomGameUI->GetGoActoin()) {
		_pRoomGameUI->SetGoAction(false);

		ModeServer::GetInstance()->Del(this);  // このモードを削除予約
		ModeServer::GetInstance()->Add(new Action3DGame(), 2, "Action3DGame");  // 次の
		_pSoundManager->PlaySERoomGame(SoundManager::SERoomGame::OK2);
	}

	ModeBase::Process();
	_pCamera->Process();
	_pRoomModel->Process();

	_pRoomGameUI->Process();
	_pInput->Process();
	_pVectorTweenPotion->Process();
	_pVectorTweenTarget->Process();
	_pFace->Process();
	_pSoundManager->Process();

	return true;
}

bool RoomGame::Render() {

	_pRoomModel->Render();

	if (_pRoomGameUI->GetSakeItem()) {
		_pOnnaRedModel->Render();
	}
	else {
		_pOnnaModel->Render();
	}

	_pRoomGameUI->Draw();
	_pMouseInput->Draw();  //最後に必ず置く

	//debug情報
	_pCamera->Render();
	//DrawFormatString(100, 900, GetColor(255, 0, 0), "好感度%d",_favor);
	//DrawFormatString(100, 930, GetColor(255, 0, 0), "分子%d", _molecule);

	return true;
}

bool RoomGame::Terminate() {

	_pRoomGameUI->Terminate();
	return true;
}