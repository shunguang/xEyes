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
    public:
        Pc(const uint32_t VecSz, const uint32_t QueSize, const double mean, const double std)
        :nVecSz(VecSz)
        ,nQueSize(QueSize)
        ,gauMean(mean)
        ,gauStd(std)
        ,exitSignal()
        ,exitSignal1()
        ,threads(std::vector<std::thread>())
        ,readi(std::vector<double>())
        ,queue(xeyes::CircularQ<std::vector<double>>(QueSize))
        {

        }

        void producer(std::future<void> futureObj) {
            std::random_device rd;
            std::mt19937 e2(rd());
            std::normal_distribution<> dist(gauMean,gauStd);
            std::vector<double> vec;
            while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
            {
                for(uint32_t i=0; i<nVecSz;i++) {
                    vec.push_back((std::round(dist(e2))));
                }
                queue.wrt(&vec);
                std::cout << "Write";
                vec.clear();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }

        void consumer(std::future<void> futureObj1) {
            while (futureObj1.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
            {
                queue.read(&readi);
                std::cout << "Read";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }

        void start() {
            std::promise<void> exitSignal;
            std::shared_future<void> futureObj = exitSignal.get_future();
            std::promise<void> exitSignal1;
            std::shared_future<void> futureObj1 = exitSignal1.get_future();

            std::thread t1(producer, std::move(futureObj));
            std::thread t2(consumer, std::move(futureObj1));
            threads.push_back(t1);
            threads.push_back(t2);
        }

        void quit() {
            for(auto &th : threads) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
            }
            
            exitSignal.set_value();
            exitSignal1.set_value();

            for(auto &th : threads) {
                th.join();
            }
        }

    private:
        //parameters to run an object of this class
        uint32_t nVecSz;
        uint32_t nQueSize;
        double gauMean, gauStd;
        std::vector<std::thread> threads;
        std::vector<double> readi;
        std::promise<void> exitSignal;
        std::promise<void> exitSignal1;
        //CircularQ
        xeyes::CircularQ<std::vector<double>> queue;
};