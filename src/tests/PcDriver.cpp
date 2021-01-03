#include "../libJack/Pc.cpp"
#include <unistd.h>
#include <cstdlib>

int main() {
    /*
    PcQ test(100,10);

    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(1.0,2.0);
    std::vector<double> vec;
    std::vector<double> readi;

    for(int i=0; i<12;i++) {
        for(uint32_t i=0; i<100;i++) {
            vec.push_back(std::round(dist(e2)));
        }
        test.wrt(&vec);
        vec.clear();
    }

    assert(test.read(&readi) == true);
    assert(test.read(&readi) == true);
    assert(test.read(&readi) == true);
    assert(test.read(&readi) == true);
    */
    Pc test(100,10,1.0,2.0);
    test.startP();
    test.startC();
    sleep(20);
    test.quit();

    return 0;
}