// ----------------------------------------------------------------
// Phongシェーダ用頂点シェーダー
// 環境光，拡散反射，鏡面反射の合成で出来上がる
//
// Create by Satoru Inukai(ゲームプログラミングC++を参考)
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// ワールド変換とビュー変換
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 頂点と法線とテクスチャ
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// テクスチャ座標
out vec2 fragTexCoord;
// ワールド法線
out vec3 fragNormal;
// 頂点のワールド座標
out vec3 fragWorldPos;

void main()
{
	// 同次座標系変換
	vec4 pos = vec4(inPosition, 1.0);
	// ワールド空間変換
	pos = pos * uWorldTransform;
	//ワールド空間の位置を保存
	fragWorldPos = pos.xyz;
	// クリップ変換
	gl_Position = pos * uViewProj;

	// 法線のワールド変換
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	//テクスチャ座標の受け渡し
	fragTexCoord = inTexCoord;
}
