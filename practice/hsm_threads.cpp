#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>


std::mutex m;
std::condition_variable cv;
std::string data;
bool ready=false;
bool processed=false;


void WorkerThreadWait()
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT
    cv.wait(lk, []{return ready;});

    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    data += " after processing";

    //send data back to main()
    processed = true;
    std::cout << __FUNCTION__ << " thread signals data processing completed" << std::endl;

    //Manual unlocking is done before notifying, to avoid waking up
    //the waiting rhead onlu to block again 
    lk.unlock();
    cv.notify_one();
}

void WorkerThreadWaitFor()
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT_FOR
    
    if(cv.wait_for(lk, 
                   std::chrono::seconds(1),
                   []{return ready;}))
    
    {
        std::cout << __FUNCTION__ << " thread finished waiting" << std::endl;
    }
    else
    {
        std::cout << __FUNCTION__ << " thread timed out" << std::endl;
    }

    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    data += " after processing";

    //send data back to main()
    processed = true;
    std::cout << __FUNCTION__ << "thread signals data processing completed" << std::endl;

    //Manual unlocking is done before notifying, to avoid waking up
    //the waiting rhead onlu to block again 
    lk.unlock();
    cv.notify_one();
}

void WorkerThreadWaitUntil()
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT_UNTIL
     
    if(cv.wait_until(lk, 
                     std::chrono::system_clock::now() + std::chrono::seconds(1),
                     []{return ready;}))
    {
        std::cout << __FUNCTION__ << " thread finished waiting" << std::endl;
    }
    else
    {
        std::cout << __FUNCTION__ << " thread timed out" << std::endl;
    }

    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    data += " adter processing";

    //send data back to main()
    processed = true;
    std::cout << __FUNCTION__ << " thread signals data processing completed" << std::endl;

    //Manual unlocking is done before notifying, to avoid waking up
    //the waiting rhead onlu to block again 
    lk.unlock();
    cv.notify_one();
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//int WorkerSocket(int argc, char *argv[])
void WorkerSocket(int argc, char *argv[])
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT_FOR
    
    //if(cv.wait_for(lk, 
    //               std::chrono::seconds(1),
    //               []{return ready;}))
    //
    //{
    //    std::cout << __FUNCTION__ << " thread finished waiting" << std::endl;
    //}
    //else
    //{
    //    std::cout << __FUNCTION__ << " thread timed out" << std::endl;
    //}

    cv.wait_for(lk, []{return ready;});
    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    //data += " after processing";
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
        processed = true;
        lk.unlock();
        cv.notify_one();
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        processed = true;
        lk.unlock();
        cv.notify_one();
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        processed = true;
        lk.unlock();
        cv.notify_one();
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        processed = true;
        lk.unlock();
        cv.notify_one();
        error("ERROR connecting");
    }
    int iteration = 0;
    while (1)
    {
        //printf("Please enter the message: ");
        bzero(buffer,256);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("Out of sleeping\n");
        //fgets(buffer,255,stdin);
        sprintf(buffer, "%d", ++iteration);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
        {
            processed = true;
            lk.unlock();
            cv.notify_one();
            error("ERROR writing to socket");
        }
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0)
        {
            processed = true;
            lk.unlock();
            cv.notify_one();
            error("ERROR reading from socket");
        }
        printf(">>reply inside thread: %s\n",buffer);

        data = buffer;

        //send data back to main()
        processed = true;
        std::cout << __FUNCTION__ << "thread signals data processing completed" << std::endl;

        //Manual unlocking is done before notifying, to avoid waking up
        //the waiting rhead onlu to block again 
        lk.unlock();
        cv.notify_one();
    }
    close(sockfd);
}

int main(int argc, char* argv[])
{
    //std::thread worker_wait(WorkerThreadWait);
    //std::thread worker_wait_for(WorkerThreadWaitFor);
    //std::thread worker_wait_until(WorkerThreadWaitUntil);
    std::thread worker_socket(WorkerSocket, argc, argv);

    data = "Example data";
    while(1)
    {
        //send data to worker thread
        {
            std::lock_guard<std::mutex> lk(m);
            ready = true;
            //ready = false;
            std::cout << "main() signals data ready for processing" << std::endl;
        }
        cv.notify_one();

        //wait for worker
        {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, []{return processed;});
        }
    }

    std::cout << "Back in main(), data = " << data << std::endl;

    //worker_wait.join();
    //worker_wait_for.join();
    //worker_wait_until.join();
    worker_socket.join();

}

