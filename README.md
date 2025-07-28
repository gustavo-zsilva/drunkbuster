# 🛑 DRUNKBUSTER

**DRUNKBUSTER** é um sistema embarcado projetado para detectar a presença de álcool no condutor e bloquear o sistema até que o nível esteja seguro. Também verifica a temperatura do ambiente, sinaliza falhas e apresenta todas as informações em um display LCD. Ideal para projetos acadêmicos, protótipos automotivos ou sistemas de segurança.

## 📷 Visão Geral

> Um dispositivo inteligente que monitora o nível de álcool e a temperatura ambiente usando sensores, exibe as informações em um display Nokia 5110 e bloqueia o sistema automaticamente quando necessário.

---

## 🧠 Funcionalidades

- 📟 **Interface LCD**: Exibe valores de álcool, temperatura e status do sistema em tempo real.
- 🧪 **Sensor de Álcool (MQ-3)**: Mede a concentração de álcool no ar e converte para valor digital (ADC).
- 🌡️ **Sensor de Temperatura (LM75)**: Lê a temperatura ambiente via I²C.
- 🚨 **Sistema de Bloqueio**: Ativa LEDs e buzzer se o nível de álcool estiver acima do limite.
- 🔄 **Reset Manual**: Permite desbloquear o sistema via botão, desde que o álcool tenha baixado.
- 🧩 **Máquina de Estados**: Estados definidos como `BOOT`, `CLEAR`, `BUSTED` e `ERROR`.
- 🐞 **Debug via USART**: Log detalhado na porta serial para facilitar testes e depuração.

---

## 📦 Componentes Utilizados

| Componente        | Função                          |
|-------------------|----------------------------------|
| ATmega328P        | Microcontrolador principal       |
| MQ-3              | Sensor de detecção de álcool     |
| LM75              | Sensor de temperatura via I²C    |
| Display Nokia 5110| Exibição de dados                |
| LEDs (RGB)        | Indicadores visuais              |
| Buzzer            | Alerta sonoro                    |
| Botão (Push)      | Reset manual do bloqueio         |

---

## ⚙️ Organização do Código

- `main.cpp`: Lógica principal e máquina de estados.
- `handleResetDisplay()`: Limpa e atualiza as informações fixas no LCD.
- `handleUpdateStatusOnDisplay()`: Atualiza status como "CLEAR", "BUSTED", "ERROR", etc.
- `handleValidateAlcoholLevel()`: Verifica se nível de álcool ultrapassou o limite.
- `handlePrintTemperature()`: Lê e formata a temperatura do LM75.
- `setError()`: Gatilho para entrar no estado de erro.

---

## 📈 Máquina de Estados

```text
[BOOT] → [CLEAR] → [BUSTED] → [CLEAR]
               ↘
             [ERROR]
```
---
- Feito por Gustavo Zonta da Silva, como projeto final da disciplina de Microcontroladores do IFSC Câmpus Florianópolis. 🚀
