/*****************************************************************//**
 * \file   Transform.h
 * \brief  モデル自体の回転動き大きさ
 * \author momoka
 * \date   2021 7/22
 *********************************************************************/
#pragma once
#include "DxLib.h"

class Transform {
public:
	static float ToRadian(float angle);
	static float ToAngle(float radian);

	Transform();
	virtual ~Transform();

	void SetBaseDirection(float x, float y, float z) {
		_vBaseDirection = VNorm(VGet(x, y, z));
	}

	void Forward(float speed);
	void SetRotateY(float angle);
	void AddRotateY(float angle);
	void Update();

	//指定する関数と加える関数 Set/Add関数
	void SetPosition(VECTOR position) {
		_vPosition = VGet(position.x, position.y, position.z);
	}
	void AddPosition(float x, float y, float z) {
		_vPosition = VGet(_vPosition.x + x, _vPosition.y + y, _vPosition.z + z);
	}

	void SetScale(VECTOR scale) {
		_vScale = VGet(scale.x, scale.y, scale.z);
	}
	void AddScale(float x, float y, float z) {
		_vScale = VGet(_vScale.x + x, _vScale.y + y, _vScale.z + z);
	}

	void SetDirection(VECTOR direction) {
		_vDirection = VNorm(VGet(direction.x, direction.y, direction.z));
		//_vDirection = VGet(direction.x, direction.y, direction.z);
	}
	void AddDirection(float x, float y, float z) {
		_vDirection = VNorm(VGet(_vDirection.x + x, _vDirection.y + y, _vDirection.z + z));
	}

	VECTOR GetPosition() const {
		return _vPosition;
	}
	VECTOR GetScale() const {
		return _vScale;
	}
	VECTOR GetDirection() const {
		return _vDirection;
	}
	MATRIX GetTransform() const {
		return _mTransform;
	}

private:
	VECTOR _vPosition;  //場所
	VECTOR _vScale;  // 拡大縮小
	VECTOR _vBaseDirection;  //最初の向き(モデルの作りによって違う)
	VECTOR _vDirection;  //向き
	MATRIX _mTransform;  //変換行列
};