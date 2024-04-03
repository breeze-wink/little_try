#pragma once

#include "server_socket.h"
#include "event_poller.h"
#include "echo_task.h"
#include <cstddef>
#include <map>
namespace breeze
{
    namespace socket
    {
        class SocketHandler
        {
            SINGLETON(SocketHandler);

        public:
            void listen(const string& ip, int port);
            void attach(int sockfd);
            void detach(int sockfd);
            void handle(int max_conn, int timeout);

        private:
            Socket* m_server = nullptr;
            EventPoller m_epoll;
        };
    }
}
