#ifndef WIFI_LWIPOPTS_FRAGMENTO_HTTP_SERVIDOR_H
#define WIFI_LWIPOPTS_FRAGMENTO_HTTP_SERVIDOR_H

/*=============================================================================
 * Fragmento: Servidor HTTP
 * Habilita os recursos do HTTPD quando necessário.
 *=============================================================================*/

/* LWIP_HTTPD: Ativa o servidor HTTP embutido. */
/*
 * Remove definições anteriores para evitar conflitos quando múltiplos
 * fragmentos de configuração ajustam as opções do HTTPD.
 */
#undef LWIP_HTTPD

/*
 * Habilita o servidor HTTP embutido, permitindo publicar páginas e serviços
 * REST diretamente a partir do firmware.
 */
#define LWIP_HTTPD                1

/* LWIP_HTTPD_SSI: Suporte a Server-Side Includes. */
/*
 * Anula qualquer valor pré-definido e garante que a funcionalidade SSI seja
 * recompilada com suporte completo.
 */
#undef LWIP_HTTPD_SSI

/*
 * Libera o uso de Server-Side Includes para atualizar trechos de HTML com
 * informações dinâmicas geradas pelo microcontrolador.
 */
#define LWIP_HTTPD_SSI            1

/* LWIP_HTTPD_SUPPORT_POST: Suporte a POST no servidor HTTP. */
/*
 * Desfaz valores anteriores para impedir que configurações conflitantes
 * deixem o tratamento de POST desabilitado inadvertidamente.
 */
#undef LWIP_HTTPD_SUPPORT_POST

/*
 * Garante que o HTTPD aceite requisições POST, viabilizando upload de dados
 * e formulários enviados pelo cliente.
 */
#define LWIP_HTTPD_SUPPORT_POST   1

/* LWIP_HTTPD_DYNAMIC_HEADERS: Geração dinâmica de cabeçalhos. */
/*
 * Limpa definições anteriores para que a geração dinâmica de cabeçalhos seja
 * ativada sempre que este fragmento for incluído.
 */
#undef LWIP_HTTPD_DYNAMIC_HEADERS

/*
 * Autoriza a montagem de cabeçalhos HTTP em tempo de execução, permitindo
 * personalizar campos como Content-Type e caching por resposta.
 */
#define LWIP_HTTPD_DYNAMIC_HEADERS 1

/* HTTPD_USE_CUSTOM_FSDATA: Seleção de fsdata customizado. */
/*
 * Garante que não exista definição anterior forçando fsdata customizado quando
 * o objetivo é utilizar o armazenamento embutido padrão.
 */
#undef HTTPD_USE_CUSTOM_FSDATA

/*
 * Mantém o servidor HTTP utilizando o conjunto de arquivos embutidos padrão,
 * simplificando a distribuição quando o fsdata customizado não é necessário.
 */
#define HTTPD_USE_CUSTOM_FSDATA   0

/* LWIP_HTTPD_CGI: Suporte a CGI (mantido desabilitado por padrão). */
/*
 * Revoga possíveis ativações anteriores do CGI para garantir um comportamento
 * coerente com o escopo deste projeto.
 */
#undef LWIP_HTTPD_CGI

/*
 * Mantém o suporte a CGI desabilitado, evitando sobrecarga desnecessária no
 * firmware quando scripts externos não serão utilizados.
 */
#define LWIP_HTTPD_CGI            0

#endif
