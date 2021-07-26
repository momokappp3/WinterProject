/*****************************************************************//**
 * \file   Camera.h
 * \brief  ÉJÉÅÉâä÷êî
 * \author momoka
 * \date   2021 7/22
 *********************************************************************/
#pragma once
#include <string>
#include "../../AppFrame/source/appframe.h"
#include "Transform.h"

#define PI 3.1415926535897932386f;
#define	DEG2RAD (x)(((x) / 180.0f ) * PI );
#define	RAD2DEG (x)(((x) * 180.0f ) / PI );

class Camera {
public:
	Camera();
	virtual ~Camera();

	void SetPosition(float x, float y, float z) {
		_vPos = VGet(x, y, z);
	}
	void SetTarget(float x, float y, float z) {
		_vTarget = VGet(x, y, z);
	}

	void SetNearFar(float cNear, float cFar) {
		_clipNear = cNear;
		_clipFar = cFar;
	}

	VECTOR GetPosition() {
		return _vPos;
	}

	VECTOR GetTarget() {
		return _vTarget;
	}

	void Process();
	void Render();  //debugéû

	Transform& GetTransform() {
		return _transform;
	}

	VECTOR	_vPos;					// à íu
	VECTOR	_vTarget;				// ãóó£

	float _clipNear;
	float _clipFar;

	Transform _transform;
};