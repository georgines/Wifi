#include "UtilitariosRede.h"

#include "lwip/dns.h"
#include "lwip/err.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

struct UtilitariosRede::ContextoResolucaoDns {
    bool resolvido = false;
    err_t codigoResultado = ERR_INPROGRESS;
    ip_addr_t enderecoRecebido{};
};

bool UtilitariosRede::resolverNomeHostParaIp(const char *nome_host, ip4_addr_t &endereco_saida,
                                             uint32_t tempo_limite_ms)
{
    ip4_addr_set_zero(&endereco_saida);

    bool host_invalido = nome_host == nullptr;
    if (host_invalido)
    {
        return false;
    }

    ContextoResolucaoDns contexto{};
    ip_addr_set_zero_ip4(&contexto.enderecoRecebido);

    cyw43_arch_lwip_begin();
    err_t resultado_dns = dns_gethostbyname(nome_host, &contexto.enderecoRecebido,
                                            tratarRespostaDns, &contexto);
    cyw43_arch_lwip_end();

    bool resolvido_imediatamente = resultado_dns == ERR_OK;
    if (resolvido_imediatamente)
    {
        contexto.codigoResultado = ERR_OK;
        contexto.resolvido = true;
    }

    bool solicitacao_em_andamento = resultado_dns == ERR_INPROGRESS;
    if (!solicitacao_em_andamento && !resolvido_imediatamente)
    {
        return false;
    }

    if (!contexto.resolvido)
    {
        uint32_t tempo_inicial_ms = to_ms_since_boot(get_absolute_time());
        while (!contexto.resolvido)
        {
            uint32_t tempo_atual_ms = to_ms_since_boot(get_absolute_time());
            uint32_t tempo_decorrido_ms = tempo_atual_ms - tempo_inicial_ms;
            bool excedeu_tempo_limite = tempo_decorrido_ms >= tempo_limite_ms;
            if (excedeu_tempo_limite)
            {
                break;
            }

            cyw43_arch_poll();
            sleep_ms(INTERVALO_POLL_DNS_MS);
        }
    }

    if (!contexto.resolvido)
    {
        return false;
    }

    bool endereco_e_ipv4 = IP_IS_V4(&contexto.enderecoRecebido);
    if (!endereco_e_ipv4)
    {
        return false;
    }

    ip4_addr_t *endereco_convertido = ip_2_ip4(&contexto.enderecoRecebido);
    bool endereco_convertido_valido = endereco_convertido != nullptr;
    if (!endereco_convertido_valido)
    {
        return false;
    }

    ip4_addr_copy(endereco_saida, *endereco_convertido);
    bool consulta_ok = contexto.codigoResultado == ERR_OK;
    return consulta_ok;
}

bool UtilitariosRede::converterTextoParaIp4(const char *texto_endereco, ip4_addr_t &endereco_saida)
{
    ip4_addr_set_zero(&endereco_saida);

    bool texto_invalido = texto_endereco == nullptr;
    if (texto_invalido)
    {
        return false;
    }

    int resultado_conversao = ip4addr_aton(texto_endereco, &endereco_saida);
    bool conversao_ok = resultado_conversao != 0;
    return conversao_ok;
}

bool UtilitariosRede::converterIp4ParaTexto(const ip4_addr_t &endereco_origem, char *destino_texto,
                                            size_t tamanho_texto)
{
    bool destino_invalido = destino_texto == nullptr;
    if (destino_invalido)
    {
        return false;
    }

    bool tamanho_insuficiente = tamanho_texto < IP4ADDR_STRLEN_MAX;
    if (tamanho_insuficiente)
    {
        return false;
    }

    ip4addr_ntoa_r(&endereco_origem, destino_texto, static_cast<int>(tamanho_texto));
    return true;
}

void UtilitariosRede::tratarRespostaDns(const char *nome_host, const ip_addr_t *endereco_resolvido,
                                        void *contexto_usuario)
{
    (void)nome_host;

    ContextoResolucaoDns *contexto = static_cast<ContextoResolucaoDns *>(contexto_usuario);
    bool contexto_invalido = contexto == nullptr;
    if (contexto_invalido)
    {
        return;
    }

    bool endereco_valido = endereco_resolvido != nullptr;
    if (!endereco_valido)
    {
        ip_addr_set_zero_ip4(&contexto->enderecoRecebido);
        contexto->codigoResultado = ERR_ARG;
        contexto->resolvido = true;
        return;
    }

    ip_addr_copy(contexto->enderecoRecebido, *endereco_resolvido);
    contexto->codigoResultado = ERR_OK;
    contexto->resolvido = true;
}
