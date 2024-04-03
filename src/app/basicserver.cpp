#include <cerrno>
#include <iostream>
#include <sys/epoll.h>
#include "server_socket.h"

using namespace breeze::socket;
constexpr int MAX_CONN = 1024;

int main()
{
    Singleton<Logger>::Instance() -> open("/home/huang/networkProject/epoll/server.log");

    Serversocket Server("127.0.0.1", 8080);


    //1.创建epoll实例
    int epfd = epoll_create(MAX_CONN);

    if (epfd < 0)
    {
        log_error("epoll create error: errno = %d, errmsg = %s", errno, strerror(errno));
        return -1;
    }

    //2.将服务端套接字添加到监听队列

    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLERR|EPOLLHUP;
    ev.data.fd = Server.fd();

    epoll_ctl(epfd, EPOLL_CTL_ADD, Server.fd(), &ev);


    //3.接受内核返回的就绪事件列表
    struct epoll_event events[MAX_CONN];
    while (true)
    {
        int num = epoll_wait(epfd, events, MAX_CONN, -1);
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
            if (events[i].data.fd == Server.fd())
            {
                //服务端套接字可读
                int connfd = Server.accept();

                //将连接套接字添加到监听队列
                struct epoll_event ev2;
                ev2.events = EPOLLIN|EPOLLHUP|EPOLLERR|EPOLLONESHOT; //有EPOLLONESHOT后触发一次后就不再监听
                ev2.data.fd = connfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev2);
            }
            else
            {
                Socket client(events[i].data.fd);
                if (events[i].events & EPOLLHUP)
                {   
                    //连接套接字挂断了
                    log_error("socket hang up by peer: errno = %d, errmsg = %s", errno, strerror(errno));
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.fd(), nullptr);
                    client.close();                    
                }
                else if (events[i].events & EPOLLERR)
                {   
                    //连接套接字出现错误  
                    log_error("socket error: connfd = %d, errno = %d, errmsg = %s", client.fd(), errno, strerror(errno));
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.fd(), nullptr);
                    client.close();
                }
                else if (events[i].events & EPOLLIN)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.fd(), nullptr);
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
                        struct epoll_event ev3;
                        ev3.events = EPOLLIN|EPOLLERR|EPOLLONESHOT|EPOLLHUP;
                        ev3.data.fd = client.fd();
                        epoll_ctl(epfd, EPOLL_CTL_ADD, client.fd(), &ev3);
                    }
                }
            }
        }
    }

    return 0;
}