#include "vtcpsocket.h"

#include "vposix_network.h"
#include "vposix_core.h"
#include "vposix_files.h"
#include "vposix_alloca.h"

#include "vlog_pretty.h"
#include "verror.h"

#include "vpoll/vpoll.h"

#include <assert.h>

using namespace vposix;

//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class VTcpSocket::Pimpl : public VPoll::EventReceiver
{
public:
    Pimpl( int fd_, VTcpSocket *owner_ );
    ~Pimpl() override;
    void close();

    void event_received( VPoll::EventFlags flags ) override;

    void connection_ok();

    bool is_connected = false;
    int fd;

private:
    VTcpSocket *owner;
};
#pragma GCC diagnostic pop
//=======================================================================================
VTcpSocket::Pimpl::Pimpl( int fd_, VTcpSocket *owner_ )
    : fd( fd_ )
    , owner( owner_ )
{
    VPoll::add_fd( fd, this, VPoll::Direction::InOut );
}
//=======================================================================================
VTcpSocket::Pimpl::~Pimpl()
{
    vtrace << "tcp ~pimpl";
    close();
}
//=======================================================================================
void VTcpSocket::Pimpl::close()
{
    is_connected = false;
    if ( fd < 0 ) return;
    VPoll::del_fd( fd );
    vposix::Files::close( fd );
    fd = -1;
    owner->socket_disconnected();
}
//=======================================================================================
void VTcpSocket::Pimpl::event_received( VPoll::EventFlags flags )
{
    //vdeb << "err" << (flags.take_ERR() ? "Need to see in Qt" : "");
    //vdeb << "hup" << flags.take_HangUp();

    if ( flags.take_RD_HangUp() )
    {
        close();
        return;
    }

    if ( flags.take_OUT() )
        connection_ok();

    if ( flags.take_IN() )
        owner->ready_read();

    //vdeb.hex() << flags.raw();
}
//=======================================================================================
void VTcpSocket::Pimpl::connection_ok()
{
    VPoll::mod_fd( fd, this, VPoll::Direction::In );
    is_connected = true;
    owner->socket_connected();
}
//=======================================================================================


//=======================================================================================
VTcpSocket::VTcpSocket()
{}
//=======================================================================================
VTcpSocket::VTcpSocket( VTcpSocket::Peer *peer )
    : p( new Pimpl(peer->take_fd(),this) )
{
    p->is_connected = true;
}
//=======================================================================================
VTcpSocket::~VTcpSocket()
{}
//=======================================================================================
bool VTcpSocket::is_connected() const
{
    return p && p->is_connected;
}
//=======================================================================================
void VTcpSocket::connect_to_host( VIpAddress addr, uint16_t port )
{
    p.reset();
    auto fd = Socket::socket( Socket::Domain::Inet4, Socket::Type::STREAM );
    Socket::set_out_of_band_data( fd );
    p.reset( new Pimpl(fd, this) );

    auto res = Socket::connect_or_err( fd, addr._get_host(), port );
    if ( res == -1 )
    {
        Errno e ;
        if ( e.operation_in_progress() || e.connection_already_in_progress() )
        {
            return;
            //vtrace << "Need to set in write polling." << e.str();
        }
        else
            e.throw_verror( "VTcpSocket::connect_to_host" );
    }
}
//=======================================================================================
bool VTcpSocket::send( const std::string& data )
{
    if ( !is_connected() ) return false;
    auto res = Socket::send( p->fd, data );
    return res == ssize_t(data.size());
}
//=======================================================================================
VString VTcpSocket::receive_all()
{
    auto buf = vposix::Alloca::allocate<char>( Buffer_Size );
    VString res;

    while(1)
    {
        auto has_read = Socket::_recv_or_err( p->fd, buf, Buffer_Size, 0 );

        if ( has_read == -1 )
        {
            Errno e;
            if ( e.resource_unavailable_try_again() )
                return res;
            e.throw_verror("VTcpSocket::receive_all");
        }

        res.append( buf, buf + has_read );

        if ( has_read < Buffer_Size )
            return res;
    }
}
//=======================================================================================

//=======================================================================================
static void kill_unused_tcp( void* atom_int )
{
    auto ai_ptr = static_cast<std::atomic_int*>(atom_int);
    auto fd = ai_ptr->fetch_or( -1 );
    delete ai_ptr;

    if ( fd < 0 ) return;
    vwarning << "Server side VTcpSocket has not attached";
    Files::close( fd );
}
//=======================================================================================
VTcpSocket::Peer::Peer( int fd )
    : _ptr( new std::atomic<int>(fd), kill_unused_tcp )
{}
//=======================================================================================
int VTcpSocket::Peer::take_fd()
{
    assert( _ptr );
    auto res = _ptr->fetch_or( -1 );
    _ptr.reset();
    return res;
}
//=======================================================================================
