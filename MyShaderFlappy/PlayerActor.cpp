
#include "PlayerActor.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "MeshComponent.h"
#include "SDL/SDL_scancode.h"
#include "BoxComponent.h"
#include "WallActor.h"
#include "PlaneActor.h"
#include "Mesh.h"
#include <stdio.h>


//アクターの生成
PlayerActor::PlayerActor(Game* game)
    :Actor(game)
,mJumpPower(60.0f)
,mCount(0)
,mLast(false)
,mNow(false)
{
    mMoveComp = new MoveComponent(this);
    SetScale(10.0f);
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Cube.gpmesh");
    mc->SetMesh(mesh);

    //boxComponentの追加
    mBoxComp = new BoxComponent(this);
    mBoxComp->SetObjectBox(mesh->GetBox());
    //mBoxComp->SetShouldRotate(false);
}

//アクターの更新
void PlayerActor::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);
    FixCollisions();
    
    mTimeCount += deltaTime;
    mGravity = -5.0f * mTimeCount;
    mMoveSpeed += mGravity;
    if(!GetGame()->GetStartFlag())mMoveSpeed = 0.0f;
    if(mMoveSpeed < -0.1f && GetPosition().z < -80.0f)
    {
        mMoveSpeed = 0.0f;
        //SwapShaderFlag();
        SetPosition(Vector3(200.0f,-75.0f,0.0f));
        GetGame()->SetStartflag(false);
    }
    mMoveComp->SetUpperSpeed(mMoveSpeed);
    
    //カメラの位置
    Vector3 cameraPos = GetPosition();
    //ターゲット方向
    Vector3 target = GetPosition() + GetForward() * 100.0f;
    
    Vector3 up = Vector3::UnitZ;

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    GetGame()->GetRenderer()->SetViewMatrix(view);
}

void PlayerActor::ActorInput(const uint8_t* keys)
{
    if(keys[SDL_SCANCODE_SPACE])
    {
        mMoveSpeed = mJumpPower;
        mTimeCount = 0.0f;
    }
}


//AABB同士の衝突判定，今回はWallActorと
void PlayerActor::FixCollisions()
{
    //ワールド空間のボックスを更新するために，
    //自分のワールド変換を再計算する必要がある
    ComputeWorldTransform();
    
    const AABB& playerBox = mBoxComp->GetWorldBox();
    
    auto& walls = GetGame()->GetWalls();
    
    for(auto wa : walls)
    {
        //このWallActorと衝突するか？
        const AABB& wallBox = wa->GetBox()->GetWorldBox();
        if(Intersect(playerBox, wallBox))
        {
            if(!mLast){
                SwapShaderFlag();
            }
            mNow = true;
        }
    }
    if(mNow)mLast = true;
    else mLast = false;
    mNow = false;
}

void PlayerActor::SwapShaderFlag()
{
    int i = GetGame()->GetRenderer()->GetShaderFlag();
    if(i==0)
    {
        i = 1;
    }
    else {
        i = 0;
    }
    GetGame()->GetRenderer()->SetShaderFlag(i);
}
