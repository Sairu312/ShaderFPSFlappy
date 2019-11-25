
#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
    PlayerActor(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keys) override;
    void SwapShaderFlag();
    
    void FixCollisions();
private:
    class MoveComponent* mMoveComp;
    class BoxComponent* mBoxComp;
    
    float mJumpPower;
    float mGravity;
    float mMoveSpeed;
    float mTimeCount;
    int mCount;
    bool mLast;
    bool mNow;
    
};
