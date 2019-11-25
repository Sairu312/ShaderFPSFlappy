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

#include "Shader.h"
#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{
	
}

Shader::~Shader()
{

}

//コンパイルのリンクを行う関数
bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// 頂点シェーダーとフラグメントシェーダーのコンパイル
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		mVertexShader) ||
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			mFragShader))
	{
		return false;
	}
	
	//シェーダープログラムをリンクしてシェーダープログラムを作る
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);
	
	// プログラムが正しくリンクされたか
	if (!IsValidProgram())
	{
		return false;
	}
	
	return true;
}

void Shader::Unload()
{
	// シェーダーを削除する
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// シェーダープログラムをアクティブにする
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// Find the uniform by this name
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the matrix data to the uniform
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the vector data
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the float data
	glUniform1f(loc, value);
}


void Shader::SetBoolUniform(const char* name, bool value)
{
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniform1f(loc, value);
}

void Shader::SetTextureUniform(const char* name, std::string& fileName)
{
    Texture* tex = new Texture();
    tex->Load(fileName);
}



//シェーダーコンパイル関数
bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// ファイルを開く
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// すべてのテキストを1つの文字列に読み込む
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		
		// 指定されたタイプのシェーダーを作成
		outShader = glCreateShader(shaderType);
		// 読み込んだ文字列でのコンパイルを試みる
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);
		
		if (!IsCompiled(outShader))
		{
			SDL_Log("シェーダー %s　のコンパイルに失敗しました", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("シェーダーファイル %s　が見つかりません", fileName.c_str());
		return false;
	}
	
	return true;
}

//コンパイルが成功したか
bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// コンパイル状態を問い合わせる
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("コンパイルに失敗！:\n%s", buffer);
		return false;
	}
	
	return true;
}

bool Shader::IsValidProgram()
{
	
	GLint status;
	// Query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}
	
	return true;
}
