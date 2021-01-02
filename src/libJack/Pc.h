#ifndef _PC_H_
#define _PC_H_

#include "CircularQ.h"
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
        std::vector<double> readi;
        bool flag;
        bool wrtFlag;
        //CircularQ
        xeyes::CircularQ<std::vector<double>> queue;
};

#endif