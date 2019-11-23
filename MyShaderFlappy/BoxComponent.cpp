//-------------------------------------------------------
//BoxComponentクラス
//アクターにAABBを追加するコンポーネントだ
//つまり，箱型の衝突判定をオブジェクトに付与できる
//実際の衝突判定はコリジョンで行う
//create by Satoru Inukai(ゲームプログラミングC++を参考)
//-------------------------------------------------------

#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

BoxComponent::BoxComponent(Actor* owner, int updateOrder)
    :Component(owner, updateOrder)
    ,mObjectBox(Vector3::Zero, Vector3::Zero)
    ,mWorldBox(Vector3::Zero, Vector3::Zero)
    ,mShouldRotate(true)
{
    mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent()
{
    mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform()
{
    // Boxのオブジェクト空間の初期化
    mWorldBox = mObjectBox;
    //　スケール
    mWorldBox.mMin *= mOwner->GetScale();
    mWorldBox.mMax *= mOwner->GetScale();
    //回転
    if(mShouldRotate)
    {
        mWorldBox.Rotate(mOwner->GetRotation());
    }
    // 正規化
    mWorldBox.mMin += mOwner->GetPosition();
    mWorldBox.mMax += mOwner->GetPosition();
}


