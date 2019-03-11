#include <future>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <chrono>


class Hsm
{
public:
    enum e_Hsm_Status
    {
        e_hsm_status_CREATED = 0,
        e_hsm_status_CONNECTED,
        e_hsm_status_DISCONNECTED
    };
    Hsm(){}
    Hsm(const std::string name)
        : m_name(name),
          m_status(e_hsm_status_CREATED)
    {}
    ~Hsm(){}

    std::string Name() const { return m_name; }
    e_Hsm_Status Status() const { return m_status; }

    void Status(const e_Hsm_Status status) { m_status = status; }


    void Info() const
    {
        std::cout << "name: " << m_name << " status=" << m_status << std::endl;
    }

private:
    std::string m_name;
    e_Hsm_Status m_status;
};

void ShowAllHsmsInfo(const std::list<Hsm>& hsm_list, std::string name)
{
    std::cout << "List " << name << std::endl;
    for (auto&& hsm : hsm_list)
    {
        hsm.Info();
    }
}

void CheckAndMoveDisconnectedToBadList(std::list<Hsm>& good, std::list<Hsm>& bad)
{
//    for (auto&& hsm : good)
    for (auto&& iter_good = good.begin(); iter_good != good.end(); ++iter_good)
    {
        if (iter_good->Status() == Hsm::e_hsm_status_DISCONNECTED)
        {
            std::cout << "!Found a bad connection" << std::endl;
            iter_good->Info();
            //void list::splice (const_iterator pos, list& source, const_iterator sourcePos)
            bad.splice(bad.end(), good, iter_good);
        }
    }
}

int main()
{
    std::list<Hsm> hsm_list;
    std::list<Hsm> hsm_list_bad;
    auto hsm1 = Hsm("hsm1");

    hsm_list.push_back(hsm1);
    hsm_list.push_back(Hsm("hsm2"));
    hsm_list.push_back(Hsm("hsm3"));


    int count = 0;
    std::chrono::system_clock::time_point ttl = std::chrono::system_clock::now() + std::chrono::seconds(3);
    std::chrono::system_clock::time_point time_of_last_check;


    auto bad_hsm = std::next(hsm_list.begin(), 1);
    bad_hsm->Status(Hsm::e_hsm_status_DISCONNECTED);

//    do
//    {
//        if (time_of_last_check + std::chrono::seconds(1)
//< std::chrono::system_clock::now())
//        {
//            std::cout << "time to check connections" << std::endl;
            CheckAndMoveDisconnectedToBadList(hsm_list, hsm_list_bad);
            ShowAllHsmsInfo(hsm_list, "good");
            ShowAllHsmsInfo(hsm_list_bad, "bad");

//            time_of_last_check = std::chrono::system_clock::now();
//        }
//    }
//    while (ttl > std::chrono::system_clock::now());
}
