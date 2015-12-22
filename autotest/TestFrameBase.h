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

    bool EnumChildCount(NFrameBase*, LPARAM lParam)
    {
        int* count = reinterpret_cast<int*>(lParam);
        ++ (*count);
        return true;
    }

protected:
};

TEST_F(TestFrame, FrameTree)
{
    NInstPtr<NFrame> level1(MemToolParam);
    NInstPtr<NFrame> level2(MemToolParam);
    NInstPtr<NFrame> level3(MemToolParam);
    NInstPtr<NFrame> level4(MemToolParam);
    NInstPtr<NFrame> level5(MemToolParam);

    level1->AddChild(level2);
    level2->AddChild(level3);
    level3->AddChild(level4);
    level4->AddChild(level5);

    level1 = NULL;
}

TEST_F(TestFrame, AddRemoveChilds)
{
    NInstPtr<NFrame> parent1(MemToolParam);
    NInstPtr<NFrame> parent2(MemToolParam);
    NInstPtr<NFrame> child1(MemToolParam);
    NInstPtr<NFrame> child2(MemToolParam);

    parent1->AddChild(child1);
    EXPECT_TRUE(parent1->GetChildZOrder(child1) >= 0);
    EXPECT_EQ(GetChildCount(parent1), 1);

    parent2->AddChild(child2);
    EXPECT_TRUE(parent2->GetChildZOrder(child2) >= 0);
    EXPECT_EQ(GetChildCount(parent2), 1);

    parent1->RemoveChild(child1);
    EXPECT_EQ(GetChildCount(parent1), 0);
    {
        NAssertTempDisable();
        EXPECT_TRUE(parent1->GetChildZOrder(child1) == -1);
    }

    parent2->RemoveChild(child2);
    EXPECT_EQ(GetChildCount(parent2), 0);
    {
        NAssertTempDisable();
        EXPECT_TRUE(parent1->GetChildZOrder(child2) == -1);
    }
}

TEST_F(TestFrame, Parent)
{
    NInstPtr<NFrame> parent1(MemToolParam);
    NInstPtr<NFrame> parent2(MemToolParam);
    NInstPtr<NFrame> child1(MemToolParam);
    NInstPtr<NFrame> child2(MemToolParam);

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

    {
        NAssertTempDisable();
        EXPECT_TRUE(parent1->GetChildZOrder(child1) == -1);
    }
    EXPECT_EQ(reinterpret_cast<NFrame*>(NULL), child1->GetParent());

    // add & readd
    parent1->AddChild(child1);
    parent2->AddChild(child1);
    EXPECT_EQ(parent2, child1->GetParent());
    {
        NAssertTempDisable();
        EXPECT_TRUE(parent1->GetChildZOrder(child1) == -1);
    }
    EXPECT_TRUE(parent2->GetChildZOrder(child1) >= 0);

    parent1->AddChild(child1);
    EXPECT_EQ(parent1, child1->GetParent());
    {
        NAssertTempDisable();
        EXPECT_TRUE(parent2->GetChildZOrder(child1) == -1);
    }
    EXPECT_TRUE(parent1->GetChildZOrder(child1) >= 0);
    parent1->RemoveChild(child1);
}

TEST_F(TestFrame, ZOrder)
{
    NInstPtr<NFrame> parent(MemToolParam);
    NInstPtr<NFrame> child1(MemToolParam);
    NInstPtr<NFrame> child2(MemToolParam);

    parent->AddChild(child1);
    parent->AddChild(child2);
    EXPECT_EQ(parent->GetChildZOrder(child1), 0);
    EXPECT_EQ(parent->GetChildZOrder(child2), 1);

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

    parent->RemoveChild(child1);
    parent->RemoveChild(child2);

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

TEST_F(TestFrame, Id)
{
    NInstPtr<NFrame> parent(MemToolParam);
    NInstPtr<NFrame> child1(MemToolParam);
    NInstPtr<NFrame> child2(MemToolParam);
    NInstPtr<NFrame> grandson(MemToolParam);

    parent->AddChild(child1);
    parent->AddChild(child2);
    child1->AddChild(grandson);

    NString parentId;
    parentId.Format(_T("ParentId%d"), rand());
    parent->SetId(parentId);
    EXPECT_EQ(parentId, parent->GetId());

    NString childId1;
    childId1.Format(_T("ChildId1%d"), rand());
    child1->SetId(childId1);
    EXPECT_EQ(childId1, child1->GetId());

    NString childId2;
    childId2.Format(_T("ChildId2%d"), rand());
    child2->SetId(childId2);
    EXPECT_EQ(childId2, child2->GetId());

    NString grandsonId;
    grandsonId.Format(_T("grandsonId%d"), rand());
    grandson->SetId(grandsonId);
    EXPECT_EQ(grandsonId, grandson->GetId());

    EXPECT_EQ(parent->GetChildById(parentId, false), parent);
    EXPECT_EQ(parent->GetChildById(parentId, true), parent);

    EXPECT_EQ(parent->GetChildById(childId1, false), child1);
    EXPECT_EQ(parent->GetChildById(childId1, true), child1);

    EXPECT_EQ(parent->GetChildById(childId2, false), child2);
    EXPECT_EQ(parent->GetChildById(childId2, true), child2);

    EXPECT_EQ(parent->GetChildById(grandsonId, false), static_cast<NFrame*>(NULL));
    EXPECT_EQ(parent->GetChildById(grandsonId, true), grandson);

    parent = NULL;
}
