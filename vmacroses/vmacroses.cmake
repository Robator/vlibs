#========================================================================================
# vmacroses.cmake
#
#
# Этот файл сгенерирован автоматически.
#
# Вносить изменения можно между строк:
#   #<<< Start your code here
#   Сохраненный код.
#   #>>> Stop your code here
# Все остальные изменения будут перезаписаны.
#
#========================================================================================


#========================================================================================

if ( NOT  VMACROSES_INCLUDED )
    set ( VMACROSES_INCLUDED TRUE )

    message( "Include vmacroses..." )

    #<<< Start your code here -----------------------------------------------------------
    #>>> Stop your code here ------------------------------------------------------------

    include_directories( "${VLIBS_DIR}/vmacroses/")

    
    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vmacroses/vcompiler.h") 
    

    message( "vmacroses included" )

endif()
# vmacroses.cmake
#========================================================================================