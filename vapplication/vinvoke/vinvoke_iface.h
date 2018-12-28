#ifndef VINVOKE_IFACE_H
#define VINVOKE_IFACE_H

#include <functional>
#include "vcompiler.h"

//=======================================================================================
/*  22-11-2018  by Elapidae
 *
 *  CRTP контракт: Derived должен иметь метод do_invoke( InvokeFunc&& );
 *
*/
//=======================================================================================

#if (V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS)
//=======================================================================================
template<typename Derived>
class VInvoke_Iface
{
public:
    using InvokeFunc = std::function<void()>;

    // Function type casting, use it as: some.invoke( [](int i){ vdeb << i; }, 42 );
    template< typename Fn, typename ... Args >
    void invoke( Fn fn, const Args& ... args );

    // Class method casting, use it as: some.invoke( &cls, &Cls::foo, 42 );
    template< typename Cls, typename RetType, typename ... Args >
    void invoke( Cls *cls, RetType(Cls::*func)(Args...), const Args& ... args );

protected: ~VInvoke_Iface() = default;
}; // VInvoke_Iface
//=======================================================================================


//=======================================================================================
//      IMPLEMENTATION
//=======================================================================================
template<typename Derived>
template< typename Fn, typename ... Args >
void VInvoke_Iface<Derived>::invoke( Fn fn, const Args& ... args )
{
    InvokeFunc f = [=](){ fn(args...); };

    auto& d = static_cast<Derived&>(*this);
    d.do_invoke( std::move(f) );
}
//=======================================================================================
template<typename Derived>
template< typename Cls, typename RetType, typename ... Args >
void VInvoke_Iface<Derived>::invoke( Cls *cls, RetType(Cls::*func)(Args...),
                                     const Args& ... args )
{
    InvokeFunc f = [=](){ (cls->*func)(args...); };

    auto& d = static_cast<Derived&>(*this);
    d.do_invoke( std::move(f) );
}
//=======================================================================================
#else // V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS
    #include "vinvoke/old_compiler_vinvoke_iface.h"
#endif // V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS
//=======================================================================================


#endif // VINVOKE_IFACE_H
