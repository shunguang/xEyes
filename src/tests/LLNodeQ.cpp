#include "../libJack/CircularQ.h"
#include "CircularQ.h"
#include <iostream>
#include <assert.h>

class Node {
    public:
        int data;
        Node* next;

        Node(int d) {data = d;}
        Node() {}
};

class LLNodeQ: public xeyes::CircularQ<Node*>
{
    public:

        LLNodeQ(const uint32_t data, Node* next, const uint32_t nTotItems, const std::string &name)
            : CircularQ(nTotItems, name)
            , m_data(data)
            , m_next(*next)
        {
            allocQ(nTotItems);
        }

        ~LLNodeQ() {
            freeQ();
        }

        virtual void allocQ(const uint32_t nTotItems) {
            std::lock_guard<std::mutex> lock(m_mutexRW);

            m_items = nTotItems;
            m_q.clear();
            for(uint32_t i=0;i<m_items;i++) {
                Node* p = new Node(m_data);
                m_q.push_back(p);
            }
            m_v.resize(m_items,0);
            m_headW = 0;
            m_headR = 0;
        }

        uint32_t m_data;
        Node m_next;
};