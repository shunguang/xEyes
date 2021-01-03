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
    ,readFlag(true)
    ,nItems(0)
{
    PcQ queue(VecSz,QueSize);
}

Pc::~Pc(){}

void Pc::producer() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(gauMean,gauStd);
    std::vector<double> vec;
    flag = true;
    while (flag)
    {
        if(nItems >= nQueSz) {
            wrtFlag = false;
            nItems = 0;
        }
        if(wrtFlag) {
            for(uint32_t i=0; i<nVecSz;i++) {
                vec.push_back(std::round(dist(e2)));
            }
            queue.wrt(&vec);
            readFlag = true;
            nItems++;
            std::cout << "Write";
            vec.clear();
        }
    }
}

void Pc::consumer() {
    while (flag)
    {
        while(readFlag) {
            bool readElem = queue.read(&readi);
            if(!readElem) {
                wrtFlag = true;
                readFlag = false;
                return;
            }
            nItems--;
            std::cout << "Read";
            meanStd(readi);
        }
    }
}

void Pc::startP() {
    flag = true;
    std::thread t1(&Pc::producer,this);
    threads.push_back(std::move(t1));
}

void Pc::startC() {
    flag = true;
    std::thread t2(&Pc::consumer,this);
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
