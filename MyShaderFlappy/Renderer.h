// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//-----------------------------------------------------------------
//レンダラークラス
//描画処理用のクラス．もともとはGame内で行っていたが，流石に処理が多いためクラス化
//3Dメッシュの描画にしよう
//-----------------------------------------------------------------

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"

struct DirectionalLight
{
	// Direction of light
	Vector3 mDirection;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Specular color
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
    //レンダラーの初期化と終了処理
	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
    //テクスチャとメッシュを開放
	void UnloadData();
    //フレームの描画
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight0() { return mDirLight0; }
    DirectionalLight& GetDirectionalLight1() { return mDirLight1; }
    DirectionalLight& GetDirectionalLight2() { return mDirLight2; }
    DirectionalLight& GetDirectionalLight3() { return mDirLight3; }
    DirectionalLight& GetDirectionalLight4() { return mDirLight4; }
    
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
    
    int GetShaderFlag() const { return ShaderFlag; }
    void SetShaderFlag(int i) { ShaderFlag = i;}
private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetBasicLightUniforms(class Shader* shader);
    void SetPhoneLightUniforms(class Shader* shader);
    void SetToonUniforms(class Shader* shader);

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// All mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;
    

	// Game
	class Game* mGame;

	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// Meshシェーダー
	class Shader* mMeshShader;
    

	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;
	// Width/height of screen
	float mScreenWidth;
	float mScreenHeight;

	// ライティングデータ
	Vector3 mAmbientLight;
	DirectionalLight mDirLight0;
    DirectionalLight mDirLight1;
    DirectionalLight mDirLight2;
    DirectionalLight mDirLight3;
    DirectionalLight mDirLight4;
    
	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
    
    //切り替え用フラグ
    int ShaderFlag;
};
