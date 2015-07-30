#include "skiplist.hpp"
#include <string>

int main()
{
    std::cout<<"TEST\n";
    //Test implementation here!
    skiplist::skiplist<long,std::string> mylist;
    mylist.insert(1,"one");
    mylist.insert(2,"two");
    return 0;
}
