#include "../libUtil/CircularQ.h"
#include "../libJack/CircularQ.h"
#include "../libDc/YuvFrm_h.h"
#include "../libDc/YuvFrm_h.cpp"
#include <iostream> 
#include <assert.h>

class YuvCircularQ: public CircularQ<YuvFrm_hPtr>
{
    public:

        YuvCircularQ(const uint32_t imgW, const uint32_t imgH, const uint32_t nTotItems, const std::string &name)
            : m_w(imgW)
            , m_h(imgH)
            , m_v()
            , m_q()
            , m_name(name)
            , m_wrtDropCnt(0)
        {
            allocQ(nTotItems);
        }

        ~YuvCircularQ() {
            freeQ();
        }

        virtual void allocQ(const uint32_t nTotItems) {
            std::lock_guard<std::mutex> lock(m_mutexRW);

            m_items = nTotItems;
            m_q.clear();
            for(uint32_t i=0;i<m_items;i++) {
                YuvFrm_hPtr p( new YuvFrm_h(m_w,m_h,0) );
                p->fn_ = i;
                m_q.push_back(p);
            }
            m_v.resize(m_items,0);
            m_headW = 0;
            m_headR = 0;
        }

        uint32_t m_w;
        uint32_t m_h;
        uint32_t m_items; //predefined # of elements in queue
            uint32_t m_headW; //write index
            uint32_t m_headR; //read index

            std::vector<YuvFrm_hPtr> m_q; //queue represented as vector
            std::vector<int> m_v; //count the wrt (++) / read(--) activities in m_q[i] index

            std::mutex m_mutexRW; //exclusive, non-recursive ownership semantics
            std::string m_name; //qname for debugging
            uint32_t m_wrtDropCnt;
};