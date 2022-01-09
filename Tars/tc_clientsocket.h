#ifndef RETARS_TC_CLIENTSOCKET_H
#define RETARS_TC_CLIENTSOCKET_H

#include <string>

#include "tc_ex.h"

namespace tars
{
    struct TC_EndpointParse_Exception : public TC_Exception
    {
        TC_EndpointParse_Exception(const std::string &buffer) : TC_Exception(buffer){};
        ~TC_EndpointParse_Exception() throw() {};
    };

    //接管了原NetThread中的int bind(string& ip, int& port);
    class TC_Endpoint
    {
    public:
        TC_Endpoint();
        TC_Endpoint(const std::string &str, int port);
        ~TC_Endpoint();

        void init(const std::string &str, int port);

        std::string getHost() const
        {
            return _host;
        }

        int getPort() const
        {
            return _port;
        }

    private:

    protected:
        std::string _host;
        int _port;

    };
} //tars

#endif //RETARS_TC_CLIENTSOCKET_H
