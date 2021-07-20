#include "AppFrame.h"
#include "ApplicationMain.h"
#include "../source/Mode/ModeTitle.h"
#include "Mode/RoomGame.h"
#include "Mode/Action3DGame.h"

ApplicationMain	g_oApplicationMain;  // ŽÀ‘Ì

bool ApplicationMain::Initialize(HINSTANCE hInstance) {

	if (!ApplicationBase::Initialize(hInstance)) {
		return false;
	}

	// Å‰‚Ìƒ‚[ƒh‚Ì“o˜^
	//ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	//ModeServer::GetInstance()->Add(new RoomGame(), 1, "RoomGame");
	ModeServer::GetInstance()->Add(new Action3DGame(), 2, "Action");

	return true;
}

bool ApplicationMain::Input() {
	ApplicationBase::Input();
	return true;
}

bool ApplicationMain::Process() {
	ApplicationBase::Process();
	return true;
}

bool ApplicationMain::Render() {
	ApplicationBase::Render();
	return true;
}

bool ApplicationMain::Terminate() {
	ApplicationBase::Terminate();
	return true;
}