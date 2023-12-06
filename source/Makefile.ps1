Param(
    [Parameter(Mandatory = $false)]
    [string]$FunctionName = "Build-All"
)

Set-Location $PSScriptRoot
$exePath = Join-Path $PSScriptRoot "/../exe"

function Build-All {
    Build-Main
    Build-Generator
}

function Build-Main {
    g++ -std=c++17 -Wall -g -pedantic -c main.cpp
    g++ -std=c++17 -Wall -g -pedantic -c multigraph.cpp
    g++ -std=c++17 -Wall -g -pedantic main.o multigraph.o -o $exePath/main.out
}

function Build-Generator {
    g++ -std=c++17 -Wall -g -pedantic generator.cpp -o $exePath/generator.out
}

function Clean {
    Remove-Item *.o
    Remove-Item $exePath/*.out
}

if ($FunctionName -eq "Build-All") {
    Build-All
}
elseif ($FunctionName -eq "Build-Main") {
    Build-Main
}
elseif ($FunctionName -eq "Build-Generator") {
    Build-Generator
}
elseif ($FunctionName -eq "Clean") {
    Clean
}
else {
    Write-Host "Invalid function name"
}
