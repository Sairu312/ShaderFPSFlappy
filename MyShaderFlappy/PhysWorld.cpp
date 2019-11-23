//-------------------------------------------------------
//PhysWorldクラス
//物理世界用のクラス
//Gameの初期化処理のときに初期化する
//ボックスコンポーネントでAddBoxされこのクラスがボックスコンポーネントの配列を持つ
//SegmentCastは最も近い交差を重要視している
//create by Satoru Inukai(ゲームプログラミングC++を参考)
//-------------------------------------------------------

#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include <SDL/SDL.h>

PhysWorld::PhysWorld(Game* game)
    :mGame(game)
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
    bool collided = false;
    // closestTは無限大で初期化
    // 最初の交差で更新
    float closestT = Math::Infinity;
    Vector3 norm;
    // すべてのボックスエージェントをテスト
    for (auto box : mBoxes)
    {
        float t;
        // 線分とボックスが交差するか
        if (Intersect(l, box->GetWorldBox(), t, norm))
        {
            // すでに入っている交差よりも近ければ代入
            if (t < closestT)
            {
                outColl.mPoint = l.PointOnSegment(t);
                outColl.mNormal = norm;
                outColl.mBox = box;
                outColl.mActor = box->GetOwner();
                collided = true;
            }
        }
    }
    return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f)
{
    // Naive implementation O(n^2)
    for (size_t i = 0; i < mBoxes.size(); i++)
    {
        // Don't need to test vs itself and any previous i values
        for (size_t j = i + 1; j < mBoxes.size(); j++)
        {
            BoxComponent* a = mBoxes[i];
            BoxComponent* b = mBoxes[j];
            if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
            {
                // Call supplied function to handle intersection
                f(a->GetOwner(), b->GetOwner());
            }
        }
    }
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
    // Sort by min.x
    std::sort(mBoxes.begin(), mBoxes.end(),
        [](BoxComponent* a, BoxComponent* b) {
            return a->GetWorldBox().mMin.x <
                b->GetWorldBox().mMin.x;
    });

    for (size_t i = 0; i < mBoxes.size(); i++)
    {
        // Get max.x for current box
        BoxComponent* a = mBoxes[i];
        float max = a->GetWorldBox().mMax.x;
        for (size_t j = i + 1; j < mBoxes.size(); j++)
        {
            BoxComponent* b = mBoxes[j];
            // If AABB[j] min is past the max bounds of AABB[i],
            // then there aren't any other possible intersections
            // against AABB[i]
            if (b->GetWorldBox().mMin.x > max)
            {
                break;
            }
            else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
            {
                f(a->GetOwner(), b->GetOwner());
            }
        }
    }
}

void PhysWorld::AddBox(BoxComponent* box)
{
    mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
    auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
    if (iter != mBoxes.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mBoxes.end() - 1);
        mBoxes.pop_back();
    }
}

