param (
     [string]$output_dir = "windows",       # where to output the shadow build

     [switch]$clear = $false,               # Should we clear instead of building the soluction
     [switch]$unity = $false                # Should enable unity builds
)

# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$cwd = Get-Location
$external_dir = "$cwd\external"

Write-Host "Executing windows/generate.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

Write-Host "Running script for Windows desktop"

Import-Module "$cwd\scripts\windows\cmake\clear_cmake.ps1"
Import-Module "$cwd\scripts\windows\cmake\generate_cmake.ps1"

if (!(Test-Path "$external_dir"))
{	
    Invoke-Expression -Command "$cwd\scripts\windows\setup.ps1"
}

if($clear)
{
    Write-Host "Execute clean ..."
    clear_cmake $output_dir
}
else
{
    Write-Host "Execute Windows project generation ..."
    if($unity)
    {
        generate_cmake $output_dir -unity
    }
    else
    {
        generate_cmake $output_dir
    }
}

Write-Host "Generation script completed" -ForegroundColor green