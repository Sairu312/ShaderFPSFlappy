//-------------------------------------------------------
//BoxComponentクラス
//アクターにAABBを追加するコンポーネントだ
//つまり，箱型の衝突判定をオブジェクトに付与できる
//実際の衝突判定はコリジョンで行う
//create by Satoru Inukai(ゲームプログラミングC++を参考)
//-------------------------------------------------------
#pragma once
#include "Component.h"
#include "Collision.h"

class BoxComponent : public Component
{
public:
    BoxComponent(class Actor* owner, int updateOrder = 100);
    ~BoxComponent();
    
    void OnUpdateWorldTransform() override;
    
    void SetObjectBox(const AABB& model) { mObjectBox = model; }
    //
    const AABB& GetWorldBox() const {return mWorldBox; }
    
    void SetShouldRotate(bool value) { mShouldRotate = value; }
private:
    AABB mObjectBox;
    AABB mWorldBox;
    bool mShouldRotate;
};
