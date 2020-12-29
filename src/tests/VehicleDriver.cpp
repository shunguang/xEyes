#include "VehicleQ.cpp"

int main() {
     //for read
    Vehicle readi;

    //initialize int queue 
    xeyes::CircularQ<Vehicle*> test_q(5,"vehicle_q");

    //reset name to nodeQ
    test_q.resetName("vehicleQ");
    assert(test_q.getName() == "vehicleQ");

    //resize queue to 3
    test_q.resetSize(3);
    assert(test_q.getItems() == 3);

    //write nodes into queue
    for(int i=0; i<4;i++) {
        Vehicle* p = new Vehicle("123",250);
        p->position = i;
        test_q.wrt(&p);
    }

    //read queue
    assert(test_q.read(&readi) == true);
    assert(test_q.read(&readi) == true);
    assert(test_q.read(&readi) == true);
    assert(test_q.read(&readi) == false);

    //reset Queue
    test_q.reset();
    assert(test_q.getHeadW() == 0);
    assert(test_q.getHeadR() == 0);

    return 0;
}