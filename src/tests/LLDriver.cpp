#include "LLNodeQ.cpp"

int main() {
     //for read
    Node readi;

    //for initialization
    Node* next = new Node(0);

    //initialize int queue 
    LLNodeQ test_q(10,next,5,"node_q");

    //reset name to nodeQ
    test_q.resetName("nodeQ");
    assert(test_q.getName() == "nodeQ");

    //resize queue to 3
    test_q.resetSize(3);
    assert(test_q.getItems() == 3);

    //write nodes into queue
    for(int i=0; i<4;i++) {
        Node* p = new Node(i);
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