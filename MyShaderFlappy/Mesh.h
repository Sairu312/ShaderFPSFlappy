// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
//-----------------------------------------------------------------
//メッシュヘッダー
//メッシュ類のデーターを取得して格納する
//境界球とはオブジェクトの中心から一番遠い頂点までを半径とした球体
//-----------------------------------------------------------------
#pragma once
#include <vector>
#include <string>
#include "Collision.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	// メッシュデータのLoad/unload
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// 頂点配列の取得
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// テクスチャを取得
	class Texture* GetTexture(size_t index);
	// Get name of shader
	const std::string& GetShaderName() const { return mShaderName; }
	// シェーダー名を取得
	float GetRadius() const { return mRadius; }
    // AABBの取得
    const AABB& GetBox() const {return mBox; }
	// オブジェクト空間での境界球の半径の取得
	float GetSpecPower() const { return mSpecPower; }
private:
    //AABB用変数
    AABB mBox;
	// テクスチャ群の配列
	std::vector<class Texture*> mTextures;
	// 頂点配列
	class VertexArray* mVertexArray;
	// シェーダーの名前
	std::string mShaderName;
	// オブジェクト空間での境界球の半径を記録
	float mRadius;
	// Specular power of surface
	float mSpecPower;
};
