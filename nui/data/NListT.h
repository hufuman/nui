#pragma once



template < typename Elem >
class NListT
{
    NListT(const NListT&);
    NListT& operator = (const NListT&);

    class NListItemT
    {
    public:
        NListItemT* Prev;
        NListItemT* Next;

        Elem Value;
    };

public:
    typedef void* Position;

    NListT()
    {
        head_ = NULL;
        tail_ = NULL;
        count_ = 0;
    }

    ~NListT()
    {
        RemoveAllItems();
    }

    Position AddItem(const Elem& value)
    {
        if(head_ == NULL)
        {
            head_ = tail_ = NNew(NListItemT);
            head_->Next = NULL;
            head_->Prev = NULL;
            head_->Value = value;
        }
        else
        {
            NListItemT* item = NNew(NListItemT);
            tail_->Next = item;
            item->Prev = tail_;
            item->Next = NULL;
            item->Value = value;
            tail_ = item;
        }
        ++ count_;
        return reinterpret_cast<Position>(tail_);
    }

    Position GetHeadPosition() const
    {
        return reinterpret_cast<Position>(head_);
    }

    Position InsertItem(Position pos, const Elem& value)
    {
        NListItemT* posItem = reinterpret_cast<NListItemT*>(pos);
        NListItemT* item = NNew(NListItemT);
        item->Value = value;
        if(posItem == NULL || posItem == head_)
        {
            posItem = head_;
            if(posItem == NULL)
            {
                head_ = tail_ = item;
                item->Prev = NULL;
                item->Next = NULL;
            }
            else
            {
                item->Prev = NULL;
                item->Next = head_;
                posItem->Prev = item;
                head_ = item;
            }
        }
        else
        {
            item->Next = posItem;
            item->Prev = posItem->Prev;
            posItem->Prev->Next = item;
            posItem->Prev = item;
        }
        ++ count_;
        return reinterpret_cast<Position>(item);
    }

    bool RemoveItem(Position pos)
    {
        if(pos == NULL)
            return false;

        NListItemT* posItem = reinterpret_cast<NListItemT*>(pos);
        if(posItem == head_)
            head_ = posItem->Next;
        if(posItem == tail_)
            tail_ = tail_->Prev;
        if(posItem->Prev != NULL)
            posItem->Prev->Next = posItem->Next;
        if(posItem->Next != NULL)
            posItem->Next->Prev = posItem->Prev;
        NDelete(posItem);
        -- count_;
        return true;
    }

    void RemoveAllItems()
    {
        NListItemT* item = head_;
        head_ = tail_ = NULL;
        count_ = 0;
        for(; item != NULL;)
        {
            NListItemT* nextItem = item->Next;
            NDelete(item);
            item = nextItem;
        }
    }

    Elem& GetNextItem(Position& pos) const
    {
        if(pos == NULL)
            throw "Invalid position";

        NListItemT* posItem = reinterpret_cast<NListItemT*>(pos);
        Elem& result = posItem->Value;
        pos = reinterpret_cast<Position>(posItem->Next);
        return result;
    }

    int Count() const
    {
        return count_;
    }

private:
    int count_;
    NListItemT* head_;
    NListItemT* tail_;
};
