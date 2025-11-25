#ifndef WIFI_LWIPOPTS_H
#define WIFI_LWIPOPTS_H

/*=============================================================================
 * Configurações do lwIP (lwipopts.h)
 * Configuração padrão para integração com FreeRTOS (NO_SYS=0)
 * Cada opção documenta seus valores possíveis e efeito.
 *=============================================================================*/

/*=== Opções de Sistema (Single-thread, sem sistema operacional) ===*/

/**
 * NO_SYS: Usar (1) ou não usar (0) abstração de sistema operacional.
 *   1: lwIP roda em modo single-thread, sem dependência de OS.
 *   0: Habilita sys_arch e camada TCPIP thread.
 */

/*
 * Verifica se o projeto declarou o uso do FreeRTOS com lwIP para ajustar
 * automaticamente as opções dependentes de sistema operacional.
 */
#ifdef HABILITAR_FREERTOS_LWIP
/* Configura o lwIP para operar com suporte a sistema operacional (FreeRTOS). */
#define NO_SYS                    0

/* Expõe a API de sockets padrão, necessária para integrações portáveis. */
#define LWIP_SOCKET               1

/* Habilita a API Netconn, mais amigável para uso com threads. */
#define LWIP_NETCONN              1

/* Ativa seções críticas leves para proteger estruturas compartilhadas. */
#define SYS_LIGHTWEIGHT_PROT      1

/* Permite que interrupções alimentem o core TCP/IP de forma segura. */
#define LWIP_TCPIP_CORE_LOCKING_INPUT 1

/* Usa struct timeval da libc para manter compatibilidade com FreeRTOS. */
#define LWIP_TIMEVAL_PRIVATE      0

/* Dimensiona a pilha da thread TCP/IP para acomodar as tarefas extras. */
#define TCPIP_THREAD_STACKSIZE    2048

/* Define prioridade intermediária para o core TCP/IP dentro do FreeRTOS. */
#define TCPIP_THREAD_PRIO         3

/* Ajusta o tamanho padrão das filas de mensagens TCP/IP. */
#define TCPIP_MBOX_SIZE           16

/* Estabelece pilha mínima das tarefas criadas pela lwIP. */
#define DEFAULT_THREAD_STACKSIZE  1024

/* Define capacidade padrão das caixas de correio para comunicações genéricas. */
#define DEFAULT_MBOX_SIZE         16

/* Dimensiona a fila de recepção padrão para sockets TCP. */
#define DEFAULT_TCP_RECVMBOX_SIZE 16

/* Ajusta a fila de recepção padrão para sockets UDP. */
#define DEFAULT_UDP_RECVMBOX_SIZE 8

/* Configura a fila de recepção padrão para a API RAW. */
#define DEFAULT_RAW_RECVMBOX_SIZE 8
/*
 * Cenário sem FreeRTOS: simplifica a pilha para modo bare-metal, reduzindo o
 * consumo de recursos e mantendo apenas o essencial para execução single-thread.
 */
#else
/* Obriga modo sem sistema operacional quando o FreeRTOS não estiver ativo. */
#define NO_SYS                    1

/* Desativa sockets para reduzir footprint quando rodar sem RTOS. */
#define LWIP_SOCKET               0

/* Elimina a API Netconn para simplificar a build bare-metal. */
#define LWIP_NETCONN              0

/* Dispensa proteções leves porque não haverá concorrência. */
#define SYS_LIGHTWEIGHT_PROT      0

/* Impede travamento do core TCP/IP, compatível com execução single-thread. */
#define LWIP_TCPIP_CORE_LOCKING_INPUT 0

/* Usa implementação própria de timeval ao operar sem libc completa. */
#define LWIP_TIMEVAL_PRIVATE      1
#endif

/*=== Protocolos de Transporte ===*/

/**
 * LWIP_TCP: Habilita (1) ou desabilita (0) suporte a TCP.
 *   1: Inclui camada TCP (three-way handshake, retransmissão).
 *   0: Exclui código TCP.
 */
/* Garante suporte ao protocolo TCP para conexões confiáveis. */
#define LWIP_TCP                  1

/**
 * LWIP_UDP: Habilita (1) ou desabilita (0) suporte a UDP.
 *   1: Inclui camada UDP (sendto/recvfrom raw UDP).
 *   0: Exclui código UDP.
 */
/* Mantém a pilha UDP ativa para datagramas leves. */
#define LWIP_UDP                  1

/**
 * LWIP_RAW: Habilita (1) ou desabilita (0) API Raw (IP cru).
 *   1: Permite enviar/receber pacotes IP brutos.
 *   0: Desabilita Raw API.
 */
/* Disponibiliza a API RAW para controle direto de pacotes IP. */
#define LWIP_RAW                  1

/*=== Gerenciamento de Memória ===*/

/**
 * MEM_ALIGNMENT: Alinhamento de memória (bytes) para CPU.
 *   Tipicamente: 1, 2, 4 ou 8. Aqui: 4 (ARM, MIPS).
 */
/* Ajusta alinhamento de memória para 32 bits, compatível com ARM. */
#define MEM_ALIGNMENT             4

/**
 * MEM_SIZE: Tamanho (bytes) do heap de memória lwIP.
 *   Aloca espaço para pbufs, netbufs, PCB, etc.
 */
/* Reserva 4 kB de heap exclusivo ao lwIP para buffers internos. */
#define MEM_SIZE                  4096

/**
 * MEMP_NUM_PBUF: Número de estruturas pbuf.
 *   Define quantos pbufs podem existir simultaneamente.
 */
/* Define quantidade de pbufs simultâneos disponíveis para a pilha. */
#define MEMP_NUM_PBUF             16

/**
 * PBUF_POOL_SIZE: Quantidade de pbufs no pool padrão.
 *   Reservado para uso interno do TCP/IP core.
 */
/* Controla a reserva de pbufs no pool padrão do core TCP/IP. */
#define PBUF_POOL_SIZE            16

/**
 * MEMP_NUM_UDP_PCB: Número máximo de PCBs UDP.
 *   Cada socket/PCB UDP consome uma unidade.
 */
/* Limita o número de control blocks UDP criados em paralelo. */
#define MEMP_NUM_UDP_PCB          4

/**
 * MEMP_NUM_TCP_PCB: Número de PCBs TCP simultâneos.
 *   Limita quantas conexões TCP podem ser abertas.
 */
/* Determina quantas conexões TCP podem coexistir. */
#define MEMP_NUM_TCP_PCB          4

/**
 * MEMP_NUM_TCP_SEG: Número de segmentos TCP em buffer.
 *   Usado para enfileirar segmentos aguardando envio.
 */
/* Estabelece o total de segmentos TCP aguardando envio. */
#define MEMP_NUM_TCP_SEG          16

/**
 * MEMP_NUM_SYS_TIMEOUT: Número de timers de sistema.
 *   Deve ser >= LWIP_NUM_SYS_TIMEOUT_INTERNAL + timers extras (ex: MQTT).
 */
/* Reserva espaço para timers internos e um extra para aplicações. */
#define MEMP_NUM_SYS_TIMEOUT      (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 1)

/*=== Configurações de Rede e IPv4 ===*/

/**
 * LWIP_IPV4: Habilita (1) ou desabilita (0) suporte a IPv4.
 */
/* Mantém suporte a IPv4 nativo para compatibilidade ampla. */
#define LWIP_IPV4                 1

/**
 * LWIP_ICMP: Habilita (1) ou desabilita (0) suporte a ICMP (ping).
 */
/* Ativa ICMP para permitir echo requests (ping) e diagnósticos. */
#define LWIP_ICMP                 1

/**
 * LWIP_DHCP: Habilita (1) ou desabilita (0) cliente DHCP.
 *   1: Usa DHCP para configurar endereço IPv4 automaticamente.
 */
/* Habilita cliente DHCP para obter endereço IP automaticamente. */
#define LWIP_DHCP                 1

/**
 * LWIP_AUTOIP: Habilita (1) ou desabilita (0) link-local (RFC3927).
 */
/* Ativa link-local (AutoIP) como fallback em redes sem DHCP. */
#define LWIP_AUTOIP               1

/**
 * LWIP_DNS: Habilita (1) ou desabilita (0) cliente DNS.
 *   Necessário para resolver nomes de host em requests.
 */
/* Disponibiliza resolução de nomes via DNS. */
#define LWIP_DNS                  1

/**
 * LWIP_NETIF_HOSTNAME: Habilita (1) ou desabilita (0) campo hostname.
 *   Usado pelo cliente DHCP e SNMP.
 */
/* Preenche o hostname do dispositivo nas requisições DHCP/SNMP. */
#define LWIP_NETIF_HOSTNAME       1

/*=== Servidor HTTP (HTTPD) ===*/

/**
 * LWIP_HTTPD: Habilita (1) ou desabilita (0) servidor HTTP.
 */
/* Mantém o servidor HTTP embutido inativo por padrão. */
#define LWIP_HTTPD                0

/**
 * LWIP_HTTPD_SSI: Suporte a Server-Side Includes.
 *   1: Permite incluir códigos SSI em páginas HTML.
 */
/* Desativa SSI até que um fragmento específico o reabilite. */
#define LWIP_HTTPD_SSI            0

/**
 * LWIP_HTTPD_SUPPORT_POST: Suporte a requisições HTTP POST.
 */
/* Desliga suporte a POST para economizar recursos quando não usado. */
#define LWIP_HTTPD_SUPPORT_POST   0

/**
 * LWIP_HTTPD_DYNAMIC_HEADERS: Geração dinâmica de cabeçalhos HTTP.
 */
/* Desativa cabeçalhos dinâmicos para simplificar o build base. */
#define LWIP_HTTPD_DYNAMIC_HEADERS 0

/**
 * HTTPD_USE_CUSTOM_FSDATA: Usar sistema de arquivos customizado.
 *   0: Usa fsdata embutido padrão.
 *   1: Upload extern fsdata.c.
 */
/* Mantém o fsdata padrão até que o projeto exija um customizado. */
#define HTTPD_USE_CUSTOM_FSDATA   0

/**
 * LWIP_HTTPD_CGI: Suporte a Common Gateway Interface (CGI).
 */
/* Deixa CGI desativado para reduzir complexidade inicial. */
#define LWIP_HTTPD_CGI            0

/*=== Cliente HTTP ===*/

/**
 * HTTP_CLIENT_SUPPORT: Habilita (1) ou desabilita (0) cliente HTTP.
 */
/* Desliga o cliente HTTP até que um fragmento o habilite. */
#define HTTP_CLIENT_SUPPORT       0

/**
 * LWIP_HTTPC_HAVE_RESULT: Suporta retorno de evento httpc_result_fn.
 */
/* Oculta a callback de resultado do cliente HTTP por padrão. */
#define LWIP_HTTPC_HAVE_RESULT    0

/**
 * LWIP_HTTPC_HAVE_STATE: Armazena estado interno do cliente HTTP.
 */
/* Remove armazenamento de estado extra para economizar memória. */
#define LWIP_HTTPC_HAVE_STATE     0

/*=== Cliente MQTT (contrib/apps/mqtt) ===*/

/**
 * LWIP_MQTT: Habilita (1) ou desabilita (0) cliente MQTT.
 */
/* Mantém o cliente MQTT inativo até ser requisitado. */
#define LWIP_MQTT                         0

/**
 * MQTT_OUTPUT_RINGBUF_SIZE: Tamanho do buffer de saída (bytes).
 *   Ajuste conforme tamanho máximo de payload que deseja armazenar.
 */
/* Reserva buffer de 256 bytes para dados de saída MQTT quando ativo. */
#define MQTT_OUTPUT_RINGBUF_SIZE         256

/**
 * MQTT_VAR_HEADER_BUFFER_LEN: Tamanho do buffer para cabecalhos variáveis.
 */
/* Define 128 bytes para montar cabeçalhos variáveis MQTT. */
#define MQTT_VAR_HEADER_BUFFER_LEN       128

/**
 * MQTT_REQ_MAX_IN_FLIGHT: Número máximo de requisições PUB/SUB pendentes.
 */
/* Limita a quatro o número de requisições MQTT pendentes. */
#define MQTT_REQ_MAX_IN_FLIGHT           4

/**
 * MQTT_CYCLIC_TIMER_INTERVAL: Intervalo (s) para timer periódico MQTT.
 */
/* Define 5 s como intervalo do timer cíclico MQTT. */
#define MQTT_CYCLIC_TIMER_INTERVAL       5

/**
 * MQTT_REQ_TIMEOUT: Timeout (s) para operações PUB/SUB/UNSUB.
 */
/* Configura 30 s como timeout das operações MQTT. */
#define MQTT_REQ_TIMEOUT                 30

/**
 * MQTT_CONNECT_TIMOUT: Timeout (s) para esperar CONNACK após CONNECT.
 */
/* Estabelece 100 s de espera pelo CONNACK após o CONNECT. */
#define MQTT_CONNECT_TIMOUT              100

#ifdef WIFI_FRAGMENTO_HTTP_CLIENTE
#include "lwipopts_fragmento_http_cliente.h"
#endif

#ifdef WIFI_FRAGMENTO_HTTP_SERVIDOR
#include "lwipopts_fragmento_http_servidor.h"
#endif

#ifdef WIFI_FRAGMENTO_MQTT
#include "lwipopts_fragmento_mqtt.h"
#endif

#endif