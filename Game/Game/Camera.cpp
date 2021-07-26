#include "Camera.h"
#include "source/ApplicationMain.h"

Camera::Camera() {  //0を入れる

	_vPos = VGet(0.0f, 0.0f, 0.0f);
	_vTarget = VGet(-12.230986f, 59.101776f, -15.002045f);
	_clipNear = 0.0f;
	_clipFar = 0.0f;

}

Camera::~Camera() {
}

void Camera::Process() {

	_transform.Update();
	SetCameraViewMatrix( _transform.GetTransform());

	//カメラの角度はModeで変更しないとバグる

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_vPos, _vTarget);
	SetCameraNearFar(_clipNear, _clipFar);

}

//debug情報
void Camera::Render() {
	/*
	int x = 0, y = 0, size = 16;

	SetFontSize(size);
	DrawFormatString(x, y, GetColor(255, 0, 0), "Camera:");y += size;
	DrawFormatString(x, y, GetColor(255, 0, 0), "  target = (%5.2f, %5.2f, %5.2f)", _vTarget.x, _vTarget.y, _vTarget.z);y += size;
	DrawFormatString(x, y, GetColor(255, 0, 0), "  pos    = (%5.2f, %5.2f, %5.2f)", _vPos.x, _vPos.y, _vPos.z);y += size;

	float sx = _vPos.x - _vTarget.x;
	float sz = _vPos.z - _vTarget.z;
	float length = sqrt(sz * sz + sx * sx);
	float rad = atan2(sz, sx);
	float deg = RAD2DEG(rad);

	DrawFormatString(x, y, GetColor(255, 0, 0), "  len = %5.2f, rad = %5.2f, deg = %5.2f", length, rad, deg);y += size;
	*/
}