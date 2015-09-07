#include "skiplist.hpp"
#include "randomizer.hpp"
#include <string>
#include <chrono>
#include <functional>
#include <map>
#include <sstream>
#include <fstream>

using namespace std::chrono;

template<typename K, typename V>
using dataContainer = std::vector<std::pair<K, V>>;

namespace
{
enum class generator_mode
{
    generate_random,
    generate_sorted,
    generate_rev_sorted,
    generate_almost_sorted,
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
    myrandom::random rnd ( 0, std::sqrt ( std::numeric_limits<long>::max() ) );
    std::vector<long> keys, values;
    if( p_mode == generator_mode::generate_almost_sorted)
    {
        rnd.fill_with_almost_sorted(keys, p_amount, true);
    }
    else if ( p_mode == generator_mode::generate_sorted )
    {
        rnd.fill_with_sorted_random ( keys, p_amount, true );
    }
    else if ( p_mode == generator_mode::generate_rev_sorted )
    {
        rnd.fill_with_sorted_random ( keys, p_amount, true );
        std::reverse ( begin ( keys ), end ( keys ) );
    }
    else
    {
        rnd.fill_with_random ( keys, p_amount, true );
    }
    rnd.fill_with_random ( values, p_amount );
    for ( long i {0}; i < p_amount; i++ )
    {
        p_container.emplace_back ( std::make_pair ( keys[i], values[i] ) );
    }
}

template<typename C>
auto bench_insertion ( C &p_container,
                       dataContainer < typename C::key_type,
                       typename C::mapped_type > &p_data )
{
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data )
    {
        p_container.insert (e.first, e.second);
    }
    auto timeEnd = high_resolution_clock::now();
    return duration_cast<milliseconds> ( timeEnd - timeStart ).count();
}


template<typename K, typename V>
auto bench_insertion ( std::map<K, V> &p_map,
                       dataContainer<K, V> &p_data )
{
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data )
    {
        p_map[e.first] = e.second;
    }
    auto timeEnd = high_resolution_clock::now();
    return duration_cast<milliseconds> ( timeEnd - timeStart ).count();
}


inline
bool check_search_result(const std::map<long, long>::iterator &p_value,
                         const long &p_expected)
{
    return p_value->second == p_expected;
}

inline
bool check_search_result(const long *p_value,
                         const long &p_expected)
{
    return *p_value == p_expected;
}

template<typename C>
auto bench_searching ( C &p_container,
                       dataContainer < typename C::key_type,
                       typename C::mapped_type > &p_data )
{
    bool search_failed {false};
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data )
    {
        auto value = p_container.find(e.first);
        if(not check_search_result(value, e.second))
        {
            std::cerr << "SEARCH FAILED!\n";
            exit(1);
            search_failed = true;
        }
    }
    auto timeEnd = high_resolution_clock::now();
    return std::make_pair ( search_failed,
                            duration_cast<milliseconds> ( timeEnd - timeStart ).count() );
}

template<typename C>
auto bench_erase ( C &p_map,
                   dataContainer < typename C::key_type,
                   typename C::mapped_type > &p_data )
{
    bool eraseFailed {false};
    auto timeStart = high_resolution_clock::now();
    for ( auto & e : p_data )
    {
        if ( p_map.erase ( e.first ) == 0 ) //Or 0
        {
            eraseFailed = true;
        }
    }
    auto timeEnd = high_resolution_clock::now();
    return std::make_pair ( eraseFailed,
                            duration_cast<milliseconds> ( timeEnd - timeStart ).count() );
}

void write_input_data(dataContainer<long, long> &data)
{
    std::ofstream out_file("input_data.txt");
    if(out_file)
    {
        long n {0};
        for(auto e : data)
        {
            out_file << "(" << n++ << "," << e.first << "),";
        }
        std::cout << "Input data saved (" << data.size() << ")\n";
    }
}

void run_benchmark ( long p_start, long p_limit, float p_mult )
{
    long amount {p_start};
    std::vector<long> amounts;
    std::stringstream container_size;
    container_size << "[";
    while ( 1 )
    {
        amounts.push_back ( amount );
        container_size << amount;
        amount *= p_mult;
        if ( amount > p_limit )
        {
            break;
        }
        container_size << ",";
    }
    container_size << "]";
    std::stringstream insertion_data, searching_data, erase_data;
    insertion_data << "[";
    searching_data << "[";
    erase_data << "[";
    long last_size = amounts.back();
    for ( auto size : amounts )
    {
        std::cout << "Current size: " << size << "\r";
        std::cout.flush();
        dataContainer<long, long> data;
        std::map<long, long> stl_map;
        skiplist::skiplist<long, long> slist;
        generate_random_data ( data, size, generator_mode::generate_almost_sorted );
        auto slist_insertion_time = bench_insertion ( slist, data );
        auto slist_searching_time = bench_searching ( slist, data );
        auto slist_erase_time = bench_erase ( slist, data );
        auto map_insertion_time = bench_insertion ( stl_map, data );
        auto map_searching_time = bench_searching ( stl_map, data );
        auto map_erase_time = bench_erase ( stl_map, data );
        insertion_data << "(" << slist_insertion_time << "," << map_insertion_time << "),";
        searching_data << "(" << slist_searching_time.second << "," << map_searching_time.second << "),";
        erase_data << "(" << slist_erase_time.second << "," << map_erase_time.second << "),";
        if(last_size == size)
        {
            write_input_data(data);
        }
    }
    std::cout << "Completed!\n";
    insertion_data << "]";
    searching_data << "]";
    erase_data << "]";
    //Output the data to the relative files
    std::ofstream insertion_data_file ( "insertion_data.txt" ),
        searching_data_file ( "searching_data.txt" ),
        erase_data_file("erase_data.txt"),
        test_size_file ( "test_size_data.txt" );
    if ( not insertion_data_file || not searching_data_file || not test_size_file || not erase_data_file )
    {
        std::cerr << "Unable to open the output files!\n";
    }
    else
    {
        test_size_file << container_size.str().c_str() << std::endl;
        insertion_data_file << insertion_data.str().c_str() << std::endl;
        searching_data_file << searching_data.str().c_str() << std::endl;
        erase_data_file << erase_data.str().c_str() << std::endl;
        std::cout << "Output written!\n";
    }
}

int main ( int argc, char **argv )
{
    long first {1024}, limit { 1024 * 512 };
    float mult {1.2};
    if ( argc < 4 )
    {
        std::cout << "Start from: " << first << ", limit:" << limit << ", multiplier: " << mult << std::endl;
    }
    else
    {
        first = atoi ( argv[1] );
        limit = atoi ( argv[2] );
        mult = atof ( argv[3] );
    }
    if ( first <= 0 || limit <= 0 || mult <= 0 )
    {
        std::cerr << "Please provide some valid arguments!\n";
    }
    else
    {
        run_benchmark ( first, limit, mult );
    }
    return 0;
}
