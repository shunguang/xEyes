#include "../libJack/CircularQ.h"
#include <iostream>
#include <assert.h>
#ifndef _LL_Q_
#define _LL_Q_
class Node {
    public:
        int data;
        Node* next;

        Node(int d) : data(d) {}
        Node(int d, Node* n) : data(d) , next(n) {}
        Node() {}
};

class LLNodeQ: public xeyes::CircularQ<Node*>
{
    public:

        virtual void allocQ(const uint32_t nTotItems) {
            std::lock_guard<std::mutex> lock(m_mutexRW);

            m_items = nTotItems;
            m_q.clear();
            for(uint32_t i=0;i<m_items;i++) {
                Node* p = new Node(0, NULL);
                m_q.push_back(p);
            }
            m_v.resize(m_items,0);
            m_headW = 0;
            m_headR = 0;
        }

        uint32_t m_data;
        Node m_next;
};
#endif