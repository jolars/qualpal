#!/bin/bash
# Install qualpal from GitHub releases
# Usage: curl -sSL https://raw.githubusercontent.com/jolars/qualpal/main/scripts/install.sh | bash

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
REPO="jolars/qualpal"
BASE_URL="https://github.com/${REPO}/releases/latest/download"

# Detect OS and package manager
detect_system() {
    if command -v dpkg >/dev/null 2>&1; then
        echo "deb"
    elif command -v rpm >/dev/null 2>&1; then
        echo "rpm"
    else
        echo "unknown"
    fi
}

# Get architecture
get_arch() {
    case $(uname -m) in
        x86_64) echo "amd64" ;;
        aarch64|arm64) echo "arm64" ;;
        *) echo "unknown" ;;
    esac
}

# Get RPM architecture name  
get_rpm_arch() {
    case $(uname -m) in
        x86_64) echo "x86_64" ;;
        aarch64|arm64) echo "aarch64" ;;
        *) echo "unknown" ;;
    esac
}

# Download and install DEB packages
install_deb() {
    local arch="$1"
    local version="$2"
    
    echo -e "${GREEN}Installing DEB packages for ${arch}...${NC}"
    
    # Download runtime library
    echo "Downloading libqualpal1..."
    wget -q "${BASE_URL}/libqualpal1_${version}_${arch}.deb" -O "/tmp/libqualpal1.deb"
    
    # Download CLI tool
    echo "Downloading qualpal CLI..."
    wget -q "${BASE_URL}/qualpal_${version}_${arch}.deb" -O "/tmp/qualpal.deb"
    
    # Install packages
    echo "Installing packages..."
    sudo dpkg -i /tmp/libqualpal1.deb /tmp/qualpal.deb
    
    # Clean up
    rm -f /tmp/libqualpal1.deb /tmp/qualpal.deb
    
    echo -e "${GREEN}Installation complete!${NC}"
}

# Download and install RPM packages  
install_rpm() {
    local arch="$1"
    local version="$2"
    
    echo -e "${GREEN}Installing RPM packages for ${arch}...${NC}"
    
    # Get RPM architecture name
    local rpm_arch=$(get_rpm_arch)
    
    # Download runtime library
    echo "Downloading libqualpal1..."
    wget -q "${BASE_URL}/libqualpal1-${version}.${rpm_arch}.rpm" -O "/tmp/libqualpal1.rpm"
    
    # Download CLI tool
    echo "Downloading qualpal CLI..."
    wget -q "${BASE_URL}/qualpal-${version}.${rpm_arch}.rpm" -O "/tmp/qualpal.rpm"
    
    # Install packages
    echo "Installing packages..."
    sudo rpm -i /tmp/libqualpal1.rpm /tmp/qualpal.rpm
    
    # Clean up  
    rm -f /tmp/libqualpal1.rpm /tmp/qualpal.rpm
    
    echo -e "${GREEN}Installation complete!${NC}"
}

# Main installation function
main() {
    echo -e "${GREEN}qualpal installer${NC}"
    echo "Installing qualpal from GitHub releases..."
    echo
    
    # Detect system
    SYSTEM=$(detect_system)
    ARCH=$(get_arch)
    
    if [ "$SYSTEM" = "unknown" ]; then
        echo -e "${RED}Error: Unsupported system. This installer supports Debian/Ubuntu (dpkg) and Red Hat/Fedora/SUSE (rpm) systems.${NC}"
        echo "You can manually download packages from: https://github.com/${REPO}/releases/latest"
        exit 1
    fi
    
    if [ "$ARCH" = "unknown" ]; then
        echo -e "${RED}Error: Unsupported architecture $(uname -m).${NC}"
        echo "Supported architectures: x86_64, aarch64/arm64"
        exit 1
    fi
    
    echo "Detected system: $SYSTEM"
    echo "Detected architecture: $ARCH"
    echo
    
    # Get latest version from GitHub API
    echo "Fetching latest version..."
    VERSION=$(curl -s "https://api.github.com/repos/${REPO}/releases/latest" | grep '"tag_name"' | cut -d'"' -f4 | sed 's/^v//')
    
    if [ -z "$VERSION" ]; then
        echo -e "${RED}Error: Could not fetch latest version.${NC}"
        exit 1
    fi
    
    echo "Latest version: $VERSION"
    echo
    
    # Install based on system type
    case $SYSTEM in
        deb)
            install_deb "$ARCH" "$VERSION"
            ;;
        rpm)
            install_rpm "$ARCH" "$VERSION"
            ;;
    esac
    
    # Test installation
    echo
    echo "Testing installation..."
    if command -v qualpal >/dev/null 2>&1; then
        echo -e "${GREEN}✅ qualpal CLI installed successfully!${NC}"
        echo "Try: qualpal --help"
    else
        echo -e "${YELLOW}⚠️  qualpal CLI not found in PATH. You may need to restart your shell.${NC}"
    fi
}

# Check if running as root
if [ "$EUID" -eq 0 ]; then
    echo -e "${RED}Error: Do not run this script as root. It will prompt for sudo when needed.${NC}"
    exit 1
fi

# Check dependencies
for cmd in wget curl sudo; do
    if ! command -v $cmd >/dev/null 2>&1; then
        echo -e "${RED}Error: Required command '$cmd' not found.${NC}"
        exit 1
    fi
done

# Run main function
main "$@"