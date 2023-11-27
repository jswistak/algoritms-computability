# Get the directory of the current script
Param(
    [Parameter(Mandatory)][bool]$TimeMeasurementsReport
)

$ScriptDir = Split-Path $MyInvocation.MyCommand.Path -Parent
$ProgramPath = Join-Path $ScriptDir "main.out"
$InputDir = Join-Path (Split-Path $ScriptDir -Parent) "examples"
$InputFiles = @(Get-ChildItem -Path $InputDir -Filter *.txt | Select-Object -ExpandProperty FullName)
$TestVariants = @(
    @{ K_CLIQUE = 1; L_CONN = 1 }
    @{ K_CLIQUE = 2; L_CONN = 2 }
    @{ K_CLIQUE = 5; L_CONN = 5 }
    @{ K_CLIQUE = 10; L_CONN = 10 }
    @{ K_CLIQUE = 15; L_CONN = 15 }
)

foreach ($Variant in $TestVariants) {
    $env:K_CLIQUE = $Variant.K_CLIQUE
    $env:L_CONN = $Variant.L_CONN

    foreach ($InputFile in $InputFiles) {
        Get-Content $InputFile | & $ProgramPath
    }
}