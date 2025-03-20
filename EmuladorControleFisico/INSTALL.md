
# Guia de Instalação do Emulador de Controle Virtual

## Pré-requisitos

Antes de instalar e configurar o Emulador de Controle Virtual, certifique-se de ter:

1. Windows 7, 8, 10 ou 11 (64 bits recomendado)
2. Visual C++ Redistributable 2019 ou mais recente
3. Privilégios de administrador no sistema

## Instalação dos Drivers

### 1. Driver Interception

O Interception é responsável por capturar e manipular eventos de teclado e mouse.

1. Execute o script `install-interception.bat` como administrador
2. Aguarde a conclusão da instalação
3. Reinicie o computador

**Observação:** Se ocorrer algum erro, verifique se você está executando como administrador.

### 2. Driver ViGEm

O ViGEm (Virtual Gamepad Emulation Framework) é responsável por criar e gerenciar o controle virtual.

1. Execute o instalador `ViGEmBus_Setup.msi` como administrador
2. Siga as instruções na tela
3. Reinicie o computador após a instalação

## Compilação do Projeto

Se você deseja compilar o projeto a partir do código-fonte:

1. Abra o arquivo `EmuladorControleFisico.sln` no Visual Studio 2019 ou superior
2. Configure o projeto para compilação x64
3. Verifique se as dependências estão corretamente configuradas:
   - Diretórios de inclusão para Interception e ViGEm
   - Diretórios de biblioteca para Interception e ViGEm
4. Compile o projeto (F7 ou Ctrl+Shift+B)

## Execução

1. Execute o aplicativo `EmuladorControleFisico.exe` como administrador
2. Configure os mapeamentos conforme necessário na interface
3. Ative a emulação pressionando F8 ou usando o botão na interface

## Resolução de Problemas

### Driver Interception não instala

- Verifique se você está executando como administrador
- Desative temporariamente seu antivírus
- Verifique se o Secure Boot está desativado (para Windows 8/10/11)

### Driver ViGEm não instala

- Verifique se você tem permissões de administrador
- Tente instalar manualmente os drivers em modo de segurança
- Verifique se há conflitos com outros drivers de dispositivo

### Aplicativo não encontra os drivers

- Verifique se ambos os drivers foram instalados corretamente
- Reinicie o computador após a instalação dos drivers
- Execute o aplicativo como administrador

### Controle virtual não funciona em todos os jogos

- Verifique se o jogo suporta controles XInput (Xbox 360)
- Alguns jogos podem requerer configuração adicional
- Tente reiniciar o jogo após ativar a emulação

## Contato e Suporte

Para obter ajuda adicional ou reportar problemas:

- Envie um e-mail para [seu-email@exemplo.com]
- Visite o repositório do projeto: [URL do Repositório]
- Consulte a documentação completa em: [URL da Documentação]
