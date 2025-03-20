
# Emulador de Controle Virtual

## Visão Geral
Este projeto implementa um emulador de controle virtual para Windows que mapeia eventos de teclado e mouse para um controle Xbox 360 virtual.

## Pré-requisitos
* Windows 7/8/10/11 (64-bit)
* Visual Studio 2019 ou superior com suporte a C++
* Windows Driver Kit (WDK) 7.1.0 ou superior
* Biblioteca Interception (incluída)
* ViGEm (Virtual Gamepad Emulation Framework)

## Instalação
1. Instale o driver Interception:
   - Execute `install-interception.exe` como administrador
2. Instale o driver ViGEm:
   - Execute o instalador `ViGEmBus_Setup.msi` como administrador
3. Compile o projeto no Visual Studio

## Uso
- Execute o aplicativo como administrador
- Pressione F8 para ativar/desativar a emulação
- Configure os mapeamentos na interface ou através do arquivo config.json

## Licença
Este projeto é distribuído sob licença LGPL para uso não comercial.
