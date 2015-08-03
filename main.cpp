#include "skiplist.hpp"
#include "randomizer.hpp"
#include <string>
#include <chrono>
#include <functional>

using namespace std::chrono;

template<typename K, typename V>
using dataContainer = std::vector<std::pair<K, V>>;

template<typename K, typename V>
void generate_random_data ( dataContainer<K, V> &p_container,
                            long p_amount )
{
    std::cout << "Dont know what to do\n";
}

template<>
void generate_random_data<long, long> ( dataContainer<long, long> &p_container,
                                        long p_amount )
{
    myrandom::random<long> rnd ( 0, std::sqrt ( std::numeric_limits<long>::max() ) );
    std::vector<long> keys, values;
    rnd.fill_with_random ( keys, p_amount, true );
    rnd.fill_with_random ( values, p_amount );
    for ( long i {0}; i < p_amount; i++ ) {
        p_container.emplace_back ( std::make_pair ( keys[i], values[i] ) );
    }
}

template<typename K, typename V>
auto bench_insertion ( skiplist::skiplist<K, V> &p_sl,
                       dataContainer<K, V> &p_data )
{
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data ) {
        p_sl.insert ( e.first, e.second );
    }
    std::cout << std::endl;
    auto timeEnd = high_resolution_clock::now();
    return duration_cast<milliseconds> ( timeEnd - timeStart ).count();
}

template<typename K, typename V>
auto bench_searching ( skiplist::skiplist<K, V> &p_sl,
                       dataContainer<K, V> &p_data )
{
    bool searchFailed {false};
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data ) {
        if ( p_sl.search ( e.first ) != e.second ) {
            searchFailed = true;
        }
    }
    auto timeEnd = high_resolution_clock::now();
    return std::make_pair ( searchFailed,
                            duration_cast<milliseconds> ( timeEnd - timeStart ).count() );
}

int main()
{
    dataContainer<long, long> data;
    generate_random_data ( data, 1024 * 128 );
    skiplist::skiplist<long, long> mylist;
    std::cout << "Insertion time: " << bench_insertion ( mylist, data ) << "ms\n";
    auto searchResult = bench_searching ( mylist, data );
    std::cout << "Searching time: " << searchResult.second << "ms ";
    if ( searchResult.first ) {
        std::cout << "(with search failures!)\n";
    } else {
        std::cout << std::endl;
    }
    return 0;
}
