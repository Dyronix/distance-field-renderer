# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$cwd = Get-Location
$external_dir = "$cwd\external"

Write-Host "Executing windows\setup.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

Write-Host "Running script for Windows desktop"

if (!(Test-Path "$external_dir"))
{	
    # Check that we have a 'external' folder
	Write-Host "External directory was not found, creating ..."
	New-Item -ItemType Directory -Path "$external_dir"
}

Write-Host "External directory: $external_dir"

# Check that we have the SDL2 external Windows dev library
if (!(Test-Path "$external_dir\sdl-windows"))
{
	try
    {
        Write-Host "Downloading SDL Windows Dev library into external folder sdl-windows ..."
        $WebClient = New-Object System.Net.WebClient
        $WebClient.DownloadFile("https://www.libsdl.org/release/SDL2-devel-2.0.20-VC.zip", "$external_dir\SDL2-devel-2.0.20-VC.zip")
	}
	catch
	{
		Write-Host "Something went wrong when downloading SDL Windows Dev library" -ForegroundColor Red
		Write-Host $_
		
		exit
	}

    try
    {
        Write-Host "Unzipping SDL Windows Dev library into external\sdl-windows ..."
        
        Expand-Archive "$external_dir\SDL2-devel-2.0.20-VC.zip" "$external_dir\."
        Rename-Item -Path "$external_dir\SDL2-2.0.20" -NewName "sdl-windows"
        Remove-Item -Path "$external_dir\SDL2-devel-2.0.20-VC.zip"
    }
    catch
    {
        Write-Host "Something went wrong when unzipping SDL Windows Dev library" -ForegroundColor Red
        Write-Host $_
		
		exit
    }
}

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