#include "tc_clientsocket.h"


namespace tars
{

    TC_Endpoint::TC_Endpoint() : _host(""), _port(20999)
    {

    }

    TC_Endpoint::TC_Endpoint(const std::string &str, int port)
    {
        init(str, port);
    }

    void TC_Endpoint::init(const std::string &str, int port)
    {
        _host = str;
        _port = port;
    }

    TC_Endpoint::~TC_Endpoint() {

    }


} //tars