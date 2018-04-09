#include "vfilelog_threaded.h"

#include "vlogger.h"

using namespace vlog;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"


//=======================================================================================
VGroupFileLog_Threaded::VGroupFileLog_Threaded( const std::string &path,
                                                long one_file_size,
                                                int rotate_files_count )
{
    _thread.finvoke( [=]()
    {
        _log.reset( new VGroupFileLog(path, one_file_size, rotate_files_count) );
    });
}
//=======================================================================================
VGroupFileLog_Threaded::~VGroupFileLog_Threaded()
{
    _thread.finvoke( [this](){ _log.reset(); } );
}
//=======================================================================================
void vlog::VGroupFileLog_Threaded::execute( const vlog::VLogEntry &entry )
{
    _thread.finvoke( [=](){ if (_log) _log->execute(entry); } );
}
//=======================================================================================
void vlog::VGroupFileLog_Threaded::register_self()
{
    VLogger::add_executer( [this](const VLogEntry &e){ execute(e); } );
}
//=======================================================================================
int VGroupFileLog_Threaded::queued_entries() const
{
    return _thread.queue_size();
}
//=======================================================================================


//=======================================================================================
VOneFileLog_Threaded::VOneFileLog_Threaded( const std::string &fname,
                                            long one_file_size,
                                            int rotate_files_count )
{
    _thread.finvoke( [=]()
    {
        _log.reset( new VOneFileLog(fname, one_file_size, rotate_files_count) );
    });
}
//=======================================================================================
VOneFileLog_Threaded::~VOneFileLog_Threaded()
{
    _thread.finvoke( [this](){ _log.reset(); } );
}
//=======================================================================================
void VOneFileLog_Threaded::execute( const VLogEntry &entry )
{
    _thread.finvoke( [=](){ if (_log) _log->execute(entry); } );
}
//=======================================================================================
void VOneFileLog_Threaded::register_self()
{
    VLogger::add_executer( [this](const VLogEntry &e){ execute(e); } );
}
//=======================================================================================
int VOneFileLog_Threaded::queued_entries() const
{
    return _thread.queue_size();
}
//=======================================================================================



#pragma GCC diagnostic pop