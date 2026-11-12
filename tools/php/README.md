# Exemplo de NF-e em PHP

Use o PHP CLI 8.1+ de 64 bits com a extensão FFI. O exemplo usa a ABI 1 da
CrossNFe e carrega os binários da pasta pai. `CrossNFe.php` contém o acesso
nativo e `emissao.php` contém o preenchimento e o fluxo de emissão.

## Windows x64

Disponibilize `php.exe` no `PATH` e siga a [lista de DLLs compatíveis com MSVC](../README.md#windows-x64).
O script acrescenta ao `PATH` a pasta da `crossnfe.dll` para a execução.
Se a extensão FFI ainda estiver desativada, ele tenta carregar a
`php_ffi.dll` que acompanha a instalação do PHP.

```powershell
.\executar.ps1
.\executar.ps1 -AssinarValidar
```

## Linux x64

Habilite a extensão FFI no PHP CLI e siga a [lista de bibliotecas nativas](../README.md#linux-x64).
Se o Qt 6.12+ estiver fora das pastas do sistema, informe sua pasta `lib`
em `CROSSNFE_QT_LIB`.

```bash
bash executar.sh
bash executar.sh --assinar-validar
```

## Configuração e saída

Edite `config.properties` para alterar o emitente, a série, o número, a
data de emissão com fuso horário e a pasta de saída. A configuração de
exemplo reproduz a NF-e 55 do C# em homologação, com dois itens, ICMS,
IBS/CBS e total de R$ 125,00. Os XMLs ficam em `build/emissao` por padrão.

A opção de assinatura usa um certificado de teste. Defina
`CROSSNFE_PFX`, `CROSSNFE_PFX_PASSWORD` e `CROSSNFE_SCHEMAS` antes de
executá-la. A pasta de schemas deve conter `nfe_v4.00.xsd` e as
dependências dele. O exemplo assina o XML, valida-o pelo XSD e confere a
releitura em uma sessão separada.
Todas as operações do exemplo são locais.

Para rodar com outro arquivo de configuração, use
`php -d ffi.enable=true emissao.php outro.properties` após habilitar a
extensão e configurar o caminho das bibliotecas. Para selecionar outro
binário da CrossNFe, informe seu caminho absoluto em `CROSSNFE_LIBRARY`.

## Uso da interface

```php
require_once __DIR__ . '/CrossNFe.php';

$session = new CrossNFe\Session();
try {
    $root = $session->root();
    $nota = $root->child('notafiscal')->child('NFe')->add();
    $produto = $nota->child('infNFe')->child('det')->add()->child('prod');
    $produto->set('cProd', '001');
} finally {
    $session->close();
}
```

`child()` navega para um objeto ou coleção. Use `add()`, `count()` e
`at()` nas coleções, com índices a partir de zero. `get()` devolve o
valor como texto da ABI, inclusive para campos numéricos. Use os objetos
na mesma thread e enquanto a sessão estiver aberta.

[Configuração da extensão FFI no PHP](https://www.php.net/manual/pt_BR/ffi.configuration.php).
