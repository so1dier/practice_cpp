// This file is a "Hello, world!" in C++ language by GCC for wandbox.
#include <iostream>
#include <cstdlib>

class Reversal
{
public:
    void execute()
    {
        step1();
        step2();
        step3();
        step4();
        step5();
    }
    
private:
    void step1() { std::cout << "step1_parent ";  }
    void step2() { std::cout << "step2_parent ";  }
    void step3() { std::cout << "step3_parent ";  }
    virtual void step4() = 0;
    virtual void step5() = 0;
};

class MastercardReversal : public Reversal
{
    virtual void step4() {  std::cout << "step4_mastercard ";  }
    virtual void step5() {  std::cout << "step5_mastercard ";  }
};

class HeritageReversal : public Reversal
{
    virtual void step4() {  std::cout << "step4_heritage ";  }
    virtual void step5() {  std::cout << "step5_heritage ";  }
};


int main()
{
    std::cout << "Hello, Wandbox!" << std::endl;
    
    HeritageReversal heritage_reversal;
    MastercardReversal mastercard_reversal;

    Reversal* array[] = { &heritage_reversal, &mastercard_reversal };
    for (auto& reversal : array)
    {
        reversal->execute();
        std::cout << std::endl;
    }
}
