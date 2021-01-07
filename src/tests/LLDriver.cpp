#include "../libJack/LLNodeQ.cpp"
#include <assert.h>

int main() {
     //for read
    Node* readi;

    //initialize int queue 
    xeyes::CircularQ<Node*> test_q(5,"node_q");

    //reset name to nodeQ
    test_q.resetName("nodeQ");
    assert(test_q.getName() == "nodeQ");

    //resize queue to 3
    test_q.resetSize(3);
    assert(test_q.getItems() == 3);

    //write nodes into queue
    for(int i=0; i<4;i++) {
        Node* p = new Node(i, NULL);
        test_q.wrt(&p);
    }

    //read queue
    for(int i=0; i<4;i++) {
        bool el = test_q.read(&readi);
        if(el) {
            std::cout<< "read!";
        } else {
            std::cout<< "none";
        }
    }

    //reset Queue
    test_q.reset();
    assert(test_q.getHeadW() == 0);
    assert(test_q.getHeadR() == 0);

    return 0;
}