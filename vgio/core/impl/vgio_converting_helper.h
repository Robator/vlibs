#ifndef NCONVERTINGHELPER_H
#define NCONVERTINGHELPER_H

#include <vector>
#include <string>

//=======================================================================================
namespace vgio
{
    //===================================================================================
    namespace _impl
    {
        //===============================================================================
        //  Вызывает g_strfreev в случае необходимости.
        std::vector<std::string> convert_and_free( char ** list );
        //===============================================================================
        //  Вызывает g_free в случае необходимости.
        std::string convert_and_free( char * raw );
        //===============================================================================

    } // _impl namespace
    //===================================================================================

} // vgio namespace
//=======================================================================================



#endif // NCONVERTINGHELPER_H
