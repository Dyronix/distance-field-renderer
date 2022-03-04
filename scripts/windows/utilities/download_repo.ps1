# Don't allow our script to continue if any errors are observed
$ErrorActionPreference = "Stop"

$cwd = Get-Location

Write-Host "Loading windows/msvc/download_repo.ps1" -ForegroundColor Green
Write-Host "Current working directory: $cwd"

Function download_repo
{
    Param
    (
        [String]
        $externalDir,
        [String]
        $outputDirName,
        [String]
        $outputArchiveName,
        [String]
        $repositoryURL
    )

    Write-Host "Testing path: $external_dir\$outputDirName"

    # Check that we have the guideline support library
    if (!(Test-Path "$external_dir\$outputDirName"))
    {
            try
        {
            Write-Host "Downloading guideline support library into external folder $outputDirName"
            $WebClient = New-Object System.Net.WebClient
            $WebClient.DownloadFile("$repositoryURL", "$externalDir\github-repository.zip")
        }
        catch
        {
            Write-Host "Something went wrong when downloading $repositoryURL library" -ForegroundColor Red
            Write-Host $_
            
            exit
        }

        try
        {
            Write-Host "Unzipping guideline support library into external\$outputDirName ..."
            
            Expand-Archive "$externalDir\github-repository.zip" "$externalDir\."
            Rename-Item -Path "$externalDir\$outputArchiveName" -NewName "$outputDirName"
            Remove-Item -Path "$externalDir\github-repository.zip"
        }
        catch
        {
            Write-Host "Something went wrong when unzipping $outputArchiveName library" -ForegroundColor Red
            Write-Host $_
            
            exit
        }
    }
}