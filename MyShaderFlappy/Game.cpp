// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//-----------------------------------------------------------------
//ここが中核となるGameクラス
//
//
//-----------------------------------------------------------------
#include <iostream>

#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "PlaneActor.h"
#include "WallActor.h"
#include "PlayerActor.h"
#include <stdio.h>
#include "PhysWorld.h"
#include "WallCreateActor.h"

Game::Game()
:mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
,mPhysWorld(nullptr)
,mStartFlag(false)
{
	
}

//初期化メソッド
bool Game::Initialize()
{
    //SDLライブラリの初期化
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//レンダラーの生成
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}
    
    //PhysWorldの生成
    mPhysWorld = new PhysWorld(this);
    

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

//ループメソッド
//入力→処理→出力を繰り返す
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::AddPlane(PlaneActor* plane)
{
    mPlanes.emplace_back(plane);
}

void Game::AddWall(WallActor* wall)
{
    mWalls.emplace_back(wall);
}


void Game::RemovePlane(PlaneActor* plane)
{
    auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
    mPlanes.erase(iter);
}

void Game::RemoveWall(WallActor* wall)
{
    auto iter = std::find(mWalls.begin(), mWalls.end(), wall);
    mWalls.erase(iter);
}



//入力メソッド
void Game::ProcessInput()
{
    //入力はSDLイベントシステムを使用
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

//処理メソッド
//処理のメソッドではあるがアクターの管理をするだけなので
//具体的な動きは各オブジェクトのアクターで記述
void Game::UpdateGame()
{
	// deltaTimeの作成
	// Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
    

	// すべてのアクターを更新する
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待ち状態のアクターをmActorsに移動
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを消す
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

//描画処理　レンダラーに一任
void Game::GenerateOutput()
{
	mRenderer->Draw();
}


//ゲームに必要なデータのロード
//Actorを作成し，位置回転を入れMeshComponentでMeshを作成する
void Game::LoadData()
{
	// アクターの作成
	Actor* a = new Actor(this);
    /*
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
    */
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
    
    
    a = new PlayerActor(this);
    a->SetPosition(Vector3(200.0f,-75.0f,0.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ,Math::Pi));
    
    a = new WallCreateActor(this);
    
    
    /*
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
    //MeshComponentの宣言時にAddMeshCompにコンポーネントが入れられるため
    //mcを再び初期化しても問題がない
     */
//	MeshComponent* mc = new MeshComponent(a);
//    mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));
    /*
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
    
    a = new WallActor(this);
    a->SetPosition(Vector3(0.0f, -75.0f, 0.0f));
    mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));
*/
	// 床の設定
    //同じ形状のものを10x10個配置する
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// 左右の壁　2x10個並べる
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);
		
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// 前後の壁　2x10個並べる
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}
    //ライトのセッティング
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight0();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.8f, 0.8f, 0.8f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);
    
    //mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir1 = mRenderer->GetDirectionalLight1();
    dir1.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir1.mDiffuseColor = Vector3(0.0f, 1.0f, 0.0f);
    dir1.mSpecColor = Vector3(0.5f, 1.0f, 0.5f);

    //mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir2 = mRenderer->GetDirectionalLight2();
    dir2.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir2.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir2.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);
    
    //mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir3 = mRenderer->GetDirectionalLight3();
    dir3.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir3.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir3.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);
    
    //mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir4 = mRenderer->GetDirectionalLight4();
    dir4.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir4.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir4.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    
	// カメラアクター
	//mCameraActor = new CameraActor(this);
/*
	// UI要素
	a = new Actor(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor(this);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
 */
    a = new Actor(this);
    a->SetPosition(Vector3(60.0f, 0.0f, 0.0f));
    SpriteComponent* sc = new SpriteComponent(a);
    sc->SetTexture(mRenderer->GetTexture("Assets/Title.png"));
}

void Game::Shutdown()
{
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	SDL_Quit();
}

void Game::UnloadData()
{
    // アクターを消す
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    if (mRenderer)
    {
        mRenderer->UnloadData();
    }
}

//アクターの追加
void Game::AddActor(Actor* actor)
{
	// アクターが更新中でも新しいアクターが追加できるように
    //一旦待ちアクターに追加する．
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}
