
#include "../libJack/CircularQ.h"
#include <string>
#include <map>
#include <random>

#ifndef __GAU_Q_H__
#define __GAU_Q_H__

class Gau
{
    public:
        Gau(const uint32_t VecSz, const double mean, const double std)
            : gauMean(mean)
            , gauStd(std)
            , nVecSz(VecSz)
            , nVec()
        {
            std::random_device rd;
            std::mt19937 e2(rd());
            std::normal_distribution<> dist(gauMean,gauStd);
            std::vector<double> vec;
            for(uint32_t i=0; i<VecSz;i++) {
                vec.push_back(std::round(dist(e2)));
            }
            nVec = vec;
        }

        Gau() {}

        void reset() {
            this->nVec.clear();
        }

        std::vector<double> getVec() {
            return this->nVec;
        }

    private:
        uint32_t nVecSz;
        double gauMean, gauStd;
        std::vector<double> nVec;
};

class GauQ: public xeyes::CircularQ<Gau>
{
    public:
        GauQ(const uint32_t VecSz, const uint32_t nTotItems)
            :CircularQ(nTotItems)
            ,nQueSz(nTotItems)
            ,nVecSz(VecSz)
        {
        }

        GauQ(GauQ &x)
            : CircularQ(x.getQueSz())
            , nQueSz(x.getQueSz())
            , nVecSz(x.getVecSz())
        {
        }

        ~GauQ() {
            freeQ();
        }

        virtual void allocQ(const uint32_t nTotItems) {
            std::lock_guard<std::mutex> lock(m_mutexRW);

            m_items = nTotItems;
            m_q.clear();
            for(uint32_t i=0;i<nTotItems;i++) {
                Gau p(nVecSz,1.0,2.0);
                m_q.push_back(p);
            }
            m_v.resize(m_items,0);
            m_headW = 0;
            m_headR = 0;
        }

        uint32_t getQueSz() {
            return this->nQueSz;
        }

        uint32_t getVecSz() {
            return this->nVecSz;
        }

    private:
        const uint32_t nVecSz;
        const uint32_t nQueSz;
};
#endif
