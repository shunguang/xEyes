#include "Pc.h"


Pc::Pc(const uint32_t VecSz, const uint32_t QueSize, const double mean, const double std)
    :nVecSz(VecSz)
    ,nQueSz(QueSize)
    ,gauMean(mean)
    ,gauStd(std)
    ,threads(std::vector<std::thread>())
    ,readi(std::vector<double>())
    ,flag(false)
    ,wrtFlag(true)
{
    xeyes::CircularQ<std::vector<double>> queue(QueSize);
}

Pc::~Pc(){}

void Pc::producer() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(gauMean,gauStd);
    std::vector<double> vec;
    while (flag)
    {
        if(queue.getItems() >= nQueSz) {
            wrtFlag = false;
        }
        if(wrtFlag) {
            for(uint32_t i=0; i<nVecSz;i++) {
                vec.push_back((std::round(dist(e2))));
            }
            queue.wrt(&vec);
            std::cout << "Write";
            vec.clear();
        }
    }
}

void Pc::consumer() {
    while (flag)
    {
        bool readElem = queue.read(&readi);
        if(!readElem) {
            wrtFlag = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "Read";
        meanStd(readi);
    }
}

void Pc::start() {
    flag = true;
    std::thread t1(&Pc::producer,this);
    std::thread t2(&Pc::consumer,this);
    threads.push_back(std::move(t1));
    threads.push_back(std::move(t2));
}

void Pc::quit() {
    flag = false;
    for(auto &th : threads) {
        th.join();
    }
}

void Pc::meanStd(std::vector<double> v) {
    double sum = std::accumulate(v.begin(),v.end(),0.0);
    double mean = sum/v.size();
    
    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size());

    std::cout<< mean;
    std::cout<< stdev;
}
