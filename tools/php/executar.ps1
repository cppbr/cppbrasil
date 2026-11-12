param([switch]$AssinarValidar)

$ErrorActionPreference = 'Stop'
$php = (Get-Command php.exe -CommandType Application).Source
$pathAnterior = $env:PATH
Push-Location -LiteralPath $PSScriptRoot
try {
    $bibliotecas = Split-Path -Parent $PSScriptRoot
    if ($env:CROSSNFE_LIBRARY) {
        $bibliotecas = Split-Path -Parent (Resolve-Path -LiteralPath $env:CROSSNFE_LIBRARY).Path
    }
    $env:PATH = "$bibliotecas;$pathAnterior"

    $opcoesPhp = @('-d', 'ffi.enable=true')
    if ((& $php -m) -notcontains 'FFI') {
        $ffi = Join-Path (Split-Path -Parent $php) 'ext/php_ffi.dll'
        if (!(Test-Path -LiteralPath $ffi -PathType Leaf)) {
            throw 'Habilite a extensão FFI no PHP CLI ou use uma distribuição PHP x64 que inclua php_ffi.dll.'
        }
        $opcoesPhp += @('-d', "extension=$ffi")
    }
    $argumentos = @('emissao.php', 'config.properties')
    if ($AssinarValidar) { $argumentos += '--assinar-validar' }
    & $php @opcoesPhp @argumentos
    exit $LASTEXITCODE
} finally {
    $env:PATH = $pathAnterior
    Pop-Location
}
