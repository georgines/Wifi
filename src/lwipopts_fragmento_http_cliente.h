#ifndef WIFI_LWIPOPTS_FRAGMENTO_HTTP_CLIENTE_H
#define WIFI_LWIPOPTS_FRAGMENTO_HTTP_CLIENTE_H

/*=============================================================================
 * Fragmento: Cliente HTTP
 * Aplica configurações adicionais quando o cliente HTTP estiver habilitado.
 *=============================================================================*/

/* HTTP_CLIENT_SUPPORT: Habilita (1) o cliente HTTP. */
/*
 * Garante redefinição explícita da macro para evitar heranças indesejadas
 * quando outro trecho do projeto já tiver declarado suporte ao cliente HTTP.
 */
#undef HTTP_CLIENT_SUPPORT

/*
 * Ativa o cliente HTTP da lwIP para permitir que o firmware realize requisições
 * como GET e POST de forma assíncrona sobre TCP.
 */
#define HTTP_CLIENT_SUPPORT       1

/* LWIP_HTTPC_HAVE_RESULT: Suporte a retorno httpc_result_fn. */
/*
 * Força a recompilação da macro para que o callback de resultado seja sempre
 * disponibilizado, independentemente de outras configurações herdadas.
 */
#undef LWIP_HTTPC_HAVE_RESULT

/*
 * Solicita à lwIP que emita notificações de conclusão das transações HTTP,
 * permitindo tratar sucessos, falhas ou timeouts no cliente.
 */
#define LWIP_HTTPC_HAVE_RESULT    1

/* LWIP_HTTPC_HAVE_STATE: Armazena estado interno do cliente HTTP. */
/*
 * Remove qualquer configuração prévia para garantir que o estado interno do
 * cliente HTTP seja realmente armazenado quando este fragmento for incluído.
 */
#undef LWIP_HTTPC_HAVE_STATE

/*
 * Armazena contexto adicional por conexão HTTP, permitindo retomar operações
 * com segurança e manipular dados persistentes por requisição.
 */
#define LWIP_HTTPC_HAVE_STATE     1

#endif
