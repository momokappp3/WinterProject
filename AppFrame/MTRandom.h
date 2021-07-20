/*****************************************************************
 * \file   MTRandom.h
 * \brief  最小から最大の指定した数字を返す
 * 　　　　trueとfalseをランダムに返す　　　
 * \author momoka
 * \date   July 2021
 *********************************************************************/
#pragma once
#include <random>

class MTRandom {
public:
	MTRandom();
	virtual ~MTRandom();

	static int Get(int min, int max);  //指定した数値の中でランダムを返す(int)

	static bool Get() {
		return Get(1, 2) == 1 ? true : false;
	}

	static std::mt19937_64 GetRandom();
};