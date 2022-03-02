# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$cwd = Get-Location
$external_dir = "external"

Write-Host "Executing windows/setup.ps1"
Write-Host "Current working directory: $cwd"

if (!(Test-Path $external_dir))
{
    # Check that we have a 'external' folder
    Push-Location -Path "$cwd/../../"
        if (!(Test-Path "external"))
        {
            New-Item -ItemType Directory -Path "external"
        }
    Pop-Location

    $external_dir = "$cwd/../../external"
}

# Check that we have the SDL2 external Windows dev library
if (!(Test-Path "$external_dir/sdl-windows"))
{
    try
    {
        Write-Host "Downloading SDL Windows Dev library into external folder sdl-windows ..."
        $WebClient = New-Object System.Net.WebClient
        $WebClient.DownloadFile("https://www.libsdl.org/release/SDL2-devel-2.0.20-VC.zip", "$external_dir/SDL2-devel-2.0.20-VC.zip")

        Write-Host "Unzipping SDL Windows Dev library into external\sdl-windows ..."
        
        Expand-Archive "$external_dir/SDL2-devel-2.0.20-VC.zip" "$external_dir/."
        Rename-Item -Path "$external_dir/SDL2-2.0.20" -NewName "sdl-windows"
        Remove-Item -Path "$external_dir/SDL2-devel-2.0.20-VC.zip"
    }
    catch
    {
        Write-Host "Something threw an exception"
        Write-Host $_
    }
}