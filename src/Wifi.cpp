#include "Wifi.h"

#include <cstdio>

#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"


Wifi *Wifi::instanciaAtiva = nullptr;

Wifi::Wifi(const char *ssid_rede, const char *senha_rede)
    : ssid(ssid_rede), senha(senha_rede), radioInicializado(false)
{
    instanciaAtiva = this;
}

Wifi::~Wifi()
{
    if (instanciaAtiva == this)
    {
        instanciaAtiva = nullptr;
    }
}



bool Wifi::iniciar()
{
    if (!iniciarRadio())
    {
        return false;
    }

    cyw43_arch_enable_sta_mode();

    if (!conectarRede(TEMPO_INICIAL_MS))
    {
        std::printf("Falha ao conectar no Wi-Fi inicial.\n");
        return false;
    }

    return true;
}

bool Wifi::garantirConexao()
{
    if (estaConectado())
    {
        return true;
    }

    std::printf("Reconectando ao Wi-Fi...\n");
    return conectarRede(TEMPO_RECONEXAO_MS);
}

bool Wifi::estaConectado() const
{
    if (!radioInicializado || !netif_default)
    {
        return false;
    }

    return netif_is_up(netif_default) && netif_is_link_up(netif_default);
}

bool Wifi::iniciarRadio()
{
    if (radioInicializado)
    {
        return true;
    }

    int codigo_resultado = cyw43_arch_init();
    if (codigo_resultado != 0)
    {
        std::printf("Falha ao iniciar radio Wi-Fi: %d\n", codigo_resultado);
        return false;
    }

    radioInicializado = true;
    return true;
}

bool Wifi::conectarRede(uint32_t tempo_limite_ms)
{
    if (!radioInicializado)
    {
        return false;
    }

    for (uint32_t tentativa = 0; tentativa < TOTAL_TENTATIVAS; tentativa++)
    {
        int codigo_conexao = cyw43_arch_wifi_connect_timeout_ms(ssid, senha, CYW43_AUTH_WPA2_AES_PSK, tempo_limite_ms);

        if (codigo_conexao == 0)
        {
            if (netif_default)
            {
                std::printf("Wi-Fi conectado. IP: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
            }
            return true;
        }

        std::printf("Tentativa %lu falhou (%d).\n", static_cast<unsigned long>(tentativa + 1U), codigo_conexao);
        sleep_ms(ESPERA_ENTRE_TENTATIVAS_MS);
    }

    return false;
}

ip4_addr_t Wifi::obterIpLocal() const
{
    ip4_addr_t endereco{};
    ip4_addr_set_zero(&endereco);

    if (!radioInicializado || !netif_default)
    {
        return endereco;
    }

    cyw43_arch_lwip_begin();
    ip4_addr_copy(endereco, *netif_ip4_addr(netif_default));
    cyw43_arch_lwip_end();

    return endereco;
}

const char *Wifi::obterIpLocalComoTexto() const
{
    ip4_addr_t endereco = obterIpLocal();
    return converterEnderecoParaTexto(endereco);
}

ip4_addr_t Wifi::obterGatewayPadrao() const
{
    ip4_addr_t endereco{};
    ip4_addr_set_zero(&endereco);

    if (!radioInicializado || !netif_default)
    {
        return endereco;
    }

    cyw43_arch_lwip_begin();
    ip4_addr_copy(endereco, *netif_ip4_gw(netif_default));
    cyw43_arch_lwip_end();

    return endereco;
}

const char *Wifi::obterGatewayPadraoComoTexto() const
{
    ip4_addr_t endereco = obterGatewayPadrao();
    return converterEnderecoParaTexto(endereco);
}

ip4_addr_t Wifi::obterMascaraSubrede() const
{
    ip4_addr_t mascara{};
    ip4_addr_set_zero(&mascara);

    if (!radioInicializado || !netif_default)
    {
        return mascara;
    }

    cyw43_arch_lwip_begin();
    ip4_addr_copy(mascara, *netif_ip4_netmask(netif_default));
    cyw43_arch_lwip_end();

    return mascara;
}

const char *Wifi::obterMascaraSubredeComoTexto() const
{
    ip4_addr_t mascara = obterMascaraSubrede();
    return converterEnderecoParaTexto(mascara);
}

const char *Wifi::converterEnderecoParaTexto(const ip4_addr_t &endereco_origem) const
{
    ip4_addr_t endereco_formatado{};
    ip4_addr_copy(endereco_formatado, endereco_origem);
    ip4addr_ntoa_r(&endereco_formatado, textoEnderecoTemporario, static_cast<int>(TAMANHO_TEXTO_IP));
    return textoEnderecoTemporario;
}
