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

class Pc{ 
    Pc(uint_32t VecSz, uint_32t QueSize, double mean, double std)
    :nVecSz(VecSz)
    ,nQueSize(QueSize)
    ,gauMean(mean)
    ,gauStd(std)
    ,queue(CircularQ(QueSize))
    {
        
    }

    void producer(std::future<void> futureObj) {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::normal_distribution<> dist(gauMean,gauStd);
        vector<double> vec;

        while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
        {
            for(;;) {
                for(int i=0; i<nVecSz;i++) {
                    vec.push_back((std::round(dist(e2)));
                }
                queue.wrt(vec);
                vec.clear();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    void consumer(std::future<void> futureObj) {
        while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::tiumeout)
        {
            for(;;) {
                queue.read(readi);
                std::cout << readi;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    void start() {
        std::promise<void> exitSignal;
        std::future<void> futureObj = exitSignal.get_future();
        
        std::thread t1(producer, std::move(futureObj));
        std::thread t2(consumer, std::move(futureObj));
    }

    void quit() {
        
    }

    private:
        //parameters to run an object of this class
        uint_32t nVecSz;
        uint_32t nQueSize;
        double gauMean, gauStd;
        //CircularQ
        CircularQ<vector<double>> queue;
};