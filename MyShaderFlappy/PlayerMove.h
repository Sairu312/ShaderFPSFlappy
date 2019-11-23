//-------------------------------------------------------
//PlayerMoveクラス
//プレイヤーの運動を司るクラス
//create by Satoru Inukai(ゲームプログラミングC++を参考)
//-------------------------------------------------------

#pragma once
#include "MoveComponent.h"

class PlayerMove : public MoveComponent
{
public:
    PlayerMove(class Actor* owner);
    
    void Update(float daltaTime) override;
}
