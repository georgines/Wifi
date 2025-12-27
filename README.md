target_link_libraries(aplicacao PRIVATE
aplicada internamente pela biblioteca.

# Biblioteca Wifi

Camada de alto nível para inicialização e gerenciamento de Wi-Fi no Raspberry Pi Pico W (CYW43), com API simplificada e status detalhado.

## Principais recursos
- Inicialização do rádio CYW43 e conexão WPA2 AES.
- Consulta de status detalhado da conexão via enum `StatusWifi`.
- Métodos para obter IP, gateway e máscara como texto.
- DNS síncrono e utilitários de conversão IP/texto.
- Compatível com FreeRTOS e baremetal.

## Requisitos
- Raspberry Pi Pico W (RP2040 + CYW43)
- Pico SDK >= 2.2.0
- Projeto CMake com importação do Pico SDK

## Como adicionar ao projeto
1. Coloque a pasta `Wifi/` no mesmo nível do seu `CMakeLists.txt` principal.
2. No `CMakeLists.txt` raiz, inclua a subpasta e vincule a biblioteca:

```cmake
add_subdirectory(Wifi)

target_link_libraries(sua_aplicacao PRIVATE 
    Wifi 
    pico_stdlib
)
```

## Exemplo de uso básico
```cpp
#include "Wifi.h"

constexpr char SSID[] = "MinhaRede";
constexpr char SENHA[] = "SenhaSegura";

int main() {
    stdio_init_all();
    Wifi wifi;
    wifi.iniciar(SSID, SENHA);

    while (true) {
        StatusWifi status = wifi.obterStatusWifi();
        if (status == StatusWifi::Conectado) {
            printf("IP: %s\n", wifi.obterIp());
        } else {
            printf("Status: %d\n", status);
        }
        sleep_ms(1000);
    }
}
```

## API principal
- `Wifi()` — construtor padrão.
- `void iniciar(const char *ssid, const char *senha)` — inicializa rádio e inicia conexão.
- `StatusWifi obterStatusWifi()` — retorna status detalhado da conexão.
- `const char *obterIp()` — IP local como string.
- `const char *obterGatewayPadrao()` — gateway padrão como string.
- `const char *obterMascaraDeRede()` — máscara de sub-rede como string.

### Enum `StatusWifi`
- `ErroDesconhecido`, `FalhaNaAutenticacao`, `RedeNaoEncontrada`, `Falha`, `Desligado`, `Conectando`, `ConectadoSemIp`, `Conectado`

## Utilitários de Rede
Incluídos em `UtilitariosRede.h`:
- `resolverNomeHostParaIp(const char *host, ip4_addr_t &saida, uint32_t timeout_ms)`
- `converterTextoParaIp4(const char *texto, ip4_addr_t &saida)`
- `converterIp4ParaTexto(const ip4_addr_t &origem, char *destino, size_t tam)`

## Boas práticas
- Inicialize o Wi-Fi após configurar o `stdio` para logs.
- Use constantes para SSID/senha.
- Consulte o status periodicamente para reagir a quedas.
- Uma instância da classe por projeto.

**Observação importante:** Se usar FreeRTOS, nunca chame métodos da biblioteca Wifi fora de uma tarefa; sempre utilize dentro de uma função de tarefa para evitar problemas de concorrência e inicialização do sistema.

