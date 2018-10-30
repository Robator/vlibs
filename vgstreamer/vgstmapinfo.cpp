#include "vgstmapinfo.h"

#include <gst/gst.h>
#include "verror.h"

//=======================================================================================
struct _will_GstMapInfo : public GstMapInfo
{
    std::weak_ptr<GstBuffer> wbuf;
};
//=======================================================================================
ssize_t VGstMapInfo::ssize() const
{
    return static_cast<ssize_t>( size() );
}
//=======================================================================================
size_t VGstMapInfo::size() const
{
    return _raw_map_info->size;
}
//=======================================================================================
const u_char *VGstMapInfo::raw_data() const
{
    return _raw_map_info->data;
}
//=======================================================================================
bool VGstMapInfo::flag_read() const
{
    return _raw_map_info->flags & GST_MAP_READ;
}
//=======================================================================================
bool VGstMapInfo::flag_write() const
{
    return _raw_map_info->flags & GST_MAP_WRITE;
}
//=======================================================================================
VGstMapInfo VGstMapInfo::do_map_read( const std::shared_ptr<GstBuffer> &sbuf )
{
    VGstMapInfo res( sbuf );
    gst_buffer_map( sbuf.get(),
                    res._raw_map_info.get(),
                    GST_MAP_READ );
    return res;
}
//=======================================================================================
static void map_info_deleter( _will_GstMapInfo* minfo )
{
    if ( auto buf = minfo->wbuf.lock() )
    {
        gst_buffer_unmap( buf.get(), minfo );
    }
    else
    {
        // throw
        verror << "Buffer has destroyed before unmap memory";
    }
}
//=======================================================================================
VGstMapInfo::VGstMapInfo( const std::weak_ptr<GstBuffer>& wbuf )
    : _raw_map_info( new _will_GstMapInfo, map_info_deleter )
{
    _raw_map_info->wbuf = wbuf;
}
//=======================================================================================

