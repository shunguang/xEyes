#include "../libUtil/CircularQ.h"
#include <iostream>
#include <assert.h>

//CircularQ<int>::CircularQ(const uint32_t nTotItems, const std::string &name)
//{
//}

int main()
{
    //initialize int queue 
    xeyes::CircularQ<int> test_q(5,"int_q");
    
    //reset name to intQ
    test_q.resetName("intQ");
    assert(test_q.m_name == "intQ");

    //write integers 1,2,3 into queue
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    test_q.wrt(&one);
    test_q.wrt(&two);
    test_q.wrt(&three);

    //read queue
    assert(test_q.read(&one) == true);
    assert(test_q.read(&two) == true);
    assert(test_q.read(&three) == true);
    assert(test_q.m_q[0] == 1);
    assert(test_q.m_q[1] == 2);
    assert(test_q.m_q[2] == 3);

    //reset Queue
    test_q.reset();
    assert(test_q.m_headW == 0);
    assert(test_q.m_headR == 0);
    assert(test_q.m_items == 0);

    return 0;
}
