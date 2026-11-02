param([ValidateSet('serve','migrate','seed')][string]$Action = 'serve')
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
Push-Location $root
try {
 if (!(Test-Path '.env')) { throw 'Copy .env.example to .env first.' }
 $allowed = @('APP_ENV','ENABLE_DEV_AUTH','PORT','DB_PATH')
 foreach ($line in Get-Content '.env') {
  if ($line.Trim() -eq '' -or $line.Trim().StartsWith('#')) { continue }
  if ($line -notmatch '^([A-Z_]+)=(.*)$' -or $allowed -notcontains $Matches[1]) { throw "Invalid .env entry: $line" }
  [Environment]::SetEnvironmentVariable($Matches[1],$Matches[2],'Process')
 }
 $dbParent = Split-Path $env:DB_PATH -Parent
 if ($dbParent) { New-Item -ItemType Directory -Force $dbParent | Out-Null }
 if (!(Test-Path 'build/campus-server.exe')) { throw 'Run scripts/build.ps1 first.' }
 $arguments = @()
 if ($Action -eq 'migrate') { $arguments = @('--migrate') }
 if ($Action -eq 'seed') { $arguments = @('--seed-dev') }
 & ./build/campus-server.exe @arguments
 if ($LASTEXITCODE -ne 0) { throw "Server exited with code $LASTEXITCODE" }
} finally { Pop-Location }
