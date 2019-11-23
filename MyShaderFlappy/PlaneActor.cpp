// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// ----------------------------------------------------------------
// このプロジェクトでは壁として使用
// こちらも宣言時にMeshをセットする
//
//コリジョン追加前まではmcに直接メッシュを入れてたが，
//コリジョン用にメッシュクラスも入れる
// ----------------------------------------------------------------

#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);
    
    //コリジョンボックスの追加
    mBox = new BoxComponent(this);
    mBox->SetObjectBox(mesh->GetBox());
    
    game->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
    GetGame()->RemoveActor(this);
}
