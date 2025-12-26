#ifndef WIFI_WIFI_H
#define WIFI_WIFI_H

#include <cstdint>
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

enum StatusWifi : int
{
    ErroDesconhecido = -4,
    FalhaNaAutenticacao,
    RedeNaoEncontrada,
    Falha,
    Desligado,
    Conectando,
    ConectadoSemIp,
    Conectado,
};

class Wifi
{
public:
    Wifi();
    void iniciar(const char *ssid, const char *senha);
    StatusWifi obterStatusWifi();
    const char *obterIp();
    const char *obterGatewayPadrao();
    const char *obterMascaraDeRede();

private:
    void reiniciarWifi();
    void sleep_ms_compatível(uint32_t ms);
    const char *ssid;
    const char *senha;
    absolute_time_t inicio_tempo;
    StatusWifi status_do_link_atual;
};
#endif
