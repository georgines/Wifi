add_library(Wifi
        src/Wifi.cpp
        src/UtilitariosRede.cpp
)

target_include_directories(Wifi PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(Wifi PUBLIC
        pico_stdlib
)

if(TARGET FreeRTOS)
    target_compile_definitions(Wifi PUBLIC
            HABILITAR_FREERTOS_LWIP=1
    )

    target_link_libraries(Wifi PUBLIC
            FreeRTOS
            pico_cyw43_arch_lwip_sys_freertos
    )

else()
    target_link_libraries(Wifi PUBLIC
            pico_cyw43_arch_lwip_threadsafe_background
    )
endif()
