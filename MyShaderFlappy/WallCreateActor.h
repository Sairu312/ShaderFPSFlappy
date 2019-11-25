//-------------------------------------------------------
//WallCreaterクラス
//今までGame.cpp内でWallアクターを追加したが
//Game内に直接処理を書くのは管理能力が落ちてしまうため
//Wallを自動生成するためのクラスを用意
//create by Satoru Inukai
//-------------------------------------------------------

#pragma once
#include "Actor.h"

class WallCreateActor : public Actor
{
public:
    WallCreateActor(class Game* game);
    ~WallCreateActor();

    void UpdateActor(float deltaTime) override;
//    class BoxComponent* GetBox() { return mBox; }
    void ActorInput(const uint8_t* keys) override;
private:
    bool mStartFlag;
    float mTimeCount;
};
