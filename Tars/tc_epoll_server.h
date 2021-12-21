#ifndef TARS_V_TC_EPOLL_SERVER_H
#define TARS_V_TC_EPOLL_SERVER_H

#include <vector>
#include <map>
#include <list>

#include "tc_socket.h"
#include "tc_epoller.h"
#include "tc_thread.h"
#include "tc_thread_queue.h"

namespace tars
{
    class TC_EpollServer
    {
        struct tagRecvData
        {
            uint32_t        uid;            /**连接标示*/
            std::string          buffer;         /**需要发送的内容*/
            std::string          ip;             /**远程连接的ip*/
            uint16_t        port;           /**远程连接的端口*/
            int64_t         recvTimeStamp;  /**接收到数据的时间*/
            bool            isOverload;     /**是否已过载 */
            bool            isClosed;       /**是否已关闭*/
            int                fd;                /*保存产生该消息的fd，用于回包时选择网络线程*/
            // BindAdapterPtr  adapter;        /**标识哪一个adapter的消息*/
            int             closeType;     /*如果是关闭消息包，则标识关闭类型,0:表示客户端主动关闭；1:服务端主动关闭;2:连接超时服务端主动关闭*/
        };

        struct tagSendData
        {
            char            cmd;            /**命令:'c',关闭fd; 's',有数据需要发送*/
            uint32_t        uid;            /**连接标示*/
            std::string          buffer;         /**需要发送的内容*/
            std::string          ip;             /**远程连接的ip*/
            uint16_t        port;           /**远程连接的端口*/
        };

        typedef TC_ThreadQueue<tagRecvData*, std::deque<tagRecvData*> > recv_queue;
        typedef TC_ThreadQueue<tagSendData*, std::deque<tagSendData*> > send_queue;
        typedef recv_queue::queue_type recv_queue_type;

    public:
        TC_EpollServer();
        virtual ~TC_EpollServer();

        class NetThread
        {
        public:
            enum
            {
                ET_LISTEN = 1,
                ET_CLOSE  = 2,
                ET_NOTIFY = 3,
                ET_NET    = 0,
            };

            struct
            {
                std::string response;
                uint32_t uid;
            }_response;

            NetThread(TC_EpollServer *epollServer);
            virtual ~NetThread();

            void bind(std::string &ip, int port);
            void createEpoll(uint32_t iIndex = 0);
            void insertRecvQueue(const recv_queue::queue_type &vtRecvData,bool bPushBack = true);
            void send(unsigned int uid, const std::string &s, const std::string &ip, uint16_t port);

            bool waitForRecvQueue(tagRecvData* &recv, uint32_t iWaitTime);

        public:
            TC_ThreadLock               monitor;

        private:
            TC_EpollServer *_epollServer;
            TC_Epoller _epoller;

            TC_Socket _shutdown;
            TC_Socket _notify;
            TC_Socket _bind_listen;

            recv_queue                _rbuffer;
            send_queue                 _sbuffer;

            std::map<int,int> _listen_connect_id;
            std::list<uint32_t> _free;
            volatile size_t _free_size;

        };

        class Handle : public TC_Thread, public TC_ThreadLock
        {
        public:
            Handle();
            virtual ~Handle();

            virtual void run();
            void setEpollServer(TC_EpollServer *eserv);

            bool waitForRecvQueue(tagRecvData* &recv, uint32_t iWaitTime);

            virtual void initialize() {};

        protected:
            TC_EpollServer *_pEpollServer;
            int tmp = 10;

            virtual void handleImp();

        }; //Handle


    public:
        TC_EpollServer::NetThread *getNetThread()
        {
            return _netThreads;
        }

    private:
        NetThread *_netThreads;
    };

}

#endif //TARS_V_TC_EPOLL_SERVER_H