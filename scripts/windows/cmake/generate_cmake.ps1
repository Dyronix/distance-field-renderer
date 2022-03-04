# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$arguments = $args

$cwd = Get-Location

Write-Host "Loading windows/cmake/generate_cmake.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

Function generate_cmake
{
    Param
    (
        [String]
        $outputDir
    )

    # Check for a build folder
    if(!(Test-Path "$cwd\build")) 
    {
        Write-Host "Setup build directory: $cwd\build"
        Push-Location -Path $cwd
            New-Item -ItemType Directory -Path "build" | Out-Null
        Pop-Location
    }

    # Check for a ouput folder
    if([string]::IsNullOrEmpty($outputDir))
    {
        $outputDir = "windows"
    }

    if(!(Test-Path "$cwd\build\$outputDir"))
    {
        Write-Host "Setup output directory: $cwd\build\$outputDir"
        Push-Location "$cwd\build"
            New-Item -ItemType Directory -Path "$outputDir" | Out-Null
        Pop-Location
    }

    if($unity)
    {
        Write-Host "Running command: cmake --no-warn-unused-cli -B$cwd\build\$outputDir -G Visual Studio 16 2019 -T host=x64 -A x64 -DREX_UNITY_BUILD=1 -DREX_EMSCRIPTEN_BUILD=0"
        cmake --no-warn-unused-cli -B"$cwd\build\$outputDir" -G "Visual Studio 16 2019" -T host=x64 -A x64 -DREX_UNITY_BUILD=1 -DREX_EMSCRIPTEN_BUILD=0  
    }
    else
    {
        Write-Host "Running command: cmake --no-warn-unused-cli -B$cwd\build\$outputDir -G Visual Studio 16 2019 -T host=x64 -A x64 -DREX_UNITY_BUILD=0 -DREX_EMSCRIPTEN_BUILD=0"
        cmake --no-warn-unused-cli -B"$cwd\build\$outputDir" -G "Visual Studio 16 2019" -T host=x64 -A x64 -DREX_UNITY_BUILD=0 -DREX_EMSCRIPTEN_BUILD=0
    }
}