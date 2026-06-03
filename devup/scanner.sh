#!/bin/bash
get_version() {
    if command -v "$1" &> /dev/null; then
        version=$($2 2>&1 | head -n 1 | grep -oE '[0-9]+\.[0-9]+(\.[0-9]+)?' | head -n 1)
        [ -z "$version" ] && echo "unknown" || echo "$version"
    else
        echo "not_installed"
    fi
}
echo "Kernel;$(uname -r | grep -oE '^[0-9]+\.[0-9]+\.[0-9]+');6.8.0"
echo "Bash;$(get_version bash "bash --version");5.2.0"
echo "GCC;$(get_version gcc "gcc --version");13.2.0"
echo "Python;$(get_version python3 "python3 --version");3.12.0"
echo "Git;$(get_version git "git --version");2.45.0"
echo "Curl;$(get_version curl "curl --version");8.7.0"