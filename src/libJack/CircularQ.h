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
        protected:
            uint32_t m_items; //predefined # of elements in queue
            uint32_t m_headW; //write index
            uint32_t m_headR; //read index

            std::vector<T> m_q; //queue represented as vector
            std::vector<int> m_v; //count the wrt (++) / read(--) activities in m_q[i] index

            std::mutex m_mutexRW; //exclusive, non-recursive ownership semantics
            std::string m_name; //qname for debugging
            uint32_t m_wrtDropCnt;

        public:
            virtual void allocQ(const uint32_t nTotItems) {
                std::lock_guard<std::mutex> lock(m_mutexRW); // scoped lock

                m_items = nTotItems;
                m_q.clear();
                for(uint32_t i=0;i<m_items;i++) {
                    T p; //add parameters for virtual
                    m_q.push_back(p);
                }
                m_v.resize(m_items,0);
                m_headR = 0;
                m_headW = 0;
            }

            void freeQ() {
                std::lock_guard<std::mutex> lock(m_mutexRW);

                m_q.clear();
                m_v.clear();
                m_headR = 0;
                m_headW = 0;
            }
            
            //default constructor
            CircularQ() {
            }

            //assignment constructor
            CircularQ(const uint32_t nTotItems, const std::string &name)
            : m_v()
            , m_q()
            , m_name(name)
            , m_wrtDropCnt(0)
            {
                allocQ(nTotItems);
            }

            CircularQ(const uint32_t nTotItems)
            : m_v()
            , m_q()
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
                std::lock_guard<std::mutex> lock(m_mutexRW);
                m_name = name;
            }

            void resetSize(const uint32_t nTotItems) {
                freeQ();
                allocQ(nTotItems);
            }

            void reset() {
                std::lock_guard<std::mutex> lock(m_mutexRW);
                m_headW = 0;
                m_headR = 0;
                m_v.resize(m_items,0);
            }

            bool wrt(const T *src) {
                bool sucWrt = false;
                {
                    std::lock_guard<std::mutex> lock(m_mutexRW);
                    uint32_t &idx = m_headW;
		            int  &cnt = m_v[idx];
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
                    if(m_wrtDropCnt > 0) {
                        m_wrtDropCnt = 0; // pop error message for wrt
                        std::cout << "Write Dropped!\n";
                    }
                }
                return sucWrt;
            }

            bool read(T *dst) {
                bool hasData = false;
                {
                    std::lock_guard<std::mutex> lock(m_mutexRW);
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

            uint32_t getItems() {
                return this->m_items;
            }

            uint32_t getHeadW() {
                return this->m_headW;
            }

            uint32_t getHeadR() {
                return this->m_headR;
            }

            std::string getName() {
                return this->m_name;
            }

    };
}
#endif