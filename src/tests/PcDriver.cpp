#include "../libJack/Pc.cpp"

int main() {
    Pc test(1000,100,1.0,2.0);
    test.start();
    test.quit();
    return 0;
}