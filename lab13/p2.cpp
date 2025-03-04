#include <iostream>
#include <string>
using namespace std;

template<class T1, class T2>
class Pair
{
public:
    T1 key;
    T2 value;
    
    Pair(T1 key, T2 value): key(key), value(value) {}
    
    bool operator < (const Pair<T1, T2> &p) const {
        return key < p.key;
    }
    friend ostream& operator << (ostream &out, const Pair<T1, T2> &p) {
        out << p.key << " " << p.value;
        return out;
    }
};

int main() {

    Pair<string, int> one("Tom", 19);
    Pair<string, int> two("Alice", 20);

    if (one < two) {
        cout << one << endl; 
    } else {
        cout << two << endl;
    }
    
    return 0;
}