//-------------------------------------------------------
//PhysWorldクラス
//物理世界用のクラス
//Gameの初期化処理のときに初期化する
//ボックスコンポーネントでAddBoxされこのクラスがボックスコンポーネントの配列を持つ
//SegmentCastは最も近い交差を重要視している
//create by Satoru Inukai(ゲームプログラミングC++を参考)
//-------------------------------------------------------

#pragma once
#include <vector>
#include <functional>
#include "Math.h"
#include "Collision.h"

class PhysWorld
{
public:
    PhysWorld(class Game* game);
    
    //衝突したときの状態を格納する構造体
    struct CollisionInfo
    {
        //衝突した点
        Vector3 mPoint;
        //衝突した点の法線
        Vector3 mNormal;
        //交差したコンポーネント
        class BoxComponent* mBox;
        // コンポーネントを所有するアクター
        class Actor* mActor;
    };

    //線分キャスト関数
    //線分を受け取り，線分とボックスが衝突するならtrueを返し，コリジョン情報を参照で返す．
    bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

        // Tests collisions using naive pairwise
    void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
        // Test collisions using sweep and prune
    void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

    //ワールドにボックスコンポーネントを追加/削除
    //これによってワールド内のすべてのボックスコンポーネントを把握する
    void AddBox(class BoxComponent* box);
    void RemoveBox(class BoxComponent* box);
private:
    class Game* mGame;
    std::vector<class BoxComponent*> mBoxes;
};
