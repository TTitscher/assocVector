#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "AssocVector.hpp"

namespace std
{

template<
      typename _T1
    , typename _T2
>
std::ostream & operator<<( std::ostream & out, std::pair< _T1, _T2 > const & pair )
{
    return out << "(" << pair.first << "," << pair.second << ")";
}

}

//
// test_CmpByFirst
//
void test_CmpByFirst_1()
{
    typedef std::pair< int, int > Pii;

    Pii p1( 1, 11 );
    Pii p2( 1, 22 );

    assert( ( util::CmpByFirst< Pii, std::equal_to< int > >()( p1, p2 ) ) );

    assert( ( util::CmpByFirst< Pii, std::equal_to< int > >()( p1, 1 ) ) );
}

void test_CmpByFirst_2()
{
        typedef std::pair< int, int > Pii;

        Pii p1( 1, 22 );
        Pii p2( 2, 11 );

        assert( ( util::CmpByFirst< Pii, std::less< int > >()( p1, p2 ) ) );
        assert( ( util::CmpByFirst< Pii, std::less< int > >()( p1, 2 ) ) );

        assert( ( util::CmpByFirst< Pii, std::greater< int > >()( p1, p2 ) == false ) );
}

void test_CmpByFirst_3()
{
    typedef std::pair< int, int > Pii;

    Pii p1( 2, 11 );
    Pii p2( 1, 22 );

    assert( ( util::CmpByFirst< Pii, std::greater< int > >()( p1, p2 ) ) );
    assert( ( util::CmpByFirst< Pii, std::greater< int > >()( p1, 1 ) ) );

    assert( ( util::CmpByFirst< Pii, std::less< int > >()( p1, p2 ) == false ) );
}

//
// test_isBetween
//
void test_isBetween()
{
    assert( ( util::isBetween( 1, 0, 3 ) == false ) );
    assert( ( util::isBetween( 1, 1, 3 ) ) );
    assert( ( util::isBetween( 1, 2, 3 ) ) );
    assert( ( util::isBetween( 1, 3, 3 ) ) );
    assert( ( util::isBetween( 1, 4, 3 ) == false ) );
}

namespace detail
{
    //
    // For_ConstructRange_DestroyRange_Tests
    //
    struct For_ConstructRange_DestroyRange_Tests
    {
        For_ConstructRange_DestroyRange_Tests(){ ++ counter; }
        ~For_ConstructRange_DestroyRange_Tests(){ -- counter; }

    public:
        static int counter;

    private:
        int _some_data_to_makes_size_of_not_zero;
    };

    int For_ConstructRange_DestroyRange_Tests::counter = 0;
}

//
// test_constructRange_destroyRange
//
void test_constructRange_destroyRange()
{
    int const numberOfObjects = 10;

    std::allocator< detail::For_ConstructRange_DestroyRange_Tests > allocator;
    assert( detail::For_ConstructRange_DestroyRange_Tests::counter == 0 );

    detail::For_ConstructRange_DestroyRange_Tests * memory = allocator.allocate( numberOfObjects );
    assert( detail::For_ConstructRange_DestroyRange_Tests::counter == 0 );

    util::constructRange( memory, memory + numberOfObjects );
    assert( detail::For_ConstructRange_DestroyRange_Tests::counter == 10 );

    util::destroyRange( memory, memory + numberOfObjects );
    assert( detail::For_ConstructRange_DestroyRange_Tests::counter == 0 );

    allocator.deallocate( memory, numberOfObjects );
}

//
// test_copyRange
//
void test_copyRange_empty_array()
{
    std::vector< int > array;
    std::vector< int > array2;

    util::copyRange( array.begin(), array.end(), array2.begin() );

    assert( array2.empty() );
}

void test_copyRange_self_copy()
{
    std::vector< int > array;
    array.push_back( 1 );
    array.push_back( 2 );
    array.push_back( 3 );
    array.push_back( 4 );
    array.push_back( 5 );
    array.push_back( 6 );

    util::copyRange( array.begin(), array.end(), array.begin() );

    std::vector< int > expected;
    expected.push_back( 1 );
    expected.push_back( 2 );
    expected.push_back( 3 );
    expected.push_back( 4 );
    expected.push_back( 5 );
    expected.push_back( 6 );

    assert( array == expected );
}

void test_copyRange_between_different_containers()
{
    std::vector< int > array;
    array.push_back( 1 );
    array.push_back( 2 );
    array.push_back( 3 );
    array.push_back( 4 );
    array.push_back( 5 );
    array.push_back( 6 );

    std::vector< int > array2( 6, 0 );

    util::copyRange( array.begin(), array.end(), array2.begin() );

    assert( array == array2 );
}

void test_copyRange_overlap_less_then_half()
{
    std::vector< int > array;
    array.push_back( 1 );
    array.push_back( 2 );
    array.push_back( 3 );
    array.push_back( 4 );
    array.push_back( 5 );
    array.push_back( 6 );

    util::copyRange( array.begin(), array.begin() + 3, array.begin() + 3 );

    std::vector< int > expected;
    expected.push_back( 1 );
    expected.push_back( 2 );
    expected.push_back( 3 );
    expected.push_back( 1 );
    expected.push_back( 2 );
    expected.push_back( 3 );

    assert( array == expected );
}
void test_copyRange_overlap_more_than_half()
{
    std::vector< int > array;
    array.push_back( 1 );
    array.push_back( 2 );
    array.push_back( 3 );
    array.push_back( 4 );
    array.push_back( 5 );
    array.push_back( 6 );

    util::copyRange( array.begin(), array.begin() + 4, array.begin() + 2 );

    std::vector< int > expected;
    expected.push_back( 1 );
    expected.push_back( 2 );
    expected.push_back( 1 );
    expected.push_back( 2 );
    expected.push_back( 3 );
    expected.push_back( 4 );

    assert( array == expected );
}

void test_copyRange_overlap_copy_to_begining()
{
    std::vector< int > array;
    array.push_back( 1 );
    array.push_back( 2 );
    array.push_back( 3 );
    array.push_back( 4 );
    array.push_back( 5 );
    array.push_back( 6 );

    util::copyRange( array.begin() + 2, array.end(), array.begin() );

    std::vector< int > expected;
    expected.push_back( 3 );
    expected.push_back( 4 );
    expected.push_back( 5 );
    expected.push_back( 6 );
    expected.push_back( 5 );
    expected.push_back( 6 );

    assert( array == expected );
}

//
// test_merge
//
void test_merge_1()
{
    using array::Array;

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );
        a1.setSize( 0 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );

        a2[0] = 1;
        a2[1] = 2;
        a2[2] = 3;
        a2[3] = 4;

        a2.setSize( 4 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 4 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 4 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );

        delete [] a1.getData();
        delete [] a2.getData();
    }

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );

        a1[0] = 1;
        a1[1] = 2;
        a1[2] = 3;
        a1[3] = 4;

        a1.setSize( 4 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );
        a2.setSize( 0 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 4 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 0 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );

        delete [] a1.getData();
        delete [] a2.getData();
    }

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );

        a1[0] = 1;
        a1[1] = 2;
        a1[2] = 3;
        a1[3] = 4;
        a1[4] = 5;
        a1[5] = 6;

        a1.setSize( 6 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );

        a2[0] = 7;
        a2[1] = 8;
        a2[2] = 9;
        a2[3] = 10;

        a2.setSize( 4 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 10 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 4 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );
        assert( a1[4] == 5 );
        assert( a1[5] == 6 );
        assert( a1[6] == 7 );
        assert( a1[7] == 8 );
        assert( a1[8] == 9 );
        assert( a1[9] == 10 );

        delete [] a1.getData();
        delete [] a2.getData();
    }

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );

        a1[0] = 1;
        a1[1] = 3;
        a1[2] = 5;
        a1[3] = 7;
        a1[4] = 9;
        a1[5] = 11;

        a1.setSize( 6 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );

        a2[0] = 2;
        a2[1] = 4;
        a2[2] = 6;
        a2[3] = 8;

        a2.setSize( 4 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 10 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 4 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );
        assert( a1[4] == 5 );
        assert( a1[5] == 6 );
        assert( a1[6] == 7 );
        assert( a1[7] == 8 );
        assert( a1[8] == 9 );
        assert( a1[9] == 11 );

        delete [] a1.getData();
        delete [] a2.getData();
    }

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );

        a1[0] = 5;
        a1[1] = 6;
        a1[2] = 7;
        a1[3] = 8;
        a1[4] = 9;
        a1[5] = 10;

        a1.setSize( 6 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );

        a2[0] = 1;
        a2[1] = 2;
        a2[2] = 3;
        a2[3] = 4;

        a2.setSize( 4 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 10 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 4 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );
        assert( a1[4] == 5 );
        assert( a1[5] == 6 );
        assert( a1[6] == 7 );
        assert( a1[7] == 8 );
        assert( a1[8] == 9 );
        assert( a1[9] == 10 );

        delete [] a1.getData();
        delete [] a2.getData();
    }

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );

        a1[0] = 1;
        a1[1] = 2;
        a1[2] = 3;
        a1[3] = 8;
        a1[4] = 9;
        a1[5] = 10;

        a1.setSize( 6 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );

        a2[0] = 4;
        a2[1] = 5;
        a2[2] = 6;
        a2[3] = 7;

        a2.setSize( 4 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 10 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 4 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );
        assert( a1[4] == 5 );
        assert( a1[5] == 6 );
        assert( a1[6] == 7 );
        assert( a1[7] == 8 );
        assert( a1[8] == 9 );
        assert( a1[9] == 10 );

        delete [] a1.getData();
        delete [] a2.getData();
    }

    {
        Array< int > a1;
        a1.setData( new int[ 10 ] );
        a1.setCapacity( 10 );

        a1[0] = 2;
        a1[1] = 3;
        a1[2] = 5;
        a1[3] = 6;
        a1[4] = 8;
        a1[5] = 9;

        a1.setSize( 6 );

        Array< int > a2;
        a2.setData( new int[ 10 ] );
        a2.setCapacity( 10 );

        a2[0] = 1;
        a2[1] = 4;
        a2[2] = 7;
        a2[3] = 10;

        a2.setSize( 4 );

        array::merge( a1, a2, std::less< int >() );

        assert( a1.size() == 10 );
        assert( a1.capacity() == 10 );

        assert( a2.size() == 4 );
        assert( a2.capacity() == 10 );

        assert( a1[0] == 1 );
        assert( a1[1] == 2 );
        assert( a1[2] == 3 );
        assert( a1[3] == 4 );
        assert( a1[4] == 5 );
        assert( a1[5] == 6 );
        assert( a1[6] == 7 );
        assert( a1[7] == 8 );
        assert( a1[8] == 9 );
        assert( a1[9] == 10 );

        delete [] a1.getData();
        delete [] a2.getData();
    }
}

void test_merge_2()
{
    array::Array< std::pair< int, int > > storage;

    storage.setData( new std::pair< int, int >[ 10 ] );
    storage.setSize( 0 );
    storage.setCapacity( 10 );

    for( int i = 10 ; i < 15 ; ++ i ){
        storage[ storage.size() ] = std::pair< int, int >( i, 0 );
        storage.setSize( storage.size() + 1 );
    }

    array::Array< std::pair< int, int > > buffer;

    buffer.setData( new std::pair< int, int >[ 3 ] );
    buffer.setSize( 0 );
    buffer.setCapacity( 3 );

    for( int i = 3 ; i < 6 ; ++ i ){
        buffer[ buffer.size() ] = std::pair< int, int >( i, 0 );
        buffer.setSize( buffer.size() + 1 );
    }

    array::merge( storage, buffer, util::CmpByFirst< std::pair< int, int >, std::less< int > >() );

    assert( storage.capacity() == 10 );
    assert( storage.size() == 8 );

    assert( buffer.size() == 3 );
    assert( buffer.capacity() == 3 );

    assert( storage[0].first == 3 );
    assert( storage[1].first == 4 );
    assert( storage[2].first == 5 );
    assert( storage[3].first == 10 );
    assert( storage[4].first == 11 );
    assert( storage[5].first == 12 );
    assert( storage[6].first == 13 );
    assert( storage[7].first == 14 );

    delete [] storage.getData();
    delete [] buffer.getData();
}

void test_merge_3()
{
    array::Array< std::pair< int, int > > storage;

    storage.setData( new std::pair< int, int >[ 10 ] );
    storage.setSize( 0 );
    storage.setCapacity( 10 );

    for( int i = 10 ; i < 15 ; ++ i ){
        storage[ storage.size() ] = std::pair< int, int >( i, 0 );
        storage.setSize( storage.size() + 1 );
    }

    array::Array< std::pair< int, int > > buffer;

    buffer.setData( new std::pair< int, int >[ 3 ] );
    buffer.setSize( 0 );
    buffer.setCapacity( 15 );

    for( int i = 23 ; i < 26 ; ++ i ){
        buffer[ buffer.size() ] = std::pair< int, int >( i, 0 );
        buffer.setSize( buffer.size() + 1 );
    }

    array::merge( storage, buffer, util::CmpByFirst< std::pair< int, int >, std::less< int > >() );

    assert( storage.capacity() == 10 );
    assert( storage.size() == 8 );

    assert( buffer.size() == 3 );
    assert( buffer.capacity() == 15 );

    assert( storage[0].first == 10 );
    assert( storage[1].first == 11 );
    assert( storage[2].first == 12 );
    assert( storage[3].first == 13 );
    assert( storage[4].first == 14 );
    assert( storage[5].first == 23 );
    assert( storage[6].first == 24 );
    assert( storage[7].first == 25 );

    delete [] storage.getData();
    delete [] buffer.getData();
}

void test_merge_4()
{
    array::Array< std::pair< int, int > > storage;

    storage.setData( new std::pair< int, int >[ 10 ] );
    storage.setSize( 0 );
    storage.setCapacity( 10 );

    for( int i = 10 ; i < 20 ; i += 2 ){
        storage[ storage.size() ] = std::pair< int, int >( i, 0 );
        storage.setSize( storage.size() + 1 );
    }

    array::Array< std::pair< int, int > > buffer;

    buffer.setData( new std::pair< int, int >[ 3 ] );
    buffer.setSize( 0 );
    buffer.setCapacity( 10 );

    for( int i = 13 ; i < 19 ; i += 2 ){
        buffer[ buffer.size() ] = std::pair< int, int >( i, 0 );
        buffer.setSize( buffer.size() + 1 );
    }

    array::merge( storage, buffer, util::CmpByFirst< std::pair< int, int >, std::less< int > >() );

    assert( storage.capacity() == 10 );
    assert( storage.size() == 8 );

    assert( buffer.size() == 3 );
    assert( buffer.capacity() == 10 );

    assert( storage[0].first == 10 );
    assert( storage[1].first == 12 );
    assert( storage[2].first == 13 );
    assert( storage[3].first == 14 );
    assert( storage[4].first == 15 );
    assert( storage[5].first == 16 );
    assert( storage[6].first == 17 );
    assert( storage[7].first == 18 );

    delete [] storage.getData();
    delete [] buffer.getData();
}

void test_merge_5()
{
    array::Array< std::pair< int, int > > storage;

    storage.setData( new std::pair< int, int >[ 10 ] );
    storage.setSize( 0 );
    storage.setCapacity( 10 );

    for( int i = 10 ; i < 20 ; i += 2 ){
        storage[ storage.size() ] = std::pair< int, int >( i, 0 );
        storage.setSize( storage.size() + 1 );
    }

    array::Array< std::pair< int, int > > buffer;

    buffer.setData( new std::pair< int, int >[ 3 ] );
    buffer.setSize( 0 );
    buffer.setCapacity( 3 );

    array::merge( storage, buffer, util::CmpByFirst< std::pair< int, int >, std::less< int > >() );

    assert( storage.capacity() == 10 );
    assert( storage.size() == 5 );

    assert( buffer.size() == 0 );
    assert( buffer.capacity() == 3 );

    assert( storage[0].first == 10 );
    assert( storage[1].first == 12 );
    assert( storage[2].first == 14 );
    assert( storage[3].first == 16 );
    assert( storage[4].first == 18 );

    delete [] storage.getData();
    delete [] buffer.getData();
}

void test_merge_6()
{
    array::Array< std::pair< int, int > > storage;

    storage.setData( new std::pair< int, int >[ 10 ] );
    storage.setSize( 0 );
    storage.setCapacity( 10 );

    array::Array< std::pair< int, int > > buffer;

    buffer.setData( new std::pair< int, int >[ 3 ] );
    buffer.setSize( 0 );
    buffer.setCapacity( 20 );

    for( int i = 13 ; i < 19 ; i += 2 ){
        buffer[ buffer.size() ] = std::pair< int, int >( i, 0 );
        buffer.setSize( buffer.size() + 1 );
    }

    array::merge( storage, buffer, util::CmpByFirst< std::pair< int, int >, std::less< int > >() );

    assert( storage.capacity() == 10 );
    assert( storage.size() == 3 );

    assert( buffer.size() == 3 );
    assert( buffer.capacity() == 20 );

    assert( storage[0].first == 13 );
    assert( storage[1].first == 15 );
    assert( storage[2].first == 17 );

    delete [] storage.getData();
    delete [] buffer.getData();
}

void test_merge_7()
{
    array::Array< std::pair< int, int > > storage;

    storage.setData( new std::pair< int, int >[ 10 ] );
    storage.setSize( 0 );
    storage.setCapacity( 10 );

    array::Array< std::pair< int, int > > buffer;

    buffer.setData( new std::pair< int, int >[ 3 ] );
    buffer.setSize( 0 );
    buffer.setCapacity( 3 );

    array::merge( storage, buffer, util::CmpByFirst< std::pair< int, int >, std::less< int > >() );

    assert( storage.capacity() == 10 );
    assert( storage.size() == 0 );

    assert( buffer.size() == 0 );
    assert( buffer.capacity() == 3 );

    delete [] storage.getData();
    delete [] buffer.getData();
}

//
// test_push_back
//
void test_push_back()
{
    typedef AssocVector< std::string, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    av.insert( AssocVector::value_type( "1", 1 ) );
    av.insert( AssocVector::value_type( "2", 2 ) );
    av.insert( AssocVector::value_type( "3", 3 ) );
    av.insert( AssocVector::value_type( "4", 4 ) );
    av.insert( AssocVector::value_type( "5", 5 ) );

    assert( av[ "1" ] == 1 );
    assert( av[ "2" ] == 2 );
    assert( av[ "3" ] == 3 );
    assert( av[ "4" ] == 4 );
    assert( av[ "5" ] == 5 );
}

//
// test_insert_in_random_order
//
void test_insert_in_random_order()
{
    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    av.insert( AssocVector::value_type( 10, 0 ) );
    av.insert( AssocVector::value_type( 9, 0 ) );
    av.insert( AssocVector::value_type( 8, 0 ) );

    av.insert( AssocVector::value_type( 16, 0 ) );
    av.insert( AssocVector::value_type( 17, 0 ) );
    av.insert( AssocVector::value_type( 18, 0 ) );

    av.insert( AssocVector::value_type( 7, 0 ) );
    av.insert( AssocVector::value_type( 6, 0 ) );
    av.insert( AssocVector::value_type( 5, 0 ) );
    av.insert( AssocVector::value_type( 4, 0 ) );

    av.insert( AssocVector::value_type( 11, 0 ) );
    av.insert( AssocVector::value_type( 12, 0 ) );
    av.insert( AssocVector::value_type( 13, 0 ) );

    av.insert( AssocVector::value_type( 3, 0 ) );
    av.insert( AssocVector::value_type( 2, 0 ) );
    av.insert( AssocVector::value_type( 1, 0 ) );
    av.insert( AssocVector::value_type( 0, 0 ) );

    av.insert( AssocVector::value_type( 14, 0 ) );
    av.insert( AssocVector::value_type( 15, 0 ) );

    av.insert( AssocVector::value_type( 19, 0 ) );

    assert( av.size() == 20 );

    for( int i = 0 ; i < av.size() ; ++ i ){
        assert( av.find( i ) != av.end() );
        assert( av.find( i )->first == i );
    }
}

//
// test_insert_in_increasing_order
//
void test_insert_in_increasing_order()
{
    std::size_t const counter = 10;

    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    for( std::size_t i = 0 ; i < counter ; ++ i ){
        av.insert( AssocVector::value_type( i, 0 ) );
    }

    for( std::size_t i = 0 ; i < counter ; ++ i ){
        assert( av.find( i ) != av.end() );
        assert( av.find( i )->first == i );
    }
}

//
// test_insert_in_decreasing_order
//
void test_insert_in_decreasing_order()
{
    std::size_t const counter = 10;

    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    for( std::size_t i = counter ; i > 0 ; -- i ){
        av.insert( AssocVector::value_type( i, 0 ) );
    }

    for( std::size_t i = counter ; i > 0 ; -- i ){
        assert( av.find( i ) != av.end() );
        assert( av.find( i )->first == i );
    }
}

//
// test_find
//
void test_find_1()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    assert( av.find( 2 ) == av.end() );

    av.insert( AssocVector::value_type( 2, 3 ) );

    assert( av.find( 2 ) != av.end() );
    assert( av.find( 2 )->second == 3 );
}

void test_find_2()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    for( int i = 32 ; i < 32 + 64 ; ++ i ){
        av.insert( AssocVector::value_type( i, i * i ) );
    }

    for( int i = 0 ; i < 32 ; ++ i ){
        assert( av.find( i ) == av.end() );
    }

    for( int i = 32 ; i < 32 + 64 ; ++ i ){
        assert( av.find( i ) != av.end() );
        assert( av.find( i )->second == i * i );
    }

    for( int i = 32 + 64 ; i < 32 + 64 + 32; ++ i ){
        assert( av.find( i ) == av.end() );
    }
}

//
// test_count
//
void test_count()
{
    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    av.insert( AssocVector::value_type( 1, 1 ) );
    av.insert( AssocVector::value_type( 3, 2 ) );

    assert( av.count( 1 ) == 1 );
    assert( av.count( 2 ) == 0 );
    assert( av.count( 3 ) == 1 );
    assert( av.count( 4 ) == 0 );
}

//
// test_erase
//
void test_erase_1()
{
    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    std::size_t const counter = 128;

    for( std::size_t i = 0 ; i < counter ; ++ i ){
        av.insert( AssocVector::value_type( i, i ) );
    }

    assert( av.size() == counter );

    for( std::size_t i = 0 ; i < counter ; i += 2 ){
        av.erase( i );
    }

    assert( av.size() == counter / 2 );

    for( std::size_t i = 0 ; i < counter ; i += 2 ){
        assert( av.find( i ) == av.end() );
    }

    for( std::size_t i = 1 ; i < counter ; i += 2 ){
        av.erase( i );
    }

    assert( av.empty() );
}

void test_erase_2()
{
    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    int const counter = 10;

    for( int i = 0 ; i < counter ; ++ i ){
        av.insert( AssocVector::value_type( i, i ) );
    }

    for( int i = 0 ; i < counter ; ++ i ){
        assert( av.find( i ) != av.end() );
        av.erase( av.find( i ) );
        assert( av.size() == counter - i - 1 );
    }

    assert( av.empty() );
}

//
// test_operator_index
//
void test_operator_index_1()
{
    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    assert( av.empty() );

    av[ 1 ] = 1;
    assert( av.size() == 1 );
    assert( av.find( 1 ) != av.end() );
    assert( av.find( 1 )->second == 1 );

    av[ 1 ] = 11;
    assert( av[ 1 ] == 11 );

    av[ 3 ] = 33;
    assert( av[ 3 ] == 33 );

    av[ 2 ] = 22;
    assert( av[ 2 ] == 22 );
}

void test_operator_index_2()
{
    int const counter = 20000;

    std::vector< int > randomValues( counter, 0 );
    std::generate( randomValues.begin(), randomValues.end(), & rand );

    typedef AssocVector< int, int > AssocVector;
    AssocVector av;

    for( int i = 0 ; i < counter ; ++ i ){
        av[ randomValues[ i ] ] = i;
        assert( av[ randomValues[ i ] ] == i );
    }
}

namespace detail
{
    struct For_Test_User_Type_K
    {
        bool operator<( For_Test_User_Type_K const & k )const{return false;}
    };

    std::ostream & operator<<( std::ostream & out , For_Test_User_Type_K const & )
    {
        return out << "[]";
    }

    struct For_Test_User_Type_M
    {
    };

    std::ostream & operator<<( std::ostream & out , For_Test_User_Type_M const & )
    {
        return out << "[]";
    }
}

//
// test_user_type
//
void test_user_type()
{
    typedef AssocVector< detail::For_Test_User_Type_K, detail::For_Test_User_Type_M > AssocVector;

    AssocVector av;
    av.insert( AssocVector::value_type( detail::For_Test_User_Type_K(), detail::For_Test_User_Type_M() ) );
    detail::For_Test_User_Type_M & m = av[ detail::For_Test_User_Type_K() ];
    av[ detail::For_Test_User_Type_K() ] = m;
    av.find( detail::For_Test_User_Type_K() );
}

//
// test_copy_constructor
//
void test_copy_constructor()
{
    typedef AssocVector< std::string, int > AssocVector;

    AssocVector assocVector1;

    assocVector1[ "a" ] = 1;
    assocVector1[ "b" ] = 2;
    assocVector1[ "c" ] = 3;
    assocVector1[ "d" ] = 4;
    assocVector1[ "e" ] = 5;

    AssocVector assocVector2( assocVector1 );
    assert( assocVector1 == assocVector2 );
}

//
// test_assign_operator
//
void test_assign_operator()
{
    typedef AssocVector< std::string, int > AssocVector;

    AssocVector assocVector1;

    assocVector1[ "a" ] = 1;
    assocVector1[ "b" ] = 2;
    assocVector1[ "c" ] = 3;
    assocVector1[ "d" ] = 4;
    assocVector1[ "e" ] = 5;

    AssocVector assocVector2;
    assocVector2 = assocVector1;

    assert( assocVector1 == assocVector2 );
}

//
// test_clear
//
void test_clear()
{
    typedef AssocVector< std::string, int > AssocVector;

    AssocVector assocVector;

    assocVector[ "a" ] = 1;
    assocVector[ "b" ] = 2;
    assocVector[ "c" ] = 3;
    assocVector[ "d" ] = 4;
    assocVector[ "e" ] = 5;

    assocVector.erase( "a" );

    assert( assocVector.size() == 4 );

    assocVector.clear();

    assert( assocVector.empty() );
}

//
// test_iterator_to_const_iterator_conversion
//
void test_iterator_to_const_iterator_conversion()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator iterator;
    typedef AssocVector::const_iterator const_iterator;

    AssocVector av;

    iterator i = av.find( 10 );
    assert( i == av.end() );

    const_iterator ci = av.find( 10 );
    assert( ci == av.end() );
}

//
// test_iterators_equal
//
void test_iterators_equal()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector assocVector1024;
    assocVector1024.reserve( 1024 );

    AssocVector assocVector8;
    assocVector8.reserve( 8 );

    std::vector< int > randomValues( 1024, 0 );
    std::generate( randomValues.begin(), randomValues.end(), & rand );

    for( std::size_t i = 0 ; i < randomValues.size() ; ++ i )
    {
        assocVector8.insert( AssocVector::value_type( randomValues[i], i ) );
        assocVector1024.insert( AssocVector::value_type( randomValues[i], i ) );
    }

    assert(
        (
            std::equal(
                  assocVector8.begin()
                , assocVector8.end()
                , assocVector1024.begin()
                , std::equal_to< std::pair< int, int > >()
            )
        )
    );
}

//
// test_reverse_iterators_equal
//
void test_reverse_iterators_equal()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector assocVector1024;
    assocVector1024.reserve( 1024 );

    AssocVector assocVector8;
    assocVector8.reserve( 8 );

    std::vector< int > randomValues( 1024, 0 );
    std::generate( randomValues.begin(), randomValues.end(), & rand );

    for( std::size_t i = 0 ; i < randomValues.size() ; ++ i )
    {
        assocVector8.insert( AssocVector::value_type( randomValues[ i ], i ) );
        assocVector1024.insert( AssocVector::value_type( randomValues[ i ], i ) );
    }

    assert(
        (
            std::equal(
                  assocVector8.rbegin()
                , assocVector8.rend()
                , assocVector1024.rbegin()
                , std::equal_to< std::pair< int, int > >()
            )
        )
    );
}

//
// test_iterators_distance
//
void test_iterators_distance()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    assert( av.begin() == av.end() );
    assert( ( std::distance( av.begin(), av.end() ) == 0 ) );

    av.insert( AssocVector::value_type( 1, 1 ) );

    assert( av.begin() != av.end() );
    assert( ( std::distance( av.begin(), av.end() ) == 1 ) );

    av.insert( AssocVector::value_type( 2, 2 ) );
    assert( ( std::distance( av.begin(), av.end() ) == 2 ) );

    av.insert( AssocVector::value_type( 3, 3 ) );
    assert( ( std::distance( av.begin(), av.end() ) == 3 ) );
}

//
// test_reverse_iterators_distance
//
void test_reverse_iterators_distance()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    assert( av.rbegin() == av.rend() );
    assert( ( std::distance( av.rbegin(), av.rend() ) == 0 ) );

    av.insert( AssocVector::value_type( 1, 1 ) );

    assert( av.rbegin() != av.rend() );
    assert( ( std::distance( av.rbegin(), av.rend() ) == 1 ) );

    av.insert( AssocVector::value_type( 2, 2 ) );
    assert( ( std::distance( av.rbegin(), av.rend() ) == 2 ) );

    av.insert( AssocVector::value_type( 3, 3 ) );
    assert( ( std::distance( av.rbegin(), av.rend() ) == 3 ) );
}

//
// test_iterators_begin_equals_end_in_empty_container
//
void test_iterators_begin_equals_end_in_empty_container()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    assert( av.begin() == av.end() );
}

//
// test_reverse_iterators_begin_equal_end_in_empty_storage
//
void test_reverse_iterators_begin_equal_end_in_empty_storage()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    assert( av.rbegin() == av.rend() );
}

//
// test_iterators_increment_decrement
//
void test_iterators_increment_decrement()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;
    av.reserve( 128 );

    for( std::size_t i = 0 ; i < av.size() ; ++ i ){
        av.insert( AssocVector::value_type( i, i ) );
    }

    Iterator current = av.begin();
    Iterator const begin = av.begin();
    Iterator const end = av.end();

    for( /*empty*/ ; current != end ; ++ current );
    for( /*empty*/ ; current != begin ; -- current );

    assert( current == begin );
}

//
// test_iterator_1
//
void test_iterator_1()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;

    for( int i = 0 ; i < 7 ; ++ i ){
        av.insert( AssocVector::value_type( i, 0 ) );
    }

    Iterator current = av.begin();
    Iterator end = av.end();

    assert( current->first == 0 );
    ++current;
    assert( current->first == 1 );
    ++current;
    assert( current->first == 2 );
    ++current;
    assert( current->first == 3 );
    ++current;
    assert( current->first == 4 );
    ++current;
    assert( current->first == 5 );
    ++current;
    assert( current->first == 6 );
}

//
// test_iterators_iterate_not_empty_storage_empty_cache
//
void test_iterators_iterate_not_empty_storage_empty_cache()
{
    typedef AssocVector< int, int > AssocVector;
    typedef AssocVector::iterator Iterator;

    AssocVector av;
    av.reserve( 32 );

    for( std::size_t i = 0 ; i < 1024 ; ++ i ){
        av.insert( AssocVector::value_type(  i, 0 ) );
    }

    assert( av.size() == 1024 );
    assert( av.bufferSize() == 0 );
    assert( av.storageSize() == 1024 );

    Iterator current = av.begin();
    Iterator const end = av.end();

    for( int i = 0 ; current != end ; ++ current, ++i ){
        assert( current->first == i );
    }
}

void test_erase_iterator()
{
    typedef AssocVector< int, int > AVII;

    AVII av;

    av[ 1 ] = 11;
    av[ 2 ] = 22;
    av[ 3 ] = 33;
    av[ 4 ] = 44;

    av.merge();

    {
        av.erase( 5 );

        AVII::iterator current = av.begin();

        assert( ( * current ).first == 1 && ( * current ).second == 11 );

        ++ current;
        assert( ( * current ).first == 2 && ( * current ).second == 22 );

        ++ current;
        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( ( * current ).first == 4 && ( * current ).second == 44 );

        ++ current;
        assert( current == av.end() );
    }
    {
        av.erase( 4 );

        AVII::iterator current = av.begin();

        assert( ( * current ).first == 1 && ( * current ).second == 11 );

        ++ current;
        assert( ( * current ).first == 2 && ( * current ).second == 22 );

        ++ current;
        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( current == av.end() );
    }
    {
        av.erase( 1 );

        AVII::iterator current = av.begin();

        assert( ( * current ).first == 2 && ( * current ).second == 22 );

        ++ current;
        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( current == av.end() );
    }
    {
        av.erase( 2 );

        AVII::iterator current = av.begin();

        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( current == av.end() );
    }
    {
        av.erase( 3 );

        AVII::iterator current = av.begin();

        assert( current == av.end() );
    }
}

void test_erase_reverse_iterator()
{
    typedef AssocVector< int, int > AVII;

    AVII av;

    av[ 1 ] = 11;
    av[ 2 ] = 22;
    av[ 3 ] = 33;
    av[ 4 ] = 44;

    av.merge();

    {
        av.erase( 5 );

        AVII::reverse_iterator current = av.rbegin();

        assert( ( * current ).first == 4 && ( * current ).second == 44 );

        ++ current;
        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( ( * current ).first == 2 && ( * current ).second == 22 );

        ++ current;
        assert( ( * current ).first == 1 && ( * current ).second == 11 );

        ++ current;
        assert( current == av.rend() );
    }
    {
        av.erase( 4 );

        AVII::reverse_iterator current = av.rbegin();

        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( ( * current ).first == 2 && ( * current ).second == 22 );

        ++ current;
        assert( ( * current ).first == 1 && ( * current ).second == 11 );

        ++ current;
        assert( current == av.rend() );
    }
    {
        av.erase( 1 );

        AVII::reverse_iterator current = av.rbegin();

        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( ( * current ).first == 2 && ( * current ).second == 22 );

        ++ current;
        assert( current == av.rend() );
    }
    {
        av.erase( 2 );

        AVII::reverse_iterator current = av.rbegin();

        assert( ( * current ).first == 3 && ( * current ).second == 33 );

        ++ current;
        assert( current == av.rend() );
    }
    {
        av.erase( 3 );

        AVII::reverse_iterator current = av.rbegin();

        assert( current == av.rend() );
    }
}

template<
      typename _T1
    , typename _T2
>
bool isEqual(
      AssocVector< _T1, _T2 > const & av
    , std::map< _T1, _T2 > const & map
)
{
    if( av.size() != map.size() ){
        return false;
    }

    return std::equal( av.begin(), av.end(), map.begin() );
}

void black_box_test()
{
    typedef AssocVector< int, int > AV;
    AV av;

    typedef std::map< int, int > MAP;
    MAP map;

    assert( isEqual( av, map ) );

    for( int i = 0 ; i < 1024 ; ++ i )
    {
        int const operation = rand() % 5;

        switch( operation )
        {
            case 0:
                {
                    int const key = rand();
                    int const value = rand();

                    av.insert( AV::value_type( key, value ) );
                    map.insert( AV::value_type( key, value ) );

                    assert( isEqual( av, map ) );
                }

                break;

            case 1:
                {
                    int const key = rand();

                    AV::iterator foundAV = av.find( key );
                    MAP::iterator foundMap = map.find( key );

                    assert(
                           ( foundAV == av.end() && foundMap == map.end() )
                        || ( * foundAV == * foundMap )
                    );

                    assert( isEqual( av, map ) );
                }

                break;

            case 2:
                {
                    int const key = rand();

                    av.erase( key );
                    map.erase( key );

                    assert( isEqual( av, map ) );
                }

            case 3:
                {
                    int const key = rand();

                    AV::iterator foundAV = av.find( key );
                    MAP::iterator foundMap = map.find( key );

                    if( foundAV == av.end() && foundMap == map.end() )
                    {
                        // empty
                    }
                    else
                    {
                        av.erase( foundAV );
                        map.erase( foundMap );
                    }

                    assert( isEqual( av, map ) );
                }

                break;

            case 4:
                {
                    int const key = rand();
                    int const value = rand();

                    AV::iterator foundAV = av.find( key );
                    MAP::iterator foundMap = map.find( key );

                    av[ key ] = value;
                    map[ key ] = value;

                    assert( isEqual( av, map ) );
                }

                break;

        }
    }
}

int main()
{
    black_box_test();

    test_CmpByFirst_1();
    test_CmpByFirst_2();
    test_CmpByFirst_3();

    test_isBetween();

    test_constructRange_destroyRange();

    test_copyRange_empty_array();
    test_copyRange_self_copy();
    test_copyRange_between_different_containers();
    test_copyRange_overlap_less_then_half();
    test_copyRange_overlap_more_than_half();
    test_copyRange_overlap_copy_to_begining();

    test_merge_1();
    test_merge_2();
    test_merge_3();
    test_merge_4();
    test_merge_5();
    test_merge_6();
    test_merge_7();

    test_push_back();

    test_insert_in_random_order();
    test_insert_in_increasing_order();
    test_insert_in_decreasing_order();

    test_find_1();
    test_find_2();
    test_count();

    test_erase_1();
    test_erase_2();

    test_erase_iterator();
    test_erase_reverse_iterator();

    test_operator_index_1();
    test_operator_index_2();

    test_user_type();

    test_copy_constructor();
    test_assign_operator();

    test_clear();

    test_iterator_to_const_iterator_conversion();

    test_iterators_equal();
    test_reverse_iterators_equal();

    test_iterators_distance();
    test_reverse_iterators_distance();

    test_iterators_begin_equals_end_in_empty_container();
    test_reverse_iterators_begin_equal_end_in_empty_storage();

    test_iterators_increment_decrement();

    test_iterator_1();

    test_iterators_iterate_not_empty_storage_empty_cache();

    std::cout << "OK" << std::endl;

    return 0;
}

