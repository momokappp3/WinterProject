/*****************************************************************//**
 * \file   PlayerInfo.h
 * \brief  共有するプレイヤー情報
 * \author momoka
 * \date   2021 7/9
 *********************************************************************/
#pragma once

class PlayerInfo{
public:
    PlayerInfo();
    virtual ~PlayerInfo();

    void Process();

    //Get関数
    int GetFavor() {
        return _favor / 100;
    }

    int GetMolecule() {
        return _favor % 100;
    }
    int GetCoin() {
        return _coin;
    }
    int GetMentalNum() {
        return _mentalNum;
    }

    int GetTotalFavor() {
        return _favor;
    }

    //足す関数引く関数
    void SetFavor(int num, bool plus) {
        if (plus) {
            _favor += num;
        }
        else {
            _favor -= num;
        }
    }

    void SetMolecule(int num, bool plus) {
        if (plus) {
            _molecule += num;
        }
        else {
            _molecule -= num;
        }
    }

    void SetCoin(int num, bool plus) {

        if (plus) {
            _coin += num;
        }
        else{
            _coin -= num;
        }
    }

    void SetMentalNum(int num, bool plus) {

        if (plus) {
            _mentalNum += num;
        }
        else {
            _mentalNum -= num;
        }
    }

private:
    int _favor;  //好感度(num)
    int _molecule;  //好感度分子(ber)
    int _coin;
    int _mentalNum;  //赤色のbar
};