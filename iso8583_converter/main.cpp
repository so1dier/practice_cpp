#include "new_msg.h"

#include <iostream>
#include <fstream>



char * m100 = "..0100367b4481a8e0a0040030000000000000010000000000010306113155794066679523001131550306030603065542051000645183606579934364363589140610119D49122011483243400006766892442217634300300001200789164 MOBIL MANUREWA         AUCKLAND       NZ554036048VisaBaseSrc EMSink      496945394624EMVisa     ";


void OpenFile()
{
    std::fstream fs;
    fs.open ("test.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs << " more lorem ipsum";
    fs.close();
}


int main()
{
    CAS2805Msg msg(m100);
    msg.PackMessage();
    //msg.UnpackMessage(m100);
}
