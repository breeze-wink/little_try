#include "server_socket.h"
#include "event_poller.h"
#include <cerrno>
#include <sys/epoll.h>
using namespace breeze::socket;

//without SocketHandler
int main()
{
    Singleton<Logger>::Instance() -> open("/home/huang/networkProject/epoll/server.log");

    Serversocket Server("127.0.0.1", 8080);

    EventPoller epoll;
    epoll.create(1024);
    u_int32_t events = EPOLLIN|EPOLLHUP|EPOLLERR|EPOLLONESHOT;
    epoll.add(Server.fd(), events);

    while (true) 
    {
        int num = epoll.wait(1000); 
        if (num < 0)
        {
            log_error("epoll wait error: errno = %d, errmsg = %s", errno, strerror(errno));
            break;
        }
        else if (num == 0)
        {
            log_debug("epoll wait timeout");
            continue;
        }
        for (int i = 0; i < num; ++ i)
        {
            if (epoll.get_fd(i) == Server.fd())
            {
                //服务端套接字可读
                int connfd = Server.accept();

                //将连接套接字添加到监听队列
                struct epoll_event ev2;
                
                epoll.add(connfd, events);
            }
            else
            {
                Socket client(epoll.get_fd(i));
                if (epoll.is_set(i, EPOLLHUP))
                {   
                    //连接套接字挂断了
                    log_error("socket hang up by peer: errno = %d, errmsg = %s", errno, strerror(errno));
                    epoll.del(client.fd());  
                    client.close();                 
                }
                else if (epoll.is_set(i, EPOLLERR))
                {   
                    //连接套接字出现错误  
                    log_error("socket error: connfd = %d, errno = %d, errmsg = %s", client.fd(), errno, strerror(errno));
                    epoll.del(client.fd()); 
                    client.close();
                }
                else if (epoll.is_set(i, EPOLLIN))
                {
                    epoll.del(client.fd()); 
                    char buf[1024] = {0};
                    int len = client.recv(buf, sizeof(buf));

                    if (len < 0)
                    {
                        log_error("socket connection abort: connfd = %d, errno = %d, errmsg = %s", client.fd(), errno, strerror(errno));
                        client.close();
                    }
                    else if (len == 0)
                    {
                        log_debug("socket closed by peer: connfd = %d", client.fd());
                        client.close();
                    }
                    else
                    {
                        log_debug("recv: connfd = %d, msg = %s", client.fd(), buf);
                        client.send(buf, sizeof(buf));
                        epoll.add(client.fd(), events);
                    }
                }
            }
        }
    }
}