# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$cwd = Get-Location

Write-Host "Loading windows/utilities/download_archive.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

Function download_archive
{
    Param
    (
        [String]
        $outputDir,
        [String]
        $outputDirName,
        [String]
        $outputArchiveName,
        [String]
        $repositoryURL
    )

    Write-Host "Testing path: $outputDir\$outputDirName"

    # Check that we have the library
    if (!(Test-Path "$outputDir\$outputDirName"))
    {
        try
        {
            Write-Host "Downloading library into folder $outputDirName"
            $WebClient = New-Object System.Net.WebClient
            $WebClient.DownloadFile("$repositoryURL", "$outputDir\archive.zip")
        }
        catch
        {
            Write-Host "Something went wrong when downloading $repositoryURL library" -ForegroundColor Red
            Write-Host $_
            
            exit
        }

        try
        {
            Write-Host "Unzipping library into $outputDir\$outputDirName ..."
            
            Expand-Archive "$outputDir\archive.zip" "$outputDir\."
            Rename-Item -Path "$outputDir\$outputArchiveName" -NewName "$outputDirName"
            Remove-Item -Path "$outputDir\archive.zip"
        }
        catch
        {
            Write-Host "Something went wrong when unzipping $outputArchiveName library" -ForegroundColor Red
            Write-Host $_
            
            exit
        }
    }
}