# Biblioteca Wifi

Camada de alto nível para inicializar o rádio CYW43 do Raspberry Pi Pico W, conectar em redes WPA2 e expor informações básicas de rede (IP, gateway e máscara) de forma simples.

## Recursos principais
- Ativa o rádio CYW43 e configura o modo estação com tratamento de erros.
- Conecta em redes WPA2 AES com tentativas múltiplas e mensagens de depuração via `printf`.
- Verifica o estado do link antes de cada operação crítica.
- Expõe IP local, gateway padrão e máscara de sub-rede prontos para uso como `ip4_addr_t` ou texto.
- Oferece reconexão automática controlada por software na aplicação.

## Requisitos
- Raspberry Pi Pico W (RP2040 com CYW43) e SDK 2.2.0.
- Ambiente CMake com o Pico SDK já importado no projeto principal.
- Backend de logs configurado com `stdio_uart_init_full(uart1, 115200, 8, 9)` e `stdio_set_translate_crlf(&stdio_uart, true)`.

## Como adicionar ao projeto
1. Garanta que a pasta `Wifi/` esteja no mesmo nível do seu `CMakeLists.txt` principal.
2. No `CMakeLists.txt` raiz, inclua a subpasta e vincule a biblioteca:

```cmake
add_subdirectory(Wifi)


target_link_libraries(aplicacao PRIVATE
    Wifi
    pico_stdlib
)
```

## Exemplo básico de uso
```cpp
#include <cstdio>
#include "pico/stdlib.h"
#include "Wifi.h"

constexpr char SSID_REDE_PADRAO[] = "MinhaRede";
constexpr char SENHA_REDE_PADRAO[] = "SenhaMuitoSegura";
constexpr uint32_t INTERVALO_VERIFICACAO_MS = 1000U;

int main()
{
    stdio_init_all();

    Wifi wifi(SSID_REDE_PADRAO, SENHA_REDE_PADRAO);

    bool wifi_iniciado = wifi.iniciar();
    if (!wifi_iniciado)
    {
        printf("Nao foi possivel iniciar o Wi-Fi.\n");
        return 1;
    }

    printf("IP local: %s\n", wifi.obterIpLocalComoTexto());

    while (true)
    {
        bool conexao_ok = wifi.garantirConexao();
        if (!conexao_ok)
        {
            printf("Ainda sem conexao.\n");
        }

        sleep_ms(INTERVALO_VERIFICACAO_MS);
    }
}
```

## Guia rápido da API
- `bool iniciar()`: inicializa o rádio e tenta a primeira conexão. Retorna `false` em falhas de hardware ou autenticação.
- `bool garantirConexao()`: verifica o link e tenta reconectar dentro de um tempo limite; útil no `loop` principal.
- `bool estaConectado() const`: confirma se a interface padrão está ativa e com link.
- `ip4_addr_t obterIpLocal() const`: retorna o endereço IPv4 pronto para uso em sockets.
- `const char *obterIpLocalComoTexto() const`: fornece o mesmo IP em formato `"192.168.4.10"` sem alocação dinâmica.
- Métodos equivalentes existem para gateway (`obterGatewayPadrao*`) e máscara (`obterMascaraSubrede*`).

## Pequenos exemplos de uso da API
### Atualizar uma string status no display
```cpp
const char *texto_ip = wifi.obterIpLocalComoTexto();
mostrarIpNaTela(texto_ip);
```

### Executar lógica apenas quando o link cair
```cpp
bool link_ativo = wifi.estaConectado();
if (!link_ativo)
{
    registrarEventoSemRede();
}
```

### Forçar reconexão com retentativa manual
```cpp
bool conectado = wifi.garantirConexao();
if (!conectado)
{
    executarModoOffline();
}
```

## Boas práticas
- Inicialize `Wifi` após configurar `stdio` para visualizar logs de falha.
- Mantenha SSID e senha em `constexpr` ou constantes imutáveis, evitando literais espalhados.
- Chame `garantirConexao()` em intervalos regulares para reagir rapidamente a quedas de link.
aplicada internamente pela biblioteca.
- Caso precise de múltiplas interfaces, crie apenas uma instância da classe: a biblioteca guarda ponteiro para a instância ativa para manter consistência do rádio.

