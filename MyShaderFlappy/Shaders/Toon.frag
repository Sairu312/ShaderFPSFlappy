//---------------------------------------------------------
//トゥーンレンダリングシェーダー
//Create by Satoru Inukai
//---------------------------------------------------------

// GLSL 3.3
#version 330

// 頂点シェーダーから
in vec2 fragTexCoord;
// 法線
in vec3 fragNormal;
// 座標
in vec3 fragWorldPos;

// 出力の色
out vec4 outColor;

// テクスチャ
uniform sampler2D uTexture;

// 平行光源用構造体
struct DirectionalLight
{
    // 光の方向
    vec3 mDirection;
    // 拡散反射色
    vec3 mDiffuseColor;
    // 鏡面反射色
    vec3 mSpecColor;
};

// ライティングのuniform
// カメラポジション
uniform vec3 uCameraPos;
// スペキュラー
uniform float uSpecPower;
// 環境光成分
uniform vec3 uAmbientLight;

// 平行光源
uniform DirectionalLight uDirLight;

void main()
{
    // 表面の法線
    vec3 N = normalize(fragNormal);
    // 光源ベクトル
    vec3 L = normalize(-uDirLight.mDirection);
    // 視線ベクトル
    vec3 V = normalize(uCameraPos - fragWorldPos);
    // 法線Nで反射した光源ベクトル
    vec3 R = normalize(reflect(-L, N));

    // フォンシェーディングを計算
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
        Phong += Diffuse + Specular;
    }

    // 最終的な色はテクスチャの色xフォンの光(alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
