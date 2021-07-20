#pragma once
#include <tchar.h>
#include "Types.h"
#include "../AppFrame/source/Mode/ModeBase.h"
/*
class Effect3D : public ModeBase {
public:
	Effect3D();
	virtual ~Effect3D();

	
	bool Init(const TCHAR* fileName, float magnification, bool isLoop);
	virtual void Start();
	virtual void Stop();
	virtual void Process();
	virtual void ProcessPoint();
	virtual void Delete();
	virtual void Draw();

	Point GetPoint() {
		return _point;
	}

	void SetPoint(Point point) {
		_point = point;
	}

protected:

	Point _point;

	int _effectResourceHandle;
	int _playingEffectHandle;   //動かす時に使う

	bool _isLoop;
	bool _isEnd;

	virtual bool Init(const TCHAR* fileName, float magnification);
	virtual bool Terminate();
	virtual bool Process();
	virtual void Draw();

protected:
	int		_effectResourceHandle;		// エフェクトファイルをロードするハンドル
	int		_playingEffectHandle;		// ロードしたエフェクトファイルから、エフェクトを生成したもの

	// エフェクト位置
	float	_position_x = 0.0f;
	float	_position_y = 0.0f;
};

*/