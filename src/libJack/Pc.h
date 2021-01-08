#ifndef _PC_H_
#define _PC_H_

#include "CircularQ.h"
#include "GauQ.h"
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
        void start();
        void quit();
        void meanStd(std::vector<double> v);
    private:
        uint32_t nVecSz;
        uint32_t nQueSz;
        double gauMean, gauStd;
        std::vector<std::thread> threads;
        Gau readi;
        bool flag;
        //CircularQ
        GauQ queue;
};

#endif