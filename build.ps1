#!/usr/bin/env pwsh
<#
.SYNOPSIS
VSS-CO OS Build Script - Production Grade

.DESCRIPTION
Builds and runs the VSS-CO OS kernel

.PARAMETER Target
Target to build: build, run, clean, rebuild, all, help

.EXAMPLE
./build.ps1 build
./build.ps1 run
./build.ps1 all
#>

param(
    [Parameter(Position=0)]
    [ValidateSet('help', 'build', 'run', 'clean', 'rebuild', 'all', 'test', 'info', 'status')]
    [string]$Target = 'help'
)

# Color output
function Write-Header {
    param([string]$Message)
    Write-Host "====================================================" -ForegroundColor Cyan
    Write-Host "  $Message" -ForegroundColor Cyan
    Write-Host "====================================================" -ForegroundColor Cyan
}

function Write-Success {
    param([string]$Message)
    Write-Host "[OK] $Message" -ForegroundColor Green
}

function Write-Info {
    param([string]$Message)
    Write-Host "[->] $Message" -ForegroundColor Yellow
}

function Write-Error-Custom {
    param([string]$Message)
    Write-Host "[XX] $Message" -ForegroundColor Red
}

# Targets
function Show-Help {
    Write-Header "VSS-CO OS - Production Build System"
    Write-Host ""
    Write-Host "Usage: ./build.ps1 [target]" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Targets:" -ForegroundColor Green
    Write-Host "  build    - Build the OS (cmake + ninja)"
    Write-Host "  run      - Run the kernel test harness"
    Write-Host "  clean    - Clean build artifacts"
    Write-Host "  rebuild  - Clean and rebuild"
    Write-Host "  all      - Build and run"
    Write-Host "  test     - Run tests"
    Write-Host "  info     - Show build info"
    Write-Host "  status   - Show project status"
    Write-Host "  help     - Show this help message"
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Green
    Write-Host "  ./build.ps1 build"
    Write-Host "  ./build.ps1 run"
    Write-Host "  ./build.ps1 all"
    Write-Host ""
}

function Do-Build {
    Write-Info "Building VSS-CO OS..."
    
    # Create build directory
    if (-not (Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" -Force | Out-Null
    }
    
    # Configure with CMake
    Push-Location build
    try {
        Write-Info "Configuring with CMake..."
        cmake -G Ninja -DARCH=x86_64 ..
        if ($LASTEXITCODE -ne 0) {
            Write-Error-Custom "CMake configuration failed"
            return $false
        }
        
        Write-Info "Building with Ninja..."
        ninja
        if ($LASTEXITCODE -ne 0) {
            Write-Error-Custom "Build failed"
            return $false
        }
        
        Write-Success "Build complete"
        Write-Host "  Kernel: build\test-kernel.exe"
        Write-Host "  Image:  build\os"
        return $true
    }
    finally {
        Pop-Location
    }
}

function Do-Run {
    if (-not (Do-Build)) {
        return
    }
    
    Write-Host ""
    Write-Header "Running VSS-CO OS Kernel"
    Write-Host ""
    
    & ".\build\test-kernel.exe"
}

function Do-Clean {
    Write-Info "Cleaning build artifacts..."
    
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build" -ErrorAction SilentlyContinue
    }
    
    Write-Success "Clean complete"
}

function Do-Rebuild {
    Do-Clean
    Do-Build | Out-Null
    Write-Success "Rebuild complete"
}

function Do-Test {
    if (-not (Do-Build)) {
        return
    }
    
    Write-Info "Running tests..."
    Push-Location build
    try {
        ninja test
    }
    finally {
        Pop-Location
    }
}

function Show-Info {
    Write-Header "VSS-CO OS Build Information"
    Write-Host ""
    Write-Host "Project: VSS-CO OS" -ForegroundColor Green
    Write-Host "Version: 1.0" -ForegroundColor Green
    Write-Host "Status:  Production" -ForegroundColor Green
    Write-Host ""
    Write-Host "Source Code:" -ForegroundColor Yellow
    
    $cFiles = (Get-ChildItem -Recurse -Include "*.c", "*.h" -ErrorAction SilentlyContinue).Count
    Write-Host "  C/Header files: $cFiles"
    
    $sFiles = (Get-ChildItem -Recurse -Include "*.s" -ErrorAction SilentlyContinue).Count
    Write-Host "  Assembly files: $sFiles"
    
    Write-Host ""
    Write-Host "Build Configuration:" -ForegroundColor Yellow
    Write-Host "  Build System: CMake + Ninja"
    Write-Host "  C Standard: C17"
    Write-Host "  C++ Standard: C++20"
    Write-Host "  Architectures: x86-64, ARM64, RISC-V (ready)"
    Write-Host ""
}

function Show-Status {
    Write-Header "VSS-CO OS Status"
    Write-Host ""
    Write-Host "[OK] Implemented:" -ForegroundColor Green
    Write-Host "  - Buddy allocator (memory management)"
    Write-Host "  - Slab allocator (kmalloc/kfree)"
    Write-Host "  - Process scheduler (fork and scheduling)"
    Write-Host "  - Virtual filesystem (VFS)"
    Write-Host "  - Shell (bash-compatible)"
    Write-Host "  - Boot sequence"
    Write-Host ""
    Write-Host "[PLAN] Coming Soon:" -ForegroundColor Yellow
    Write-Host "  - Network stack"
    Write-Host "  - Graphics and GUI"
    Write-Host "  - Package manager"
    Write-Host "  - Security (MAC/ACL)"
    Write-Host "  - Multi-core support"
    Write-Host ""
}

# Main
switch ($Target) {
    'help'    { Show-Help }
    'build'   { Do-Build | Out-Null; if ($?) { Write-Host "" } }
    'run'     { Do-Run }
    'clean'   { Do-Clean }
    'rebuild' { Do-Rebuild }
    'all'     { Do-Run }
    'test'    { Do-Test }
    'info'    { Show-Info }
    'status'  { Show-Status }
    default   { Show-Help }
}
