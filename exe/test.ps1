# Get the directory of the current script
Param(
    [Parameter(Mandatory=$false)][switch]$TimeMeasurementsReport = $False,
    [Parameter(Mandatory=$false)][switch]$SkipCliques = $False,
    [Parameter(Mandatory=$false)][switch]$SkipConnectivity = $False
)

$ScriptDir = Split-Path $MyInvocation.MyCommand.Path -Parent
$ProgramPath = Join-Path $ScriptDir "main.out"
$InputDir = Join-Path (Split-Path $ScriptDir -Parent) "examples"
$InputFiles = @(Get-ChildItem -Path $InputDir -Filter *.txt | Select-Object -ExpandProperty FullName)
$ReportFile = Join-Path $ScriptDir "report.txt"
$TestVariants = @(
    @{ K_CLIQUE = 1; L_CONN = 1 }
    @{ K_CLIQUE = 2; L_CONN = 2 }
    @{ K_CLIQUE = 5; L_CONN = 5 }
    @{ K_CLIQUE = 10; L_CONN = 10 }
    @{ K_CLIQUE = 15; L_CONN = 15 }
)

if ($TimeMeasurementsReport -and (Test-Path $ReportFile)) {
    Remove-Item $ReportFile
}

function Capture($Output) {
    $Result = @("====================================")

    $K_Clique = ($Output | Select-String -Pattern "(?<=K_CLIQUE = )\d+").Matches.Value
    $L_Conn = ($Output | Select-String -Pattern "(?<=L_CONN = )\d+").Matches.Value
    $Result += "K_CLIQUE = $K_Clique"
    $Result += "L_CONN = $L_Conn"

    $TestCases = [regex]::split($Output, "-----------------------------Test case \d+-----------------------------")
    $TestCases = $TestCases[1..($TestCases.Length - 1)]

    foreach ($TestCase in $TestCases) {
        $Graph1Size = ($TestCase | Select-String -Pattern "(?<=Graph 1 \(N = )\d+").Matches.Value
        $Graph2Size = ($TestCase | Select-String -Pattern "(?<=Graph 2 \(N = )\d+").Matches.Value
        
        # Order of times: BronKerbosch, Monte Carlo, largest common subgraph, largest common subgraph (approximate), distance
        $Times = ($TestCase | Select-String -AllMatches -Pattern "(?<=\[)\d+(?= us\])").Matches.Value

        if (-not $SkipCliques -and -not $SkipConnectivity) {
            $Result += "Graph 1 size = $Graph1Size"
            $Result += "BronKerbosch time = $($Times[0])"
            $Result += "Monte Carlo time = $($Times[1])"

            $Result += "Graph 2 size = $Graph2Size"
            $Result += "BronKerbosch time = $($Times[2])"
            $Result += "Monte Carlo time = $($Times[3])"

            $Result += "Largest common subgraph time = $($Times[4])"
            $Result += "Largest common subgraph (approximate) time = $($Times[5])"
            $Result += "Distance = $($Times[6])"
        } elseif (-not $SkipCliques) {
            $Result += "Graph 1 size = $Graph1Size"
            $Result += "BronKerbosch time = $($Times[0])"
            $Result += "Monte Carlo time = $($Times[1])"

            $Result += "Graph 2 size = $Graph2Size"
            $Result += "BronKerbosch time = $($Times[2])"
            $Result += "Monte Carlo time = $($Times[3])"
        } elseif (-not $SkipConnectivity) {
            $Result += "Graph 1 size = $Graph1Size"
            $Result += "Graph 2 size = $Graph2Size"

            $Result += "Largest common subgraph time = $($Times[0])"
            $Result += "Largest common subgraph (approximate) time = $($Times[1])"
            $Result += "Distance = $($Times[2])"
        }

        $Result += "-----------------------------"
    }

    return $Result -join "`n"
}

foreach ($Variant in $TestVariants) {
    $env:K_CLIQUE = $Variant.K_CLIQUE
    $env:L_CONN = $Variant.L_CONN

    foreach ($InputFile in $InputFiles) {
        $Output = Get-Content $InputFile | & $ProgramPath
        $Output

        if ($TimeMeasurementsReport) {
            Add-Content $ReportFile (Capture $Output)
        }
        Add-Content $ReportFile "===================================="
    }
}
