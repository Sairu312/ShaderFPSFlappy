// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//シェーダーの読み込み用クラス
//1.頂点シェーダーをロードしてコンパイル
//2.フラグメントシェーダーをロードしてコンパイル
//3.2つのシェーダーをリンクして「シェーダープログラム」にする


#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"
//#include "Texture.h"

class Shader
{
public:
	Shader();
	~Shader();
	// 指定された名前の頂点シェーダー/フラグメントシェーダーを読み込む
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	// アクティブなシェーダープログラムとして設定する
	void SetActive();
	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	// Sets a Vector3 uniform
	void SetVectorUniform(const char* name, const Vector3& vector);
	// Sets a float uniform
	void SetFloatUniform(const char* name, float value);
    
    //　ブール変数のセット
    void SetBoolUniform(const char* name, bool value);
    //　テクスチャ変数のセット
    void SetTextureUniform(const char* name, std::string& fileName);
private:
	// シェーダーをコンパイルする
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	
	// シェーダーがコンパイルされたか？
	bool IsCompiled(GLuint shader);
	// シェーダープログラムのリンクを確認
	bool IsValidProgram();
private:
	// シェーダーオブジェクトのID
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
