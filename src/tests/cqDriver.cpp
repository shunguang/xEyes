#include "../libUtil/CircularQ.h"
#include <iostream>
#include <assert.h>
 
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

    //initialize float queue 
    xeyes::CircularQ<float> test_q(5,"float_q");
    
    //reset name to floatQ
    test_q.resetName("floatQ");
    assert(test_q.m_name == "floatQ");

    //write floats 1.0,2.0,3.0 into queue
    float onef = 1.0;
    float twof = 2.0;
    float threef = 3.0;
    float fourf = 4.0;
    test_q.wrt(&one);
    test_q.wrt(&two);
    test_q.wrt(&three);

    //read queue
    assert(test_q.read(&one) == true);
    assert(test_q.read(&two) == true);
    assert(test_q.read(&three) == true);
    assert(test_q.m_q[0] == 1.0);
    assert(test_q.m_q[1] == 2.0);
    assert(test_q.m_q[2] == 3.0);

    //reset queue
    test_q.reset();
    assert(test_q.m_headW == 0);
    assert(test_q.m_headR == 0);
    assert(test_q.m_items == 0);
    
    return 0;
}
