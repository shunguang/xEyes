#include "Pc.h"

Pc::Pc(const uint32_t VecSz, const uint32_t QueSize, const double mean, const double std)
    :nVecSz(VecSz)
    ,nQueSz(QueSize)
    ,gauMean(mean)
    ,gauStd(std)
    ,threads(std::vector<std::thread>())
    ,readi(VecSz, mean ,std)
    ,flag(false)
    ,queue(VecSz, QueSize)
{
}

Pc::~Pc(){}


void Pc::producer() {
    while (flag)
    {
        Gau p(nVecSz,gauMean,gauStd);
        queue.wrt(&p);
        p.reset();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Pc::consumer() {
    while (flag)
    {   
        
        bool readElem = queue.read(&readi);
        if(!readElem) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        meanStd(readi.getVec());
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

    std::cout<< "Mean: ";
    std::cout<< mean  << std::endl;
    std::cout<< "Standard Deviation: ";
    std::cout<< stdev << std::endl;
    std::cout<< "\n";
}
