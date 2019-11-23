//-------------------------------------------------------
//Collisionクラス
//衝突判定関連の計算ライブラリ
//衝突判定オブジェクトが使用する
//
//create by Satoru Inukai(ゲームプログラミングC++を参考)
//-------------------------------------------------------

#pragma once
#include "Math.h"
#include <vector>

struct LineSegment
{
    //線分
    LineSegment(const Vector3& start, const Vector3& end);
    // 線分上の点の位置
    Vector3 PointOnSegment(float t) const;
    // 線分と点の距離
    float MinDistSq(const Vector3& point) const;
    // 線分同士の距離
    static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

    Vector3 mStart;
    Vector3 mEnd;
};

struct Plane
{
    //平面
    Plane(const Vector3& normal, float d);
    // 3点を含む平面
    Plane(const Vector3& a, const Vector3& b, const Vector3& c);
    // 平面と点の距離
    float SignedDist(const Vector3& point) const;

    Vector3 mNormal;
    float mD;
};

//球のバウンディングボリューム
struct Sphere
{
    Sphere(const Vector3& center, float radius);
    bool Contains(const Vector3& point) const;

    Vector3 mCenter;
    float mRadius;
};

//軸平行バウンディングボックス
struct AABB
{
    AABB(const Vector3& min, const Vector3& max);
    // Update min and max accounting for this point
    // (used when loading a model)
    void UpdateMinMax(const Vector3& point);
    // Rotated by a quaternion
    void Rotate(const Quaternion& q);
    bool Contains(const Vector3& point) const;
    float MinDistSq(const Vector3& point) const;

    Vector3 mMin;
    Vector3 mMax;
};

//有向バウンディングボックス
struct OBB
{
    Vector3 mCenter;
    Quaternion mRotation;
    Vector3 mExtents;
};

//カプセル
struct Capsule
{
    Capsule(const Vector3& start, const Vector3& end, float radius);
    // カプセル内の線分の位置t
    Vector3 PointOnSegment(float t) const;
    bool Contains(const Vector3& point) const;

    LineSegment mSegment;
    float mRadius;
};

//凸ポリゴン
struct ConvexPolygon
{
    bool Contains(const Vector2& point) const;
    std::vector<Vector2> mVertices;
};

// Intersection functions
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& s, const AABB& box);

bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const AABB& b, float& outT,
    Vector3& outNorm);

bool SweptSphere(const Sphere& P0, const Sphere& P1,
    const Sphere& Q0, const Sphere& Q1, float& t);
