#ifndef RETARS_TC_EX_H
#define RETARS_TC_EX_H

#include <stdexcept>
#include <cstring>

namespace tars
{
    class TC_Exception : public std::exception
    {
    public:
        /**
         * @brief 构造函数，提供了一个可以传入errno的构造函数，
         *
         *        异常抛出时直接获取的错误信息
         *
         * @param buffer 异常的告警信息
         */
        explicit TC_Exception(const std::string &buffer);

        /**
         * @brief 构造函数,提供了一个可以传入errno的构造函数，
         *
         *        异常抛出时直接获取的错误信息
         *
         * @param buffer 异常的告警信息
         * @param err    错误码, 可用strerror获取错误信息
         */
        TC_Exception(const std::string &buffer, int err);

        /**
         * @brief 析够数函
         */
        virtual ~TC_Exception() throw();

        /**
         * @brief 错误信息.
         *
         * @return const char*
         */
        virtual const char* what() const throw();

        /**
         * @brief 获取错误码
         *
         * @return 成功获取返回0
         */
        int getErrCode() { return _code; }

    private:
        void getBacktrace();

    private:
        /**
         * 异常的相关信息
         */
         std::string _buffer;

        /**
         * 错误码
         */
        int     _code;

    };

} //tars

#endif //RETARS_TC_EX_H
