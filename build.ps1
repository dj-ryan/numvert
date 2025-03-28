# build.ps1 - Build script for creating MSI installer
# Assumes you have the WiX Toolset installed and added to your PATH

# Step 1: Build the Rust application in release mode
Write-Host "Building Rust application in release mode..."
cargo build --release

# Step 2: Compile the WiX source file to an object file
Write-Host "Compiling WiX source..."
candle.exe installer.wxs -out installer.wixobj

# Step 3: Link the object file to create the MSI
Write-Host "Linking to create MSI..."
light.exe installer.wixobj -out numvert.msi

Write-Host "Build complete. MSI installer created as numvert.msi"