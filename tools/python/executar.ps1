param([switch]$AssinarValidar)

$ErrorActionPreference = 'Stop'
Push-Location -LiteralPath $PSScriptRoot
try {
    $argumentos = @('emissao.py', 'config.properties')
    if ($AssinarValidar) { $argumentos += '--assinar-validar' }
    python @argumentos
    exit $LASTEXITCODE
} finally {
    Pop-Location
}
