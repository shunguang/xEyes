#include "../libUtil/CircularQ.h"
#include <iostream>
#include <assert.h>
 
int main()
{
    //for read
    int readi;
    float readf;

    //initialize int queue 
    xeyes::CircularQ<int> test_q(5,"int_q");

    //reset name to intQ
    test_q.resetName("intQ");
    assert(test_q.getName() == "intQ");

    //resize queue to 3
    test_q.resetSize(3);
    assert(test_q.getItems() == 3);

    //write integers into queue
    for(int i=0; i<4;i++) {
        test_q.wrt(&i);
    }

    //read queue
    assert(test_q.read(&readi) == true);
    assert(test_q.read(&readi) == true);
    assert(test_q.read(&readi) == true);
    assert(test_q.read(&readi) == false);

    //reset Queue
    test_q.reset();
    assert(test_q.getHeadW() == 0);
    assert(test_q.getHeadR() == 0);

    //initialize float queue 
    xeyes::CircularQ<float> test_qf(3,"float_q");
    
    //reset name to floatQ
    test_qf.resetName("floatQ");
    assert(test_qf.getName() == "floatQ");

    //write floats into queue
    for(int i=0; i<4;i++) {
        float j = (float)i;
        test_qf.wrt(&j);
    }

    //read queue
    assert(test_qf.read(&readf) == true);
    assert(test_qf.read(&readf) == true);
    assert(test_qf.read(&readf) == true);
    assert(test_qf.read(&readf) == false);

    //reset queue
    test_qf.reset();
    assert(test_qf.getHeadW() == 0);
    assert(test_qf.getHeadR() == 0);

    return 0;
}
