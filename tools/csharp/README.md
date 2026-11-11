# Exemplo de NF-e em C#

Abra `Emissao.csproj` com o SDK .NET 10 x64 ou execute no PowerShell dentro desta pasta.

```powershell
.\executar.ps1
```

O projeto referencia `CrossNFe.Managed.dll` localmente e copia as DLLs para a saída da compilação. O código do exemplo está em `Program.cs`.

Também é possível executar diretamente com `dotnet run --project Emissao.csproj -- config.properties`.

Antes de executar, coloque as DLLs abaixo ao lado de `crossnfe.dll` ou em um diretório no `PATH`. Use versões Windows x64 compatíveis com a compilação da `crossnfe.dll`.

| Componente | DLLs |
| --- | --- |
| CppBrasil, build MSVC | `CppBase.dll`, `CppNFe.dll` |
| Qt 6 | `Qt6Core.dll`, `Qt6Network.dll` |
| libxml2 | `libxml2.dll` |
| OpenSSL 3 | `libcrypto-3-x64.dll`, `libssl-3-x64.dll` |
| Microsoft Visual C++ Redistributable x64 | `MSVCP140.dll`, `MSVCP140_1.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll` |

As DLLs do CppBrasil estão na pasta pai deste exemplo. A compilação usa OpenSSL 3.5.9, Qt do kit `msvc2022_64` e libxml2 para MSVC.

O exemplo preenche uma NF-e modelo 55 com dados fictícios, dois produtos e total de R$ 125,00. Ele gera o XML e confere os dados por releitura. O fluxo local termina antes da transmissão à SEFAZ.

Edite `config.properties` para ajustar os dados da nota e a saída. Por padrão, os XMLs ficam em `build/emissao` dentro da pasta do exemplo.

Para assinar e validar pelo XSD, configure `CROSSNFE_PFX`, `CROSSNFE_PFX_PASSWORD` e `CROSSNFE_SCHEMAS` no ambiente e execute `.\executar.ps1 -AssinarValidar`. A pasta de schemas precisa conter `nfe_v4.00.xsd` e os demais arquivos do pacote. Use um certificado de teste.
