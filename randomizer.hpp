#include <random>
#include <vector>
#include <algorithm>
#include <set>

namespace myrandom
{
class random
{
    std::random_device rd;
    std::default_random_engine eng;
    std::normal_distribution<> dist;
    void generate_unique ( std::vector<long> &data, long n )
    {
        std::set<long> is_unique;
        while ( n )
        {
            long r = dist ( eng );
            if ( is_unique.insert ( r ).second )
            {
                --n;
                data[n] = r ;
            }
        }
    }
public:
    random ( long double p_mean, long double p_stddev ) :
        eng ( rd() ),
        dist ( p_mean, p_stddev )
    {
    }
    void fill_with_random ( std::vector<long> &p_vec,
                            long p_amount,
                            bool p_unique = false )
    {
        p_vec.resize ( p_amount );
        if ( p_unique )
        {
            generate_unique ( p_vec, p_amount );
        }
        else
        {
            for ( long i {0}; i < p_amount; i++ )
            {
                p_vec[i] = (long)dist ( eng );
            }
        }
    }
    void fill_with_sorted_random ( std::vector<long> &p_vec,
                                   long p_amount,
                                   bool p_unique = false )
    {
        fill_with_random ( p_vec, p_amount, p_unique );
        std::sort ( p_vec.begin(), p_vec.end() );
    }
    void fill_with_almost_sorted( std::vector<long> &p_vec,
                                  long p_amount,
                                  bool p_unique = false)
    {
        fill_with_sorted_random(p_vec, p_amount, p_unique);
        //std::reverse(p_vec.begin(),p_vec.end());
        //Now let's add some disorder to this ordered set of elments.
        float dis = .0001;
        long max_distance = p_amount * .1;
        for(long n {p_amount * dis}; n >= 0; --n)
        {
            long a, b;
            do
            {
                a = std::abs((long)dist(eng)) % p_amount;
                b = std::abs((long)dist(eng)) % max_distance;
                if((a + b) >= p_amount)
                {
                    b = a - b;
                }
                else
                {
                    b = a + b;
                }
            }
            while(a == b);
            std::swap(p_vec[a], p_vec[b]);
        }
    }
};
}
