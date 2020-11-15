#ifndef _CIRCULAR_Q_H_
#define _CIRCULAR_Q_H_

namespace xeyes {
    //Question: Since allocQ and wrt call two different classes would it be correct
    //to instantiate the template class with two generic types? Or do we use the shared_ptr?
    template <class T, class T1>
    class CircularQ
    {
        private:
            uint32_t m_items; //predefined # of elements in queue
            uint32_t m_headW; //write index
            uint32_t m_headR; //read index

            std::vector<T> m_q; //queue represented as vector
            std::vector<int> m_v; //count the wrt (++) / read(--) activities in m_q[i] index

            std::mutex m_mutexRW; //exclusive, non-recursive ownership semantics
            std::string m_name; //qname for debugging
            uint32_t m_wrtDropCnt;
        protected:
        //Question: Do we use our shared_ptr or generic type?
            void allocQ(const uint32_t nTotItems) {
                std::mutex::scoped_lock lock(m_mutexRW) // scoped lock

                m_items = nTotITems;
                m_q.clear();
                for(uint32_t i=0;i<m_items;i++) {
                    CircularQ_hPtr p;
                    m_q.push_back(p);
                }
                m_v.resize(m_items,0);

            }
            void freeQ() {
                std::mutex:scoped_lock lock(m_mutexRW);

                m_q.clear();
                m_v.clear();
                m_headR = 0;
                m_headW = 0;
            }
        public:
            //Question: How would we know what attributes x has? ie. the constructor
            //I have right now is a combination of the one in YuvFrm_h and yuvCircularQ_h
            //What else would I need to change/add?
            CircularQ(T &x, const uint32_t nTotItems, const std::string &name)
            : m_v()
            , m_q()
            , m_name(name)
            , m_wrtDropCnt(0)
            {
                allocQ(nTotItems);
            }
            ~CircularQ() {
                freeQ();
            }
            void resetName(const std::string &name) {
                std::mutex::scoped_lock lock(m_mutexRW);
                n_name = name;
            }
            void resetSize(const uint32_t nTotItems) {
                freeQ();
                allocQ(nTotItems);
            }
            void reset() {
                std::mutex::scoped_lock lock(m_mutexRW);
                m_headW = 0;
                m_headR = 0;
                m_v.resize(m_items,0);
            }
            //Question: For read and write, is the instantiation of dst correct?
            bool wrt(T *src) {
                bool sucWrt = false;
                {
                    std::mutex::scoped_lock lock(m_mutexRW);
                    uint32_t &idx = m_headW;
                    if (cnt == 0) {
                        T1 *dst = m_q[idx].get();
                        *dst = *src;
                        cnt++;
                        ++idx;
                        if(idx >= m_items) {
                            idx = 0;
                        }
                        sucWrt = true;
                    }
                }
                if(!suxWrt) {
                    m_wrtDropCnt++;
                    if(m_wrtDropCnt > 999) {
                        m_wrtDropCnt = 0;
                    }
                }
                return sucWrt;
            }
            bool read(T *dst) {
                bool hasData = false;
                {
                    std::mutex::scoped_lock lock(m_mutexRW);
                    uint32_t &idx = m_headR;
                    int &cnt = m_v[idx];
                    if(cnt > 0) {
                        T1 *src = m_q[idx]
                        *dst = *src;

                        cnt = 0;
                        hasData = ture;
                        ++idx;
                        if(idx >= m_items) {
                            idx = 0;
                        }
                    }
                }
                return hasData;
            }
        typedef std::shared_ptr<T> CircularQ_hPtr;

    };
}
#endif