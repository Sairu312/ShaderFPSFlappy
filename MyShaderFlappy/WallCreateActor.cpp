//-------------------------------------------------------
//WallCreaterクラス
//今までGame.cpp内でWallアクターを追加したが
//Game内に直接処理を書くのは管理能力が落ちてしまうため
//Wallを自動生成するためのクラスを用意
//create by Satoru Inukai
//-------------------------------------------------------

#include "WallCreateActor.h"
#include "Game.h"
#include "WallActor.h"
//#include "MeshComponent.h"
#include "Renderer.h"
#include "SDL/SDL_scancode.h"
#include <stdio.h>

//アクターの生成
WallCreateActor::WallCreateActor(Game* game)
    :Actor(game)
,mTimeCount(0.0f)
,mStartFlag(false)
{
    //SetScale(10.0f);
}

WallCreateActor::~WallCreateActor()
{
    GetGame()->RemoveActor(this);
}

//アクターの更新
void WallCreateActor::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);
    
    mTimeCount += deltaTime;
    
    if(mTimeCount > 2.0f && GetGame()->GetStartFlag())
    {
        float i = rand()/50000000;
        WallActor* a = new WallActor(GetGame());
        a->SetPosition(Vector3(-50.0f, -75.0f, i));
        
        a = new WallActor(GetGame());
        a->SetPosition(Vector3(-50.0f, -75.0f, i + 50.0f));
        
        mTimeCount = 0.0f;
    }
}

void WallCreateActor::ActorInput(const uint8_t* keys)
{
    if(keys[SDL_SCANCODE_SPACE] && !GetGame()->GetStartFlag())
    {
        GetGame()->SetStartflag(true);
        mTimeCount = 0.0f;
    }
}
