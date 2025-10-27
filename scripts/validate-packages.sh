#!/bin/bash
# Package validation script for qualpal
# Tests that generated packages contain expected files and metadata

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

PASS=0
FAIL=0

# Test function
test_condition() {
    local description="$1"
    local condition="$2"
    
    if eval "$condition"; then
        echo -e "${GREEN}✅ $description${NC}"
        ((PASS++))
    else
        echo -e "${RED}❌ $description${NC}"
        ((FAIL++))
    fi
}

echo "=== Package Validation Script ==="
echo

# Find packages
DEB_PACKAGES=(build/*.deb)
RPM_PACKAGES=(build/*.rpm)
TGZ_PACKAGES=(build/*.tar.gz)

echo "Found packages:"
ls -la build/*.{deb,rpm,tar.gz} 2>/dev/null || echo "No packages found"
echo

# Test DEB packages
echo "=== Testing DEB packages ==="
for deb in "${DEB_PACKAGES[@]}"; do
    if [ -f "$deb" ]; then
        echo "Testing $(basename "$deb"):"
        
        # Test package format
        test_condition "DEB format is valid" "dpkg-deb --info '$deb' > /dev/null 2>&1"
        
        # Test required fields
        test_condition "Has Package field" "dpkg-deb --field '$deb' Package | grep -q ."
        test_condition "Has Version field" "dpkg-deb --field '$deb' Version | grep -q ."
        test_condition "Has Architecture field" "dpkg-deb --field '$deb' Architecture | grep -q ."
        test_condition "Has Maintainer field" "dpkg-deb --field '$deb' Maintainer | grep -q ."
        test_condition "Has Description field" "dpkg-deb --field '$deb' Description | grep -q ."
        
        # Test file contents
        if [[ "$deb" == *"Applications"* ]] || [[ "$deb" == *"qualpal_"* ]]; then
            test_condition "CLI package contains binary" "dpkg-deb --contents '$deb' | grep -q '/usr/bin/qualpal'"
            test_condition "CLI package contains man page" "dpkg-deb --contents '$deb' | grep -q 'qualpal.1'"
        elif [[ "$deb" == *"Runtime"* ]] || [[ "$deb" == *"libqualpal1"* ]]; then
            test_condition "Runtime package contains shared library" "dpkg-deb --contents '$deb' | grep -q 'libqualpal.so'"
        elif [[ "$deb" == *"Development"* ]] || [[ "$deb" == *"dev"* ]]; then
            test_condition "Dev package contains headers" "dpkg-deb --contents '$deb' | grep -q '/usr/include/'"
            test_condition "Dev package contains static library" "dpkg-deb --contents '$deb' | grep -q 'libqualpal.a'"
            test_condition "Dev package contains CMake config" "dpkg-deb --contents '$deb' | grep -q 'cmake/qualpal/'"
        fi
        echo
    fi
done

# Test RPM packages  
echo "=== Testing RPM packages ==="
for rpm in "${RPM_PACKAGES[@]}"; do
    if [ -f "$rpm" ]; then
        echo "Testing $(basename "$rpm"):"
        
        # Test package format
        test_condition "RPM format is valid" "rpm -qp '$rpm' > /dev/null 2>&1"
        
        # Test required fields
        test_condition "Has Name field" "rpm -qp --queryformat '%{NAME}' '$rpm' | grep -q ."
        test_condition "Has Version field" "rpm -qp --queryformat '%{VERSION}' '$rpm' | grep -q ."
        test_condition "Has Architecture field" "rpm -qp --queryformat '%{ARCH}' '$rpm' | grep -q ."
        test_condition "Has Summary field" "rpm -qp --queryformat '%{SUMMARY}' '$rpm' | grep -q ."
        
        # Test file contents
        if [[ "$rpm" == *"qualpal-"* ]] && [[ "$rpm" != *"libqualpal"* ]]; then
            test_condition "CLI package contains binary" "rpm -qlp '$rpm' | grep -q '/usr/bin/qualpal'"
        elif [[ "$rpm" == *"libqualpal1-"* ]] && [[ "$rpm" != *"devel"* ]]; then
            test_condition "Runtime package contains shared library" "rpm -qlp '$rpm' | grep -q 'libqualpal.so'"
        elif [[ "$rpm" == *"devel"* ]]; then
            test_condition "Dev package contains headers" "rpm -qlp '$rpm' | grep -q '/usr/include/'"
            test_condition "Dev package contains static library" "rpm -qlp '$rpm' | grep -q 'libqualpal.a'"
        fi
        echo
    fi
done

# Test TGZ packages
echo "=== Testing TGZ packages ==="
for tgz in "${TGZ_PACKAGES[@]}"; do
    if [ -f "$tgz" ]; then
        echo "Testing $(basename "$tgz"):"
        
        test_condition "TGZ format is valid" "tar -tzf '$tgz' > /dev/null 2>&1"
        test_condition "Contains binary" "tar -tzf '$tgz' | grep -q 'usr/bin/qualpal'"
        test_condition "Contains headers" "tar -tzf '$tgz' | grep -q 'usr/include/'"
        test_condition "Contains libraries" "tar -tzf '$tgz' | grep -q 'libqualpal'"
        echo
    fi
done

# Summary
echo "=== Validation Summary ==="
echo -e "${GREEN}Passed: $PASS${NC}"
echo -e "${RED}Failed: $FAIL${NC}"

if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}🎉 All package validation tests passed!${NC}"
    exit 0
else
    echo -e "${RED}💥 Some package validation tests failed!${NC}"
    exit 1
fi