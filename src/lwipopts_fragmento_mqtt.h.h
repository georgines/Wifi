#ifndef WIFI_LWIPOPTS_FRAGMENTO_MQTT_H
#define WIFI_LWIPOPTS_FRAGMENTO_MQTT_H

/*=============================================================================
 * Fragmento: Cliente MQTT
 * Ajusta opções utilizadas pelo cliente MQTT da lwIP contrib.
 *=============================================================================*/

/* LWIP_MQTT: Habilita o cliente MQTT. */
/*
 * Remove possíveis definições anteriores para evitar conflitos quando
 * diferentes módulos controlam o suporte a MQTT.
 */
#undef LWIP_MQTT

/*
 * Ativa o cliente MQTT integrado, permitindo publicar e assinar tópicos em
 * brokers compatíveis de maneira assíncrona.
 */
#define LWIP_MQTT                         1

/* MQTT_OUTPUT_RINGBUF_SIZE: Tamanho do buffer de saída (bytes). */
/*
 * Zera valores herdados para garantir que o tamanho do buffer seja exatamente
 * o necessário para este projeto.
 */
#undef MQTT_OUTPUT_RINGBUF_SIZE

/*
 * Define a capacidade do buffer de saída, controlando quantos bytes podem ser
 * enfileirados antes do envio sem bloquear a aplicação.
 */
#define MQTT_OUTPUT_RINGBUF_SIZE         256

/* MQTT_VAR_HEADER_BUFFER_LEN: Buffer para cabeçalhos variáveis. */
/*
 * Elimina definições anteriores para evitar divergência na alocação do buffer
 * destinado aos cabeçalhos variáveis do protocolo.
 */
#undef MQTT_VAR_HEADER_BUFFER_LEN

/*
 * Ajusta o espaço reservado aos cabeçalhos variáveis, assegurando memória
 * suficiente para montar pacotes MQTT com propriedades extras.
 */
#define MQTT_VAR_HEADER_BUFFER_LEN       128

/* MQTT_REQ_MAX_IN_FLIGHT: Requisições simultâneas. */
/*
 * Garante redefinição explícita para impedir que limites herdados comprometam
 * o número de requisições simultâneas.
 */
#undef MQTT_REQ_MAX_IN_FLIGHT

/*
 * Controla quantos comandos PUB/SUB podem aguardar confirmação ao mesmo tempo,
 * equilibrando desempenho e consumo de memória.
 */
#define MQTT_REQ_MAX_IN_FLIGHT           4

/* MQTT_CYCLIC_TIMER_INTERVAL: Intervalo do timer cíclico (s). */
/*
 * Limpa configurações antigas para garantir que o temporizador cíclico use o
 * intervalo apropriado escolhido para o firmware.
 */
#undef MQTT_CYCLIC_TIMER_INTERVAL

/*
 * Define a frequência, em segundos, com que a pilha MQTT processa tarefas
 * periódicas como keep-alive e retransmissões pendentes.
 */
#define MQTT_CYCLIC_TIMER_INTERVAL       5

/* MQTT_REQ_TIMEOUT: Timeout padrão das operações (s). */
/*
 * Remove qualquer timeout herdado para padronizar o tempo máximo de espera de
 * respostas de requisições MQTT.
 */
#undef MQTT_REQ_TIMEOUT

/*
 * Estabelece o limite de tempo para concluir operações como publicação ou
 * assinatura antes de reportar falha ao aplicativo.
 */
#define MQTT_REQ_TIMEOUT                 30

/* MQTT_CONNECT_TIMOUT: Timeout para aguardar CONNACK (s). */
/*
 * Retira definições prévias para assegurar que a aplicação use o timeout de
 * conexão apropriado ao cenário de rede.
 */
#undef MQTT_CONNECT_TIMOUT

/*
 * Define quanto tempo o cliente aguarda a resposta CONNACK após enviar CONNECT,
 * ajudando a detectar links lentos ou indisponíveis.
 */
#define MQTT_CONNECT_TIMOUT              100

#endif
