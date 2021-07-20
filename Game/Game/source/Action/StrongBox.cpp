#include "StrongBox.h"
#include "../../../../AppFrame/MTRandom.h"

StrongBox::StrongBox() {
    _pStrongBox = nullptr;
	_pSoundManager = nullptr;

	_point = { 0,0,0 };

	_player3DPosi = { 0,0,0 };
	_endPlayerPosi = { 0,0,0 };

	_vertex1 = { 0,0,0 };
	_vertex2 = { 0,0,0 };
	_vertex3 = { 0,0,0 };
	_vertex3Anime = { 0,0,0 };

	_isPopUp = false;
	_isAnim = false;
	_isItem = true;

	_itemNum = -1;
}

StrongBox::~StrongBox() {
}

bool StrongBox::Init(std::shared_ptr<SoundManager>sound,VECTOR point) {

	_pSoundManager = sound;
	
	if (_pSoundManager == nullptr) {
		return false;
	}
	if (_pSoundManager != nullptr) {
		bool seTitle = _pSoundManager->LoadSEInGame();

		if (!seTitle) {
			return false;
		}
	}
	else {
		return false;
	}

    //置く場所をもらう
    _point = point;

	_pStrongBox.reset(new ModelAnimation);
	_pStrongBox->Load("model/Action3D/boxAnime5.mv1");

	if (_pStrongBox->GetHandle() == -1) {
		return false;
	}

	_pStrongBox->AnimationPushBack(0, 0, 0.0f, 0.8f);  //鍵
	_pStrongBox->AnimationPushBack(0, 3, 0.0f, 0.9f);  //開く  

	//設置する場所
	//(プレイヤーの二個隣)  (行き止まりに設置する)
	_pStrongBox->GetTransform().SetPosition(_point);
	_pStrongBox->GetTransform().SetScale({ 0.18f, 0.18f, 0.18f });
	_pStrongBox->GetTransform().SetDirection({ 0.0f,0.0f,120.0f });

	_vertex1 = { _point.x - 20.0f,_point.y,_point.z };
	_vertex2 = { _point.x + 20.0f,_point.y,_point.z };
	_vertex3 = { _point.x,_point.y,_point.z - 100.0f };
	_vertex3Anime = { _point.x,_point.y,_point.z - 50.0f };

	//=============================================
	//ランダムでアイテムを何にするか決める
	//保持(4～6)
	_itemNum = MTRandom::Get(4, 6);

    return true;
}

void StrongBox::Process() {
	
	//宝箱三角形と線分の当たり判定
	_isPopUp = HitCheck_Line_Triangle(_player3DPosi, _endPlayerPosi, _vertex1, _vertex2, _vertex3Anime).HitFlag == 1;

	//Anime当たり判定
	if (!_isAnim && HitCheck_Line_Triangle(_player3DPosi, _endPlayerPosi, _vertex1, _vertex2, _vertex3).HitFlag) {
		_pStrongBox->PlayAnimation(0);
		_pSoundManager->PlaySEInGame(SoundManager::InGame::BoxOpen);
		_isAnim = true;
	}

	_pStrongBox->Process();
}

void StrongBox::Draw(){

	_pStrongBox->Draw();

	//プレイヤーから伸びた線
	DrawLine3D(_player3DPosi, _endPlayerPosi, GetColor(255, 30, 0));

	//ドアエリアの当たり判定描画(三角)
	DrawTriangle3D(_vertex1, _vertex2, _vertex3, GetColor(255, 255, 0), FALSE);

	//ドアアニメの当たり判定描画(三角)
	DrawTriangle3D(_vertex1, _vertex2, _vertex3Anime, GetColor(255, 30, 0), FALSE);
}