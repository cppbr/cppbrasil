#!/usr/bin/env bash
set -euo pipefail
pasta="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$pasta"
if [[ -n "${CROSSNFE_QT_LIB:-}" ]]; then
    export LD_LIBRARY_PATH="$CROSSNFE_QT_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi
exec "${PHP:-php}" -d ffi.enable=true emissao.php config.properties "$@"
