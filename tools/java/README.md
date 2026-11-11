# Exemplo de NF-e em Java

Funciona no Windows x64 e no Linux x64. As instruções abaixo são do Windows; veja a seção [Linux x64](#linux-x64).

Com um JDK 22 ou superior de 64 bits no PATH, execute no PowerShell dentro desta pasta.

```powershell
.\executar.ps1
```

O script compila `Emissao.java` usando `crossnfe.jar` e inicia a JVM com acesso nativo habilitado. Ele configura o caminho da `crossnfe.dll` e das dependências para essa execução.

Os comandos equivalentes no PowerShell são

```powershell
$pasta = (Get-Location).Path
$env:PATH = "$pasta;$env:PATH"
javac -encoding UTF-8 --release 22 -cp crossnfe.jar -d build Emissao.java
java --enable-native-access=ALL-UNNAMED "-Dcrossnfe.library=$pasta\crossnfe.dll" -cp "build;crossnfe.jar" Emissao config.properties
```

Antes de executar, coloque as DLLs abaixo ao lado de `crossnfe.dll` ou em um diretório no `PATH`. Use versões Windows x64 compatíveis com a compilação da `crossnfe.dll`.

| Componente | DLLs |
| --- | --- |
| CppBrasil, build MSVC | `CppNFe.dll` |
| Qt 6 | `Qt6Core.dll`, `Qt6Network.dll` |
| libxml2 | `libxml2.dll` |
| OpenSSL 3 | `libcrypto-3-x64.dll`, `libssl-3-x64.dll` |
| Microsoft Visual C++ Redistributable x64 | `MSVCP140.dll`, `MSVCP140_1.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll` |

A DLL do CppBrasil está na pasta pai deste exemplo. A compilação usa OpenSSL 3.5.9, Qt do kit `msvc2022_64` e libxml2 para MSVC.

O exemplo preenche uma NF-e modelo 55 com dados fictícios, dois produtos e total de R$ 125,00. Ele gera o XML e confere os dados por releitura. O fluxo local termina antes da transmissão à SEFAZ.

Edite `config.properties` para ajustar os dados da nota e a saída. Por padrão, os XMLs ficam em `build/emissao` dentro da pasta do exemplo.

Para assinar e validar pelo XSD, configure `CROSSNFE_PFX`, `CROSSNFE_PFX_PASSWORD` e `CROSSNFE_SCHEMAS` no ambiente e execute `.\executar.ps1 -AssinarValidar`. A pasta de schemas precisa conter `nfe_v4.00.xsd` e os demais arquivos do pacote. Use um certificado de teste.

## Linux x64

Com um JDK 22 ou superior de 64 bits no PATH, execute dentro desta pasta:

```bash
./executar.sh
./executar.sh --assinar-validar
```

Os comandos equivalentes são

```bash
javac -encoding UTF-8 --release 22 -cp crossnfe.jar -d build Emissao.java
java --enable-native-access=ALL-UNNAMED "-Dcrossnfe.library=$PWD/../libcrossnfe.so" -cp "build:crossnfe.jar" Emissao config.properties
```

A `libcrossnfe.so` encontra a `libCppNFe.so` na mesma pasta. O Qt 6.12+ deve estar no sistema ou na pasta
indicada em `CROSSNFE_QT_LIB`. Veja as dependências e a liberação do SHA1 no Fedora/RHEL no
[README da pasta](../README.md#linux-x64).
