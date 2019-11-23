// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//-----------------------------------------------------------------
//3Dメッシュを描画するためのコンポーネント
//とはいえ，描画処理のほとんどはシェーダーで行うため，
//このクラスでは使用するテクスチャやシェーダーを指定する
//-----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	// このメッシュコンポーネントを指定のシェーダーで描画する
	virtual void Draw(class Shader* shader);
	// mesh/textureの設定
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};
