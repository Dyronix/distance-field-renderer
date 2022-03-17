# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$cwd = Get-Location
$external_dir = "$cwd\external"

Write-Host "Executing windows\setup.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

Write-Host "Running script for Windows desktop"

Import-Module "$cwd\scripts\windows\utilities\download_repo.ps1"

if (!(Test-Path "$external_dir"))
{	
    # Check that we have a 'external' folder
	Write-Host "External directory was not found, creating ..."
	New-Item -ItemType Directory -Path "$external_dir"
}

Write-Host "External directory: $external_dir"

download_repo $external_dir "sdl-windows" "SDL2-2.0.20" "https://www.libsdl.org/release/SDL2-devel-2.0.20-VC.zip"
download_repo $external_dir "spdlog-windows" "spdlog-1.x" "https://github.com/gabime/spdlog/archive/refs/heads/v1.x.zip"
download_repo $external_dir "gsl-windows" "GSL-main" "https://github.com/microsoft/GSL/archive/refs/heads/main.zip"
download_repo $external_dir "crc-windows" "CRCpp-master" "https://github.com/d-bahr/CRCpp/archive/refs/heads/master.zip"
download_repo $external_dir "glm-windows" "glm-master" "https://github.com/g-truc/glm/archive/refs/heads/master.zip"
download_repo $external_dir "entt-windows" "entt-master" "https://github.com/skypjack/entt/archive/refs/heads/master.zip"
download_repo $external_dir "stb-windows" "stb-master" "https://github.com/nothings/stb/archive/refs/heads/master.zip"
download_repo $external_dir "assimp-windows" "rex-assimp-master" "https://github.com/Dyronix/rex-assimp/archive/refs/heads/master.zip"

try
{
    Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope CurrentUser
}
catch
{
    Write-Host "Could not set execution policy for CurrentUser to \"Unrestricted\"." -ForegroundColor Red
    Write-Host $_
}

Write-Host "ExecutionPolicy for CurrentUser switched to Unrestriced"

try
{
    Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope LocalMachine
}
catch
{
    Write-Host "Could not set execution policy for LocalMachine to \"Unrestricted\"." -ForegroundColor Red
    Write-Host $_
}

Write-Host "ExecutionPolicy for LocalMachine switched to Unrestriced"

Write-Host "Listing execution policies ..."
Get-ExecutionPolicy -List

Write-Host "Execution policies have been changed." -ForegroundColor Yellow
Write-Host "A restart might be required" -ForegroundColor Yellow

Write-Host "Setup script completed" -ForegroundColor Green