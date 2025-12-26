#include "Wifi.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#ifdef HABILITAR_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

Wifi::Wifi() {}

void Wifi::iniciar(const char *ssid, const char *senha)
{
    this->ssid = ssid;
    this->senha = senha;
    cyw43_arch_init();
    reiniciarWifi();
    inicio_tempo = get_absolute_time();
    status_do_link_atual = StatusWifi::Desligado;
}

StatusWifi Wifi::obterStatusWifi()
{
    status_do_link_atual = static_cast<StatusWifi>(cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA));

    if (absolute_time_diff_us(inicio_tempo, get_absolute_time()) > 2000000) // 2 segundos
    {
        if (status_do_link_atual < 0)
        {
            reiniciarWifi();
        }
        inicio_tempo = get_absolute_time();
    }

    return status_do_link_atual;
}

const char *Wifi::obterIp()
{
    const char *ip_str = "";
    if (status_do_link_atual != StatusWifi::Conectado)
    {
        return ip_str;
    }
    cyw43_arch_lwip_begin();
    ip_str = ip4addr_ntoa(netif_ip4_addr(netif_default));
    cyw43_arch_lwip_end();

    return ip_str;
}

const char *Wifi::obterGatewayPadrao()
{
    const char *gw_str = "";
    if (status_do_link_atual != StatusWifi::Conectado)
    {
        return gw_str;
    }
    cyw43_arch_lwip_begin();
    gw_str = ip4addr_ntoa(netif_ip4_gw(netif_default));
    cyw43_arch_lwip_end();
    return gw_str;
}

const char *Wifi::obterMascaraDeRede()
{
    const char *mask_str = "";
    if (status_do_link_atual != StatusWifi::Conectado)
    {
        return mask_str;
    }
    cyw43_arch_lwip_begin();
    mask_str = ip4addr_ntoa(netif_ip4_netmask(netif_default));
    cyw43_arch_lwip_end();
    return mask_str;
}

void Wifi::reiniciarWifi()
{
    cyw43_arch_disable_sta_mode();
    sleep_ms_compatível(100);
    cyw43_arch_enable_sta_mode();
    cyw43_arch_wifi_connect_async(ssid, senha, CYW43_AUTH_WPA2_AES_PSK);
}

void Wifi::sleep_ms_compatível(uint32_t ms)
{
#ifdef HABILITAR_FREERTOS
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    sleep_ms(ms);
#endif
}