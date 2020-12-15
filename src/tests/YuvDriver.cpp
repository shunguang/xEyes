#include "YuvCircularQ.cpp"
#include "../libDc/YuvFrm_h.h"
#include "../libUtil/DataTypes.h"
#include "../libUtil/"
#include "../libDc/"

int main() {
    //initialize YuvFrm queue 
    YuvCircularQ test_q(2,2,5,"yuv_q");
    
    //reset name to yuvQ
    test_q.resetName("yuvQ");
    assert(test_q.m_name == "yuvQ");

    //write yuv
    YuvFrm_hPtr one( new YuvFrm_h(2,2,0) ); 
    YuvFrm_hPtr two( new YuvFrm_h(3,3,0) ); 
    test_q.wrt(&one);
    test_q.wrt(&two);

    //read queue
    assert(test_q.read(&one) == true);
    assert(test_q.read(&two) == true);

    //reset Queue
    test_q.reset();
    assert(test_q.m_headW == 0);
    assert(test_q.m_headR == 0);
    assert(test_q.m_items == 0);
}
