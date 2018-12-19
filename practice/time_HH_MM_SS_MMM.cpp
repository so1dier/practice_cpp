

//std::string time_in_HH_MM_SS_MMM()
//{
//    using namespace std::chrono;
//
//    // get current time
//    auto now = system_clock::now();
//
//    // get number of milliseconds for the current second
//    // (remainder after division into seconds)
//    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
//
//    // convert to std::time_t in order to convert to std::tm (broken time)
//    auto timer = system_clock::to_time_t(now);
//
//    // convert to broken time
//    std::tm bt = *std::localtime(&timer);
//
//    std::ostringstream oss;
//
//    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
//    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
//
//    return oss.str();
//}

void log(const std::string message)
{
    {
        //dead lock with mutex locks
        //std::lock_guard<std::mutex> lock(mtx_print_out); 
        //or
       // mtx_print_out.lock();
        
       //auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 
        time_t rawtime;
        time (&rawtime);

        struct tm * timeinfo;
        timeinfo = localtime (&rawtime);
         
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S",timeinfo);

        struct timeval time_ms;
        gettimeofday(&time_ms, NULL);
        //uint64_t millis = (time_ms.tv_sec * (uint64_t)1000) + (time_ms.tv_usec / 1000);
        uint64_t millis = (time_ms.tv_usec / 1000);

        std::cout << buffer << "." << millis << " " << message << std::endl;
        // mtx_print_out.unlock();
//        std::cout << time_in_HH_MM_SS_MMM() << " " << message << std::endl;
     }
}
