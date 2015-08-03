#include <iostream>
//Remove all the stuff before this comment
#include <memory>
#include <random>

namespace skiplist {
template<typename KEY, typename VALUE>
struct node {
    KEY   key;
    VALUE value;
    short height;
    std::unique_ptr<node<KEY, VALUE>*[]> links;
    node() = default;
    node ( short p_height ) :
        height {p_height} {
        links = std::make_unique<node<KEY, VALUE>*[]> ( p_height );
    }
};

constexpr short g_defaultMaxHeight {16};

template < typename KEY,
         typename VALUE >
class skiplist {
    using NODE = node<KEY, VALUE>;
    std::random_device         m_rd;
    std::default_random_engine m_re;
    NODE                      *m_head;

    short m_maxLevel;
    float m_pFactor;
    NODE *createNode();
    void insert_impl ( NODE *cur, NODE *newNode , short lvl );
    NODE *search_impl ( NODE *p_cur, KEY &p_key , short p_lvl );
public:
    skiplist ( short p_maxLevel = g_defaultMaxHeight , float p_pFactor = .6 );
    ~skiplist();
    void  insert ( KEY p_key, VALUE p_value );
    VALUE search ( KEY &p_key );
};

template<typename KEY, typename VALUE>
skiplist<KEY, VALUE>::skiplist ( short p_maxLevel , float p_pFactor ) :
    m_re ( m_rd() ), m_maxLevel {p_maxLevel}, m_pFactor {p_pFactor} {
    m_head = new NODE ( m_maxLevel );
}

template<typename KEY, typename VALUE>
skiplist<KEY, VALUE>::~skiplist()
{
    NODE *cur = m_head;
    do {
        NODE *toDelete = cur;
        cur = cur->links[0];
        delete toDelete;
    } while ( cur != nullptr );
}

template<typename KEY, typename VALUE>
typename skiplist<KEY, VALUE>::NODE *skiplist<KEY, VALUE>::createNode()
{
    short lvl {1};
    while ( ( std::generate_canonical<double, 16> ( m_re ) <= m_pFactor ) && ( lvl < m_maxLevel ) ) {
        ++lvl;
    }
    return new NODE ( lvl );
}

template<typename KEY, typename VALUE>
void skiplist<KEY, VALUE>::insert_impl ( skiplist::NODE *cur,
        skiplist::NODE *newNode,
        short lvl )
{
    if ( cur->links[lvl] == nullptr || cur->links[lvl]->key > newNode->key ) {
        if ( lvl < newNode->height ) {
            newNode->links[lvl] = cur->links[lvl];
            cur->links[lvl] = newNode;
        }
        if ( lvl == 0 ) {
            return;
        }
        insert_impl ( cur, newNode, lvl - 1 );
        return;
    }
    insert_impl ( cur->links[lvl], newNode, lvl );
}

template<typename KEY, typename VALUE>
void skiplist<KEY, VALUE>::insert ( KEY p_key, VALUE p_value )
{
    NODE *newNode = createNode();
    newNode->value = p_value;
    newNode->key = p_key;
    insert_impl ( m_head, newNode, m_maxLevel - 1 );
}

template<typename KEY, typename VALUE>
typename skiplist<KEY, VALUE>::NODE *skiplist<KEY, VALUE>::search_impl ( skiplist<KEY, VALUE>::NODE *p_cur,
        KEY &p_key ,
        short p_lvl )
{
    if ( p_cur == nullptr ) {
        return nullptr;
    }
    if ( p_cur->key == p_key ) {
        return p_cur;
    }
    if ( p_cur->links[p_lvl] == nullptr || p_cur->links[p_lvl]->key > p_key ) {
        if ( p_lvl == 0 ) {
            return nullptr;
        }
        return search_impl ( p_cur, p_key, p_lvl - 1 );
    }
    return search_impl ( p_cur->links[p_lvl], p_key, p_lvl );
}

template<typename KEY, typename VALUE>
VALUE skiplist<KEY, VALUE>::search ( KEY &p_key )
{
    auto node = search_impl ( m_head, p_key, m_maxLevel - 1 );
    return ( node == nullptr ) ? -1 : node->value;
}
}

