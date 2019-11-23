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



#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

//MeshComponentの宣言
//宣言時にGameのレンダラーが持つAddMeshCompに自身を入れる
MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ワールド座標変換の設定
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// 
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
        // アクティブテクスチャの設定
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// メッシュの頂点配列をアクティブ
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// 描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
