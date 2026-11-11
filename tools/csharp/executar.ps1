param([switch]$AssinarValidar)

$ErrorActionPreference = 'Stop'
Push-Location -LiteralPath $PSScriptRoot
try {
    $argumentos = @('config.properties')
    if ($AssinarValidar) { $argumentos += '--assinar-validar' }
    dotnet run --project Emissao.csproj -- @argumentos
    exit $LASTEXITCODE
} finally {
    Pop-Location
}
