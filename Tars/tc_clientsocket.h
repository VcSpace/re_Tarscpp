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

    class TC_Endpoint
    {
    public:

    private:

    protected:

    };
} //tars

#endif //RETARS_TC_CLIENTSOCKET_H
