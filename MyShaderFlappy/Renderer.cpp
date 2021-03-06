// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//-----------------------------------------------------------------
//レンダラークラス
//Gameの描画処理をすべて引き継いでいる
//
//-----------------------------------------------------------------

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include <GL/glew.h>
#include "Game.h"

Renderer::Renderer(Game* game)
	:mGame(game)
	,mSpriteShader(nullptr)
	,mMeshShader(nullptr)
    ,mToonShader(nullptr)
    ,mPhongShader(nullptr)
    ,ShaderFlag(0)
{
}

Renderer::~Renderer()
{
}

//初期化メソッド
bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// RGBAのメモリを確保
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //デプスバッファを確保する
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// ダブルバッファの有効化
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("ShaderFPSFlappy", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEWの初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
    mPhongShader->Unload();
    delete mPhongShader;
    mToonShader->Unload();
    delete mToonShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}


//描画メソッド　Gameからはこの関数が呼び出される
void Renderer::Draw()
{
	// 背景色でクリア
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// カラーバッファとデプスバッファのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw mesh components
	// デプスバッファ法の有効化
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// Set the mesh shader active
    for(auto mc : mMeshComps)
    {
        if(mc->GetGroup() == 2)
        {
            if(ShaderFlag == 0)
            {
                mMeshShader->SetActive();
                // Update view-projection matrix
                mMeshShader->SetMatrixUniform("uViewProj",
                                              mView * mProjection);
                // Update lighting uniforms
                SetBasicLightUniforms(mMeshShader);
                mc->Draw(mMeshShader);
            }else{
                mPhongShader->SetActive();
                // Update view-projection matrix
                mPhongShader->SetMatrixUniform("uViewProj",
                                               mView * mProjection);
                // Update lighting uniforms
                SetPhongLightUniforms(mPhongShader);
                mc->Draw(mPhongShader);
            }
        }else{
            mMeshShader->SetActive();
            // Update view-projection matrix
            mMeshShader->SetMatrixUniform("uViewProj",
                                          mView * mProjection);
            // Update lighting uniforms
            SetBasicLightUniforms(mMeshShader);
            mc->Draw(mMeshShader);
        }
    }
    
      
      
	// Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// シェーダーと頂点配列の有効化
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
    if(!mGame->GetStartFlag())
    {
        for (auto sprite : mSprites)
        {
            sprite->Draw(mSpriteShader);
        }
    }

	// バッファーの入れ替え
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

//メッシュデータの読み込みメソッド
Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders()
{
	// スプライトシェーダーの生成
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// メッシュ用シェーダーの生成
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
    
    mPhongShader = new Shader();
    if(!mPhongShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    mPhongShader->SetActive();
    
    
    mToonShader = new Shader();
    if(!mToonShader->Load("Shaders/Toon.vert", "Shaders/Toon.frag"))
    {
        return false;
    }
    mToonShader->SetActive();
     
    
    
    
	// ビューと射影行列を設定
	mView = Matrix4::CreateLookAt(
            Vector3::Zero,
            Vector3::UnitX,
            Vector3::UnitZ
            );
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

//Basicメッシュ用ライティング
void Renderer::SetBasicLightUniforms(Shader* shader)
{
	// Camera position is from inverted view
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    /*
	// 環境光
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// 平行光構造体
	shader->SetVectorUniform("uDirLight.mDirection",
                             mDirLight0.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
                             mDirLight0.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
                             mDirLight0.mSpecColor);
     */
}

//Phongシェーダー用ライティング
void Renderer::SetPhongLightUniforms(Shader* shader)
{
    // Camera position is from inverted view
    Matrix4 invView = mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // 環境光
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    // 平行光構造体
    shader->SetVectorUniform("uDirLight.mDirection",
                             mDirLight0.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor",
                             mDirLight0.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor",
                             mDirLight0.mSpecColor);
}

void Renderer::SetToonUniforms(Shader* shader)
{
    Matrix4 invView = mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // 環境光
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    // 平行光構造体
    shader->SetVectorUniform("uDirLight.mDirection",
                             mDirLight0.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor",
                             mDirLight0.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor",
                             mDirLight0.mSpecColor);
    
    
}
