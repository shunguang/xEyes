#include "../libJack/CircularQ.h"

class PcQ: public xeyes::CircularQ<std::vector<double>>
{
    public:
        PcQ(const uint32_t VecSz, const uint32_t QueSize)
            :CircularQ(QueSize)
            ,nVecSz(VecSz)
            ,nQueSz(QueSize)
        {
            allocQ(QueSize);
        }

        PcQ() {
        }

        ~PcQ() {
            freeQ();
        }

        virtual void allocQ(const uint32_t QueSize) {
            std::lock_guard<std::mutex> lock(m_mutexRW);

            m_q.clear();
            for(uint32_t i=0;i<QueSize;i++) {
                std::vector<double> p(nVecSz,0.0);
                m_q.push_back(p);
            }
            m_v.resize(QueSize,0);
            m_headW = 0;
            m_headR = 0;
        }

    private:
        uint32_t nVecSz;
        uint32_t nQueSz;
};
