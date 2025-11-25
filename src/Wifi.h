#ifndef WIFI_WIFI_H
#define WIFI_WIFI_H

#include <cstdint>

#include "lwip/ip4_addr.h"
#include "pico/cyw43_arch.h"

class Wifi {
public:
    static constexpr uint8_t TAMANHO_ENDERECO_MAC = 6U;
    static constexpr uint8_t TAMANHO_MAXIMO_SSID = 32U;

    struct RedeWifiDisponivel {
        char nomeRede[TAMANHO_MAXIMO_SSID + 1U]{};
        int16_t intensidadeSinalDbm = 0;
        uint8_t canalRede = 0U;
        bool possuiProtecao = false;
        uint8_t enderecoBssid[TAMANHO_ENDERECO_MAC]{};
    };

    explicit Wifi(const char *ssid_rede, const char *senha_rede);
    ~Wifi();

    bool iniciar();
    bool garantirConexao();
    bool estaConectado() const;
    ip4_addr_t obterIpLocal() const;
    ip4_addr_t obterGatewayPadrao() const;
    ip4_addr_t obterMascaraSubrede() const;
    const char *obterIpLocalComoTexto() const;
    const char *obterGatewayPadraoComoTexto() const;
    const char *obterMascaraSubredeComoTexto() const;

private:
    static constexpr uint32_t TEMPO_INICIAL_MS = 20000U;
    static constexpr uint32_t TEMPO_RECONEXAO_MS = 10000U;
    static constexpr uint32_t ESPERA_ENTRE_TENTATIVAS_MS = 500U;
    static constexpr uint32_t TOTAL_TENTATIVAS = 5U;
    static constexpr uint8_t TAMANHO_TEXTO_IP = IP4ADDR_STRLEN_MAX;

    static Wifi *instanciaAtiva;

    bool iniciarRadio();
    bool conectarRede(uint32_t tempo_limite_ms);
    const char *converterEnderecoParaTexto(const ip4_addr_t &endereco_origem) const;

    const char *ssid;
    const char *senha;
    bool radioInicializado;
    mutable char textoEnderecoTemporario[TAMANHO_TEXTO_IP]{};
};

#endif
