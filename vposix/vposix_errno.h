#ifndef VPOSIX_ERRNO_H
#define VPOSIX_ERRNO_H

#include <string>
#include <stdexcept>
//#include <bits/move.h>

//=======================================================================================
/*  24-09-2018      by Elapidae
 *
 *  Errno -- очередная попытка привести линуховское хозяйство в божеский вид.
 *  Один из аспектов работы с Сишным интерфейсом -- постоянная проверка возвращаемых
 *  величин. Если есть что-нибудь, то это надо, если -1, то была ошибка и иди читай ее из
 *  errno. Чтобы этим не страдать сделано следующее:
 *
*/
//=======================================================================================


//=======================================================================================
namespace vposix
{
    //===================================================================================
    struct Errno final
    {
        static std::string str_error( int err );

        //std::string str_error() const;
        //int err_code;
        //  Проверяет возвращаемое значение, если оно равно -1, то бросается исключение
        //  std::runtime_error( msg ). Если все хорошо, просто возвращается что хотели
        //  (через operator int).
        //Errno( int res );
        //operator int() const;
    };
    //===================================================================================
    //  Кидает исключение в случае ошибки, иначе возвращает результат функции Fn.
    //  Вызывается примерно так: auto fd = vposix::linux_call<int>(::open, O_RDWR);
    template< typename RetType, typename Fn, typename ... Args >
    RetType linux_call( Fn fn, Args ... args );
    //===================================================================================
    //      FACE
    //===================================================================================


    //===================================================================================
    //      IMPLEMENTATION
    //===================================================================================
    template< typename RetType, typename Fn, typename ... Args >
    RetType linux_call( Fn fn, Args ... args )
    {
        auto res = fn( args... );
        static_assert( std::is_same<decltype(res),RetType>::value, "" );

        if ( res != -1 ) return res;

        throw std::runtime_error( Errno::str_error(errno) );
    }
    //===================================================================================
} // namespace vposix
//=======================================================================================


#endif // VPOSIX_ERRNO_H