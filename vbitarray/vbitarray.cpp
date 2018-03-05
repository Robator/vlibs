#include "vbitarray.h"


/*
    src     sign-norm   sign-dop     u-n    u-dop

 00 000    0 000(0)     0 000(0)     0 t  0 000(0)
 00 001    0 001(1)     1 111(-1)    0 r  0 111(7)
 00 010    0 010(2)     1 110(-2)    0 i  0 110(6)
 00 011    0 011(3)     1 101(-3)    0 v  0 101(5)
 11 100    1 100(-4)    1 100(-4)    0 i  0 100(4)
 11 101    1 101(-3)    0 011(3)     0 a  0 011(3)
 11 110    1 110(-2)    0 010(2)     0 l  0 010(2)
 11 111    1 111(-1)    0 001(1)     0 !  0 001(1)
*/



//=======================================================================================
VBitArray::VBitArray()
{}
//=======================================================================================
VBitArray::VBitArray( const std::string &data_to_bits, const _MostSignificant most )
{
    for ( auto ch: data_to_bits )
    {
        push( ch, most );
    }
}
//=======================================================================================
void VBitArray::push( char val, const VBitArray::_MostSignificant most )
{
    for ( int i = 8; i > 0; --i )
    {
        if ( most == MostSignificant )
        {
            _bools.push( val & 0x80 );
            val <<= 1;
        }
        else
        {
            _bools.push( val & 0x01 );
            val >>= 1;
        }
    } // for each bit.
}
//=======================================================================================
int32_t VBitArray::pop_i( int cnt )
{
    return _pop_( cnt, int32_t(front() ? -1 : 0) );
}
//=======================================================================================
int32_t VBitArray::pop_i_dop( int cnt )
{
    auto res = pop_i( cnt );
    int32_t sign_except = -(1 << (cnt - 1));
    return res == sign_except ? sign_except : -res;
}
//=======================================================================================
uint32_t VBitArray::pop_u(int cnt)
{
    return _pop_( cnt, uint32_t(0) );
}
//=======================================================================================
uint32_t VBitArray::pop_u_dop( int cnt )
{
    auto res = pop_u( cnt );
    uint32_t max1_except = 1 << cnt;
    return res == 0 ? 0 : max1_except - res;
}
//=======================================================================================
int VBitArray::remained_bits() const
{
    return int(_bools.size());
}
//=======================================================================================
bool VBitArray::front() const
{
    return _bools.front();
}
//=======================================================================================
bool VBitArray::empty() const
{
    return _bools.empty();
}
//=======================================================================================
void VBitArray::_check_cnt(int cnt) const
{
    if ( cnt <= 0 || cnt > 32 )
        throw std::logic_error("VBitArray: bits count is not correct.");

    if ( cnt > int(_bools.size()) )
        throw std::out_of_range("VBitArray: bits count less than need.");
}
//=======================================================================================
//int32_t VBitArray::_pop( int cnt, bool in_dop_code )
//{
//    _check_cnt( cnt );

//    int32_t res = in_dop_code ? -1 : 0;
//    while (cnt--)
//    {
//        res <<= 1;
//        if ( _bools.front() )
//            res |= 1;
//        _bools.pop();
//    }
//    return res;
//}
//=======================================================================================
