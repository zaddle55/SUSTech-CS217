#include <iostream>

enum datatype
{
    TYPE_INT8 = 1,
    TYPE_INT16 = 2,
    TYPE_INT32 = 4,
    TYPE_INT64 = 8
};

struct Point
{
    datatype type;
    union 
    {
        std::int8_t data8[3];
        std::int16_t data16[3];
        std::int32_t data32[3];
        std::int64_t data64[3];
    };
};

size_t datawidth( Point p )
{
    return size_t( p.type * 3 );
}

int64_t l1norm( Point p )
{
    int64_t sum = 0;
    for ( int i = 0; i < 3; i++ )
    {
        switch ( p.type )
        {
            case TYPE_INT8:
                sum += std::abs( p.data8[i] );
                break;
            case TYPE_INT16:
                sum += std::abs( p.data16[i] );
                break;
            case TYPE_INT32:
                sum += std::abs( p.data32[i] );
                break;
            case TYPE_INT64:
                sum += std::abs( p.data64[i] );
                break;
        }
    }
    return sum;
}

int main()
{
    Point p1 = { TYPE_INT8, .data8={ 1, 2, 3 } };
    Point p2 = { TYPE_INT16, .data16={ 1, 2, 3 } };
    Point p3 = { TYPE_INT32, .data32={ 1, 2, 3 } };
    Point p4 = { TYPE_INT64, .data64={ 1, 2, 3 } };

    std::cout << "Data width of p1: " << datawidth( p1 ) << std::endl;
    std::cout << "Data width of p2: " << datawidth( p2 ) << std::endl;
    std::cout << "Data width of p3: " << datawidth( p3 ) << std::endl;
    std::cout << "Data width of p4: " << datawidth( p4 ) << std::endl;

    std::cout << "L1 norm of p1: " << l1norm( p1 ) << std::endl;
    std::cout << "L1 norm of p2: " << l1norm( p2 ) << std::endl;
    std::cout << "L1 norm of p3: " << l1norm( p3 ) << std::endl;
    std::cout << "L1 norm of p4: " << l1norm( p4 ) << std::endl;
}
