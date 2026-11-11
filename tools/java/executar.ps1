param([switch]$AssinarValidar)

$ErrorActionPreference = 'Stop'
$pathAnterior = $env:PATH
Push-Location -LiteralPath $PSScriptRoot
try {
    $env:PATH = "$PSScriptRoot;$pathAnterior"
    javac -encoding UTF-8 --release 22 -cp crossnfe.jar -d build Emissao.java
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    $argumentos = @('config.properties')
    if ($AssinarValidar) { $argumentos += '--assinar-validar' }
    java --enable-native-access=ALL-UNNAMED "-Dcrossnfe.library=$PSScriptRoot\crossnfe.dll" -cp "build;crossnfe.jar" Emissao @argumentos
    exit $LASTEXITCODE
} finally {
    $env:PATH = $pathAnterior
    Pop-Location
}
