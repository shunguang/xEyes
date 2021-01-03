#ifndef _PC_H_
#define _PC_H_

#include "CircularQ.h"
#include "../tests/PcQ.cpp"
#include <thread>
#include <random>
#include <iomanip>
#include <map>
#include <vector>
#include <chrono>
#include <assert.h>
#include <iostream>
#include <future>
#include <numeric>
#include <unistd.h>

class Pc{
    public:
        Pc(const uint32_t VecSz, const uint32_t QueSize, const double mean, const double std);
        ~Pc();
        void producer();
        void consumer();
        void startP();
        void startC();
        void quit();
        void meanStd(std::vector<double> v);
    private:
        uint32_t nVecSz;
        uint32_t nQueSz;
        double gauMean, gauStd;
        std::vector<std::thread> threads;
        std::vector<double> readi;
        bool flag;
        bool wrtFlag;
        bool readFlag;
        uint32_t nItems;
        //CircularQ
        PcQ queue;
};

#endif