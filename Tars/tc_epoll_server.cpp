#include "tc_epoll_server.h"

namespace tars
{
    NetThread::NetThread() {}
    NetThread::~NetThread() {}

    void NetThread::bind(std::string &ip, int port)
    {
        int iSocketType = SOCK_STREAM;
        int iDomain = AF_INET;

        _sock = socket(AF_INET, SOCK_STREAM, 0);

        if(_sock < 0)
        {
            std::cerr << "bind _sock faild" << std::endl;
        }

        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if(ip == "")
        {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else{
            parseAddr(ip, addr.sin_addr);
        }
    }

    void NetThread::parseAddr(const int &ip, struct in_addr &seraddr)
    {
        int ret = inet_pton(AF_INET, ip.c_str(), &seraddr);
        if(ret < 0)
        {
            std::cerr << "set ip error" << std::endl;
        }
        else if(ret == 0) //域名
        {
            struct hostent stHostent;
            struct hostent *pstHostent;
            /*
             * http://xsh8637.blog.163.com/blog/static/24099666201081634847407/
             * struct hostent{
    char *h_name;  //official name
    char **h_aliases;  //alias list
    int  h_addrtype;  //host address type
    int  h_length;  //address lenght
    char **h_addr_list;  //address list
}
             */
            char buf[4096] = "\0";
            int iError;

            gethostbyname_r(ip.c_str(), &stHostent, buf, sizeof(buf), &pstHostent, &iError);
            if (pstHostent == NULL)
            {
                cout<<"gethostbyname_r error! :"<<endl;
            }
            else
            {
                stSinAddr = *(struct in_addr *) pstHostent->h_addr;
            }

        }

    }

}