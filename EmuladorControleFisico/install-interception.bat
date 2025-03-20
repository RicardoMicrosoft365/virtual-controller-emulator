
@echo off
echo Instalador do driver Interception
echo.
echo IMPORTANTE: Execute este script como administrador
echo.
echo Este script instala o driver Interception necessário para o Emulador de Controle Virtual.
echo.
pause

echo Instalando o driver Interception...
cd lib\interception\command-line
install-interception.exe /install

echo.
echo Instalação concluída.
echo.
echo Se ocorreu algum erro, verifique se você:
echo 1. Executou este script como administrador
echo 2. Possui o Windows Driver Kit (WDK) instalado
echo.
pause
