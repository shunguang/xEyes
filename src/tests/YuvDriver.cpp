#include "../libDc/YuvFrm_h.h"
#include <iostream>
#include <assert.h>
#include "../libUtil/CircularQ.h"

class YuvCircularQ: public CircularQ<YuvFrm_h>
{
    public:
        YuvCircularQ(const uint32_t imgW, const uint32_t imgH, const uint32_t nTotItems, const std::string &name)
            : m_v()
            , m_q()
            , m_w(imgW)
            , m_h(imgH)
            , m_name(name)
            , m_wrtDropCnt(0)
        {
            allocQ(nTotItems);
        }

        ~YuvCircularQ() {
            freeQ();
        }

        void allocQ(const uint32_t nTotITems) {
            std::lock_guard<YuvFrm_h> lock(m_mutexRW);

            m_items = nTotItems;
            m_q.clear();
            for(uint32_t i=0;i<m_items;i++) {
                YuvFrm_h p = new YuvFrm_h(m_w,m_h,0);
                p->fn_ = i;
                m_q.push_back(p);
            }
            m_v.resize(m_items,0);
            m_headW = 0;
            m_headR = 0;
        }

        uint32_t m_w;
        uint32_t m_h;
};

int main() {
    //initialize YuvFrm queue 
    YuvCircularQ test_q(2,2,5,"yuv_q");
    
    //reset name to yuvQ
    test_q.resetName("yuvQ");
    assert(test_q.m_name == "intQ");

    //write yuv
    YuvFrm_h one = new YuvFrm_h(m_w,m_h,0); 
    YuvFrm_h two = new YuvFrm_h(m_w,m_h,0); 
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
