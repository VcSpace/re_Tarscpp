#ifndef RETARS_TC_EX_H
#define RETARS_TC_EX_H

#include <stdexcept>

namespace tars
{
    class TC_Exception : public exception
    {
    public:
        TC_Exception();
        virtual ~TC_Exception();

    private:
    };

} //tars

#endif //RETARS_TC_EX_H
