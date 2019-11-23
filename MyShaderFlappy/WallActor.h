// ----------------------------------------------------------------
// フラッピーバード用の壁の制御プログラム
// create by Satoru Inukai
//
//一定時間横に動いてデストロイする
//
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class WallActor : public Actor
{
public:
    WallActor(class Game* game);
    ~WallActor();

    void UpdateActor(float deltaTime) override;
    class BoxComponent* GetBox() { return mBox; }
    //void ActorInput(const uint8_t* keys) override;
private:
    class MoveComponent* mMoveComp;
    float lifeSpan;
    float forwardSpeed;
    class BoxComponent* mBox;
};
