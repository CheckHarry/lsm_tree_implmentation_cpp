/*

implement a skip list to replace std::ordered_map

// also add statistics of the whole map for flushing to disk
*/




#define P 0.5


template<typename K , typename V , unsigned max_level>
struct skip_list_node
{
    K key;
    V value;
    skip_list_node<K , V , max_level>* forward[max_level];

    skip_list_node() {
        for (int i = 0 ; i < max_level ; i ++)
            forward[i] = nullptr;
    };
    

};

template<typename K , typename V , unsigned max_level>
struct skip_list
{
    skip_list_node<K , V , max_level> *start;


    cosnt skip_list_node<K , V , max_level> *search(T searchKey)
    {
        skip_list_node<K , V , max_level> * cur = start;

        for (unsigned i = max_level - 1 ; i >= 0 ; i --)
        {
            while ((cur -> forward[i]) != nullptr && cur -> forward[i] -> key < searchKey)
                cur = cur -> forward[i];
        }

        cur = cur -> forward[0];

        if (cur && cur -> key == searchKey)
            return cur;
        else
            return nullptr;

    }

};
