#include "skiplist.hpp"
#include "randomizer.hpp"
#include <string>
#include <chrono>
#include <functional>
#include <map>

using namespace std::chrono;

template<typename K, typename V>
using dataContainer = std::vector<std::pair<K, V>>;

namespace {
enum class generator_mode {
    generate_random,
    generate_sorted,
    generate_rev_sorted,
};
}

template<typename K, typename V>
void generate_random_data ( dataContainer<K, V> &p_container,
                            long p_amount ,
                            generator_mode p_mode = generator_mode::generate_random )
{
    std::cout << "Dont know what to do\n";
}

template<>
void generate_random_data<long, long> ( dataContainer<long, long> &p_container,
                                        long p_amount ,
                                        generator_mode p_mode )
{
    myrandom::random<long> rnd ( 0, std::sqrt ( std::numeric_limits<long>::max() ) );
    std::vector<long> keys, values;
    if ( p_mode == generator_mode::generate_sorted ) {
        rnd.fill_with_sorted_random ( keys, p_amount, true );
    } else if ( p_mode == generator_mode::generate_rev_sorted ) {
        rnd.fill_with_sorted_random ( keys, p_amount, true );
        std::reverse ( begin ( keys ), end ( keys ) );
    } else {
        rnd.fill_with_random ( keys, p_amount, true );
    }
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

template<typename K, typename V>
auto bench_insertion ( std::map<K, V> &p_map,
                       dataContainer<K, V> &p_data )
{
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data ) {
        p_map[e.first] = e.second;
    }
    auto timeEnd = high_resolution_clock::now();
    return duration_cast<milliseconds> ( timeEnd - timeStart ).count();
}

template<typename K, typename V>
auto bench_searching ( std::map<K, V> &p_map,
                       dataContainer<K, V> &p_data )
{
    bool searchFailed {false};
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data ) {
        if ( p_map.find ( e.first ) == p_map.end() ) {
            searchFailed = true;
        }
    }
    auto timeEnd = high_resolution_clock::now();
    return std::make_pair ( searchFailed,
                            duration_cast<milliseconds> ( timeEnd - timeStart ).count() );
}

void run_benchmark_map()
{
    long amount {1024};
    do {
        dataContainer<long, long> data;
        std::map<long, long> stl_map;
        skiplist::skiplist<long, long> slist;
        generate_random_data ( data, amount );
        auto slist_insertion_time = bench_insertion ( slist, data );
        auto slist_searching_time = bench_searching ( slist, data );
        auto map_insertion_time = bench_insertion ( stl_map, data );
        auto map_searching_time = bench_searching ( stl_map, data );
        std::cout << amount << "\t" << slist_insertion_time << "\t" << slist_searching_time.second\
                  << "\t" << map_insertion_time << "\t" << map_searching_time.second << std::endl;
        amount *= 2;
    } while ( amount <= 1024 * 1024 );
}

int main()
{
    run_benchmark_map();
    return 0;
}
