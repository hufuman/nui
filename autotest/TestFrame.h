#pragma once


class TestFrame : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    int GetChildCount(NFrame* frame)
    {
        int count = 0;
        frame->EnumChilds(MakeDelegate(this, &TestFrame::EnumChildCount), reinterpret_cast<LPARAM>(&count));
        return count;
    }

    bool EnumChildCount(NFrame*, LPARAM lParam)
    {
        int* count = reinterpret_cast<int*>(lParam);
        ++ (*count);
        return true;
    }

protected:
};

TEST_F(TestFrame, AddRemoveChilds)
{
    NInstPtr<NFrame> parent1(InstPtrParam);
    NInstPtr<NFrame> parent2(InstPtrParam);
    NInstPtr<NFrame> child1(InstPtrParam);
    NInstPtr<NFrame> child2(InstPtrParam);

    NAssertTempDisable();

    parent1->AddChild(child1);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) >= 0);
    EXPECT_EQ(GetChildCount(parent1), 1);

    parent2->AddChild(child2);
    EXPECT_TRUE(parent2->GetChildZOrder(child2) >= 0);
    EXPECT_EQ(GetChildCount(parent2), 1);

    parent1->RemoveChild(child1);
    EXPECT_EQ(GetChildCount(parent1), 0);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) == -1);

    parent2->RemoveChild(child2);
    EXPECT_EQ(GetChildCount(parent2), 0);
    EXPECT_TRUE(parent1->GetChildZOrder(child2) == -1);
}

TEST_F(TestFrame, Parent)
{
    NInstPtr<NFrame> parent1(InstPtrParam);
    NInstPtr<NFrame> parent2(InstPtrParam);
    NInstPtr<NFrame> child1(InstPtrParam);
    NInstPtr<NFrame> child2(InstPtrParam);

    NAssertTempDisable();

    // add & remove
    parent1->AddChild(child1);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) >= 0);
    EXPECT_EQ(parent1, child1->GetParent());
    parent1->RemoveChild(child1);
    EXPECT_EQ(reinterpret_cast<NFrame*>(NULL), child1->GetParent());
    EXPECT_EQ(GetChildCount(parent1), 0);

    // add & change
    parent1->AddChild(child1);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) >= 0);
    EXPECT_EQ(parent1, child1->GetParent());
    parent1->RemoveChild(child1);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) == -1);
    EXPECT_EQ(reinterpret_cast<NFrame*>(NULL), child1->GetParent());

    // add & readd
    parent1->AddChild(child1);
    parent2->AddChild(child1);
    EXPECT_EQ(parent2, child1->GetParent());
    EXPECT_TRUE(parent1->GetChildZOrder(child1) == -1);
    EXPECT_TRUE(parent2->GetChildZOrder(child1) >= 0);

    parent1->AddChild(child1);
    EXPECT_EQ(parent1, child1->GetParent());
    EXPECT_TRUE(parent2->GetChildZOrder(child1) == -1);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) >= 0);
    parent1->RemoveChild(child1);
}

TEST_F(TestFrame, ZOrder)
{
    NInstPtr<NFrame> parent(InstPtrParam);
    NInstPtr<NFrame> child1(InstPtrParam);
    NInstPtr<NFrame> child2(InstPtrParam);

    NAssertTempDisable();

    parent->AddChild(child1);
    parent->AddChild(child2);
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    EXPECT_EQ(parent->GetChildZOrder(child2), 1);
    parent->AddChild(child1);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    parent->RemoveChild(child1);
    parent->RemoveChild(child2);

    // set zorder
    parent->AddChild(child1);
    parent->AddChild(child2);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->SetChildZOrder(child1, 0), 0);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    EXPECT_EQ(parent->SetChildZOrder(child1, 1), 1);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 1);
    EXPECT_EQ(parent->SetChildZOrder(child1, 9999), 1);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 1);

    // top most
    parent->AddChild(child1);
    parent->AddChild(child2);
    parent->SetChildTopmost(child1);
    EXPECT_EQ(2, GetChildCount(parent));
    parent->SetChildZOrder(child1, 0);
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    parent->SetChildZOrder(child1, 1);
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    parent->SetChildZOrder(child1, 999);
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);

    // bottom most
    parent->SetChildBottommost(child1);
    EXPECT_EQ(2, GetChildCount(parent));
    parent->SetChildZOrder(child1, 0);
    EXPECT_EQ(parent->GetChildZOrder(child1), 1);
    parent->SetChildZOrder(child1, 1);
    EXPECT_EQ(parent->GetChildZOrder(child1), 1);

    // 
    parent->SetChildTopmost(child1);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    parent->SetChildBottommost(child1);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 1);
    parent->SetChildTopmost(child1);
    EXPECT_EQ(2, GetChildCount(parent));
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);

    parent->RemoveAllChilds();
    EXPECT_EQ(0, GetChildCount(parent));
}
