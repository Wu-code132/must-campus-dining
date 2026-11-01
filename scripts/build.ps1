param([string]$CompilerPath = '')
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
if (!$CompilerPath) { $CompilerPath = Join-Path $root '.tools/llvm-mingw-20261020-ucrt-x86_64/bin/clang.exe' }
if (!(Test-Path $CompilerPath)) { throw 'Run scripts/setup-toolchain.ps1 first, or supply -CompilerPath.' }
$CompilerPath = (Resolve-Path $CompilerPath).Path
Push-Location $root
try {
 New-Item -ItemType Directory -Force build | Out-Null
 $env:TEMP = Join-Path $root 'build/tmp'
 $env:TMP = $env:TEMP
 New-Item -ItemType Directory -Force $env:TEMP | Out-Null
 $sources = @(Get-ChildItem backend/src,backend/modules -Filter '*.c' -Recurse | ForEach-Object FullName)
 $sources += @('vendor/civetweb/src/civetweb.c','vendor/sqlite/sqlite3.c')
 & $CompilerPath -std=c11 -O0 -g -DNO_SSL -DNO_CGI -Ibackend/include -Ivendor/civetweb/include -Ivendor/sqlite @sources -static -lwinpthread -lws2_32 -ladvapi32 -o build/campus-server.exe
 if ($LASTEXITCODE -ne 0) { throw 'C compilation failed' }
 Write-Host 'Built build/campus-server.exe'
} finally { Pop-Location }
