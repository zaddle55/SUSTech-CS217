#include <cstddef>
#include <iostream>
using namespace std;

template <class K, class V>
class Dictionary
{
    K* keys;
    V* values;
    size_t size_;
    size_t maxSize_;
public:
    Dictionary(size_t size): size_(0)
    {
        maxSize_ = 1;
        while (maxSize_ < size)
        {
            maxSize_ *= 2;
        }
        keys = new K[maxSize_];
        values = new V[maxSize_];
    }

    void add(K key, V value)
    {
        K* tmpKey; V* tmpVal;
        if (size_ + 1 >= maxSize_)
        {
            maxSize_ *= 2;
            tmpKey = new K[maxSize_];
            tmpVal = new V[maxSize_];
            for (size_t i = 0; i < size_; i++)
            {
                tmpKey[i] = keys[i];
                tmpVal[i] = values[i];
            }
            tmpKey[size_] = key;
            tmpVal[size_] = value;
            delete[] keys;
            delete[] values;
            keys = tmpKey;
            values = tmpVal;
        }
        else 
        {
            keys[size_] = key;
            values[size_] = value;
        }
        size_++;
    }

    void print()
    {
        for (size_t i = 0; i < size_; i++)
        {
            cout << "{" << keys[i] << ", " << values[i] << "}" << endl;
        }
    }
};

template<class V>
class Dictionary<int, V>
{
    int* keys;
    V* values;
    size_t size_;
    size_t maxSize_;
public:
    Dictionary(size_t size): size_(0)
    {
        maxSize_ = 1;
        while (maxSize_ < size)
        {
            maxSize_ *= 2;
        }
        keys = new int[maxSize_];
        values = new V[maxSize_];
    }

    void add(int key, V value)
    {
        int* tmpKey; V* tmpVal;
        if (size_ + 1 >= maxSize_)
        {
            maxSize_ *= 2;
            tmpKey = new int[maxSize_];
            tmpVal = new V[maxSize_];
            for (size_t i = 0; i < size_; i++)
            {
                tmpKey[i] = keys[i];
                tmpVal[i] = values[i];
            }
            tmpKey[size_] = key;
            tmpVal[size_] = value;
            delete[] keys;
            delete[] values;
            keys = tmpKey;
            values = tmpVal;
        }
        else 
        {
            keys[size_] = key;
            values[size_] = value;
        }
        size_++;
    }

    void print()
    {
        for (size_t i = 0; i < size_; i++)
        {
            cout << "{" << keys[i] << ", " << values[i] << "}" << endl;
        }
    }

    void sort()
    {
        for (size_t i = 0; i < size_; i++)
        {
            for (size_t j = 0; j < size_ - i - 1; j++)
            {
                if (keys[j] > keys[j + 1])
                {
                    int tmpKey = keys[j];
                    keys[j] = keys[j + 1];
                    keys[j + 1] = tmpKey;
                    V tmpVal = values[j];
                    values[j] = values[j + 1];
                    values[j + 1] = tmpVal;
                }
            }
        }
    }

    ~Dictionary()
    {
        delete[] keys;
        delete[] values;
    }
};

int main() {
    Dictionary<const char*, const char*> dict(10);
    dict.print();
    dict.add("apple", "fruit");
    dict.add("banana", "fruit");
    dict.add("dog", "animal");
    dict.print();

    Dictionary<int, const char*> dict_specialized(10);
    dict_specialized.print();
    dict_specialized.add(100, "apple");
    dict_specialized.add(101, "banana");
    dict_specialized.add(103, "dog");
    dict_specialized.add(89, "cat");
    dict_specialized.print();
    dict_specialized.sort();
    dict_specialized.print();
    
    return 0;
}