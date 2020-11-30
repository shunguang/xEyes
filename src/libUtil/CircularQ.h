#ifndef _CIRCULAR_Q_H_
#define _CIRCULAR_Q_H_
#include <stdint.h>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#include <chrono>
#include <iostream>

namespace xeyes {
    template <class T>
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
        //virtual function
            virtual void allocQ(const uint32_t nTotItems) {
                std::scoped_lock lock(m_mutexRW); // scoped lock

                m_items = nTotItems;
                m_q.clear();
                for(uint32_t i=0;i<m_items;i++) {
                    T p; //add parameters for virtual
                    m_q.push_back(p);
                }
                m_v.resize(m_items,0);

            }
            void freeQ() {
                std::scoped_lock lock(m_mutexRW);

                m_q.clear();
                m_v.clear();
                m_headR = 0;
                m_headW = 0;
            }
        public:

           

            //assignment constructor
            CircularQ(const uint32_t nTotItems, const std::string &name)
            : m_v()
            , m_q()
            , m_name(name)
            , m_wrtDropCnt(0)
            {
                allocQ(nTotItems);
            }

            //copy constructor
            CircularQ(T &x) 
            : m_v()
            , m_q()
            , m_name()
            , m_wrtDropCnt(0)
            {
                m_v = x.m_v;
                m_q = x.m_q;
                m_name = x.m_name;
            }

            //destructor
            ~CircularQ() {
                freeQ();
            }

            void resetName(const std::string &name) {
                std::scoped_lock lock(m_mutexRW);
                m_name = name;
            }
            void resetSize(const uint32_t nTotItems) {
                freeQ();
                allocQ(nTotItems);
            }
            void reset() {
                std::scoped_lock lock(m_mutexRW);
                m_headW = 0;
                m_headR = 0;
                m_v.resize(m_items,0);
            }

            bool wrt(T *src) {
                bool sucWrt = false;
                {
                    std::scoped_lock lock(m_mutexRW);
                    uint32_t &idx = m_headW;
		            int   &cnt = m_v[idx];
                    if (cnt == 0) {
                        m_q[idx] = *src;
                        cnt++;
                        ++idx;
                        if(idx >= m_items) {
                            idx = 0;
                        }
                        sucWrt = true;
                    }
                }
                if(!sucWrt) {
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
                    std::scoped_lock lock(m_mutexRW);
                    uint32_t &idx = m_headR;
                    int &cnt = m_v[idx];
                    if(cnt > 0) {
                        *dst = m_q[idx];
                        cnt = 0;
                        hasData = true;
                        ++idx;
                        if(idx >= m_items) {
                            idx = 0;
                        }
                    }
                }
                return hasData;
            }

    };
}
#endif