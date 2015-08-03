#include <random>
#include <vector>
#include <algorithm>
#include <set>

namespace myrandom {
template<typename T>
class random {
    std::random_device rd;
    std::default_random_engine eng;
    std::normal_distribution<> dist;
    void generate_unique ( std::vector<T> &data, long n ) {
        std::set<long> is_unique;
        while ( n ) {
            long r = dist ( eng );
            if ( is_unique.insert ( r ).second ) {
                --n;
                data[n] = r ;
            }
        }
    }
public:
    random ( long double p_mean, long double p_stddev ) :
        eng ( rd() ),
        dist ( p_mean, p_stddev ) {
    }
    T get_random() {
        return dist ( eng );
    }
    void fill_with_random ( std::vector<T> &p_vec,
                            long p_amount,
                            bool unique = false ) {
        p_vec.resize ( p_amount );
        if ( unique ) {
            generate_unique ( p_vec, p_amount );
        } else {
            for ( long i {0}; i < p_amount; i++ ) {
                p_vec[i] = dist ( eng );
            }
        }
    }
    void fill_with_sorted_random ( std::vector<T> &p_vec,
                                   long p_amount,
                                   bool unique = false ) {
        fill_with_random ( p_vec, p_amount, unique );
        std::sort ( p_vec.begin(), p_vec.end() );
    }
};
}
