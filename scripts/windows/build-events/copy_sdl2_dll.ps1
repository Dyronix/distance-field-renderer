# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$arguments = $args

$cwd=$arguments[0]
$output_dir=$arguments[1]
$external_dir=$arguments[2]

if([string]::IsNullOrEmpty($cwd))
{
	Write-Error "Emprt working directory, pass the working directory for this script as an argument"
}

Write-Host "Loading windows/build-events/copy_sdl2_dll.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

try
{
    Push-Location -Path "$output_dir"
        if (!(Test-Path "SDL2.dll")) 
        {
            Copy-Item -Path "$external_dir\sdl-windows\lib\x64\SDL2.dll"
        }   
    Pop-Location
}
catch
{
    Write-Host "Could not copy item: $external_dir\sdl-windows\lib\x64\SDL2.dll"
    Write-Host $_
}