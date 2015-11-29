#pragma once


namespace nui
{
    namespace Data
    {
        template < typename Elem, int InitCount=8, int GrowCount=8 >
        class NArrayT
        {
        public:
            NArrayT(const NArrayT& array)
            {
                CopyArray(array);
            }

            NArrayT& operator = (const NArrayT& array)
            {
                if(this == &array)
                    return *this;
                CopyArray(array);
                return *this;
            }

            typedef void* Position;

            NArrayT()
            {
                buffer_ = NULL;
                count_ = 0;
                capacity_ = 0;
            }

            ~NArrayT()
            {
                if(buffer_ != NULL)
                {
                    NDeleteArray(buffer_);
                    buffer_ = NULL;
                }
            }

            bool AddItem(const Elem& value)
            {
                if(buffer_ == NULL)
                {
                    buffer_ = NNewArray(Elem, InitCount);
                    if(buffer_ == NULL)
                        return false;
                    capacity_ = InitCount;
                    count_ = 1;
                    buffer_[0] = value;
                }
                else if(count_ + 1 < capacity_)
                {
                    buffer_[count_] = value;
                    ++ count_;
                }
                else
                {
                    Elem* data = NNewArray(Elem, capacity_ + GrowCount);
                    if(data == NULL)
                        return false;
                    for(int i=0; i<count_; ++ i)
                    {
                        data[i] = buffer_[i];
                    }
                    data[count_] = value;
                    ++ count_;
                    capacity_ += GrowCount;
                    NDeleteArray(buffer_);
                    buffer_ = data;
                }
                return true;
            }

            Elem& operator [] (int pos)
            {
                if(pos < 0 || pos >= count_)
                    throw "Invalid Position";
                return buffer_[pos];
            }

            const Elem& operator [] (int pos) const
            {
                if(pos < 0 || pos >= count_)
                    throw "Invalid Position";
                return buffer_[pos];
            }

            Elem& GetAt(int pos)
            {
                if(pos < 0 || pos >= count_)
                    throw "Invalid Position";
                return buffer_[pos];
            }

            const Elem& GetAt(int pos) const
            {
                if(pos < 0 || pos >= count_)
                    throw "Invalid Position";
                return buffer_[pos];
            }

            int Find(const Elem& value)
            {
                for(int i=0; i<count_; ++ i)
                {
                    if(value == buffer_[i])
                        return i;
                }
                return -1;
            }

            bool InsertAt(int pos, const Elem& value)
            {
                if(pos < 0)
                    pos = 0;
                else if(pos >= count_)
                    pos = count_;
                if(buffer_ == NULL)
                {
                    return AddItem(value);
                }
                else if(count_ + 1 < capacity_)
                {
                    for(int i=count_-1; i>=pos; -- i)
                    {
                        buffer_[i+1] = buffer_[i];
                    }
                    buffer_[pos] = value;
                    ++count_;
                }
                else
                {
                    Elem* data = NNewArray(Elem, capacity_ + GrowCount);
                    if(data == NULL)
                        return false;
                    for(int i=0; i<count_; ++ i)
                    {
                        if(i < pos)
                            data[i] = buffer_[i];
                        else if(i == pos)
                            data[i] = value;
                        else
                            data[i] = buffer_[i - 1];
                    }
                    buffer_ = data;
                    capacity_ += GrowCount;
                    ++ count_;
                }
                return true;
            }

            int Count() const
            {
                return count_;
            }

            void ChangeItemPos(int prevPos, int newPos)
            {
                if(prevPos < 0 || prevPos >= count_
                    || newPos < 0 || newPos >= count_)
                {
                    return;
                }
                if(prevPos == newPos)
                    return;
                if(prevPos < newPos)
                {
                    Elem tmpValue = buffer_[prevPos];
                    for(int i=prevPos; i<newPos; ++ i)
                    {
                        buffer_[i] = buffer_[i + 1];
                    }
                    buffer_[newPos] = tmpValue;
                }
                else
                {
                    Elem tmpValue = buffer_[prevPos];
                    for(int i=prevPos; i>newPos; -- i)
                    {
                        buffer_[i] = buffer_[i - 1];
                    }
                    buffer_[newPos] = tmpValue;
                }
            }

            int GetItemPos(const Elem& item)
            {
                for(int i=0; i<count_; ++ i)
                {
                    if(item == buffer_[i])
                        return i;
                }
                return -1;
            }

            bool RemoveItem(const Elem& item)
            {
                int pos = GetItemPos(item);
                if(pos == -1)
                    return false;
                return RemoveAt(pos);
            }

            bool RemoveAt(int pos)
            {
                if(pos < 0 || pos >= count_)
                    return false;
                for(int i=pos; i<count_; ++ i)
                {
                    buffer_[i] = buffer_[i + 1];
                }
                -- count_;
                return true;
            }

            void Clear()
            {
                count_ = 0;
                capacity_ = 0;
                if(buffer_ != NULL)
                {
                    NDeleteArray(buffer_);
                    buffer_ = NULL;
                }
            }

        protected:
            void CopyArray(const NArrayT<Elem>& array)
            {
                capacity_ = count_ = array.count_;
                buffer_ = NNewArray(Elem, count_);
                for(int i=0; i<count_; ++ i)
                    buffer_[i] = array.buffer_[i];
            }

        private:
            Elem* buffer_;
            int count_;
            int capacity_;
        };

    }
}
