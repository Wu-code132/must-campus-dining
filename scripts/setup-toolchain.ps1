$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
$tools = Join-Path $root '.tools'
$zip = Join-Path $tools 'llvm-mingw.zip'
$destination = Join-Path $tools 'llvm-mingw-20261020-ucrt-x86_64'
if (Test-Path (Join-Path $destination 'bin/clang.exe')) { Write-Host 'LLVM-MinGW already installed.'; exit 0 }
New-Item -ItemType Directory -Force $tools | Out-Null
Invoke-WebRequest 'https://github.com/mstorsjo/llvm-mingw/releases/download/20261020/llvm-mingw-20261020-ucrt-x86_64.zip' -OutFile $zip
if ((Get-FileHash $zip -Algorithm SHA256).Hash -ne 'FD5462B7F5D93B2FF981FCD3F9C7E427E47F3203C64EA91C27BCC444EEBD74CD') { throw 'Toolchain checksum mismatch' }
Expand-Archive -LiteralPath $zip -DestinationPath $tools -Force
Write-Host 'Local LLVM-MinGW C compiler installed. No system PATH changes.'
