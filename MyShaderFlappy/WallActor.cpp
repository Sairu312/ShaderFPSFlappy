// ----------------------------------------------------------------
// フラッピーバード用の壁の制御プログラム
// create by Satoru Inukai
//
//一定時間横に動いてデストロイする
//
// ----------------------------------------------------------------

#include "WallActor.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

//アクターの生成
WallActor::WallActor(Game* game)
    :Actor(game)
,lifeSpan(10.0f)
{
    SetScale(10.0f);
    mMoveComp = new MoveComponent(this);
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Cube.gpmesh");
    mc->SetMesh(mesh);
    
    //コリジョンボックスの追加
    mBox = new BoxComponent(this);
    mBox->SetObjectBox(mesh->GetBox());
    
    game->AddWall(this);
}
WallActor::~WallActor()
{
    GetGame()->RemoveWall(this);
}

//アクターの更新
void WallActor::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);
    forwardSpeed = 50.0f;
    lifeSpan -= deltaTime;
    mMoveComp->SetForwardSpeed(forwardSpeed);
    ComputeWorldTransform();
    if(lifeSpan < 0.0f)SetState(EDead);
    
}
