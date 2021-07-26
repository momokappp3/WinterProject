/*****************************************************************//**
 * \file   ModeOption.h
 * \brief  ç°ÇÃÇ∆Ç±ÇÎSoundí≤êﬂÇÃÇ›
 * \author momoka
 * \date   2021 7/23
 * \details 0Å`255ÇÃâπó ÇÃí≤êﬂÉ}ÉEÉXÇÃì¸óÕ
 *********************************************************************/
#pragma once
#include "appframe.h"
#include <string>
#include <vector>

class ModeOption : public ModeBase {
public:
	ModeOption();
	virtual ~ModeOption();
	virtual bool Initialize();
	virtual bool Process();
	virtual bool Render();
	virtual bool Terminate();

protected:
	int	_backHandle;
	//int	_curAnimCnt;
};