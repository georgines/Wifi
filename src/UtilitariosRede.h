#ifndef WIFI_UTILITARIOS_REDE_H
#define WIFI_UTILITARIOS_REDE_H

#include <cstddef>
#include <cstdint>

#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"

class UtilitariosRede {
public:
    static constexpr uint32_t TEMPO_PADRAO_RESOLUCAO_DNS_MS = 5000U;
    static constexpr uint32_t INTERVALO_POLL_DNS_MS = 10U;

    static bool resolverNomeHostParaIp(const char *nome_host, ip4_addr_t &endereco_saida,
                                       uint32_t tempo_limite_ms = TEMPO_PADRAO_RESOLUCAO_DNS_MS);
    static bool converterTextoParaIp4(const char *texto_endereco, ip4_addr_t &endereco_saida);
    static bool converterIp4ParaTexto(const ip4_addr_t &endereco_origem, char *destino_texto,
                                      size_t tamanho_texto);

private:
    struct ContextoResolucaoDns;

    static void tratarRespostaDns(const char *nome_host, const ip_addr_t *endereco_resolvido,
                                  void *contexto_usuario);
};

#endif
