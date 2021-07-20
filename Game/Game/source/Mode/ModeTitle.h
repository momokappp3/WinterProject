#pragma once
#include "appframe.h"
#include <memory>
#include "../../Camera.h"
#include "../../Model.h"
#include "../../AppFrame/MouseInput.h"
#include "../../AnimationBase.h"
#include "../.././../../AppFrame/Input.h"
#include "../../../../AppFrame/VectorTween.h"
#include "../SoundManager.h"
#include "../../../../AppFrame/Effect3D.h"

class ModeTitle : public ModeBase {
public:
	ModeTitle();
	virtual ~ModeTitle();

	enum class Kind {
		Max = 0,
		NewGame,
		LoadGame,
		Option,
		Help,
		End
	};

	enum class CameraKind {
		Title = 0,
		Menu,
		TitleUp

	};

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:

	bool SetModelInitInfo();
	void MoveUpDown();
	void TouchTitleMenu();

	std::unique_ptr<Camera> _pCamera;
	std::unique_ptr<Model> _pStageModel;
	//std::unique_ptr<Effect3D> _pEffect3D;

	std::unique_ptr<Model> _pLove;
	std::unique_ptr<Model> _pLove2;
	std::unique_ptr<Model> _pSweet;
	std::unique_ptr<Model> _pHeart;
	std::unique_ptr<Model> _pHeart2;
	std::unique_ptr<Model> _p2Heart;
	std::unique_ptr<Model> _p2Heart2;
	std::unique_ptr<Model> _pEx;
	std::unique_ptr<Model> _pNewGame;
	std::unique_ptr<Model> _pLoadGame;
	std::unique_ptr<Model> _pOption;
	std::unique_ptr<Model> _pHelp;
	std::unique_ptr<Model> _pExit;

	std::unique_ptr<MouseInput> _pMouseInput;
	std::unique_ptr<AnimationBase> _pAnimationBase;
	std::unique_ptr<Input> _pInput;

	std::unique_ptr<VectorTween> _pVectorTweenPotion;
	std::unique_ptr<VectorTween> _pVectorTweenTarget;
	std::unique_ptr<VectorTween> _pLoveTweenScale;

	std::unique_ptr<VectorTween> _pTweenMove1;
	std::unique_ptr<VectorTween> _pTweenMove2;
	std::unique_ptr<VectorTween> _pTweenMove3;
	std::unique_ptr<VectorTween> _pTweenMove4;
	std::unique_ptr<VectorTween> _pTweenMove5;

	//ƒ^ƒCƒgƒ‹•¶Žš
	std::unique_ptr<VectorTween> _pTweenTitleMove1;
	std::unique_ptr<VectorTween> _pTweenTitleMove2;
	std::unique_ptr<VectorTween> _pTweenTitleMove3;
	std::unique_ptr<VectorTween> _pTweenTitleMove4;
	std::unique_ptr<VectorTween> _pTweenTitleMove5;
	std::unique_ptr<VectorTween> _pTweenTitleMove6;
	std::unique_ptr<VectorTween> _pTweenTitleMove7;
	std::unique_ptr<VectorTween> _pTweenTitleMove8;

	MV1_COLL_RESULT_POLY _hitNewGame;
	MV1_COLL_RESULT_POLY _hitLoadGame;
	MV1_COLL_RESULT_POLY _hitOption;
	MV1_COLL_RESULT_POLY _hitHelp;
	MV1_COLL_RESULT_POLY _hitExit;

	MV1_COLL_RESULT_POLY _hitHeart;
	MV1_COLL_RESULT_POLY _hitHeart2;

	Kind _menuKind;
	CameraKind _cameraKind;

	bool _isBgm;
	int _lastNewGameHit;
};