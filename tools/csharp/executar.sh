#!/usr/bin/env bash
# Executa o exemplo C# no Linux. Uso: ./executar.sh [--assinar-validar]
set -euo pipefail
pasta="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$pasta"
# Qt 6.12 ou superior: informe a pasta lib do Qt se ele não estiver no sistema.
if [[ -n "${CROSSNFE_QT_LIB:-}" ]]; then
    export LD_LIBRARY_PATH="$CROSSNFE_QT_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi
exec dotnet run --project Emissao.csproj -- config.properties "$@"
