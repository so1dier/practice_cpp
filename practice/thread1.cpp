#include <iostream>
#include <thread>

//This function will be called from a thread

void call_from_thread() {
    std::cout << "Hello, World" << std::endl;
}

int main() {
    //Launch a thread
    std::thread t1(call_from_thread);

  std::cout << "Hello from main" << std::endl;
    //Join the thread with the main thread
    t1.join();

  std::cout << "After join" << std::endl;

    return 0;
}
