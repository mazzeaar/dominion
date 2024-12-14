#!/bin/bash

# Print help message
print_help() {
    echo "Help Message Not Implemented Yet"
}


# Parse command line arguments
while [ "$#" -gt 0 ]; do
    case "$1" in
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            echo "Error: Unrecognized option: $1" 1>&2
            print_help
            exit 1
            ;;
    esac
done

# Check if the 'build-essential' package is installed
if ! dpkg -l | grep -q "^ii  build-essential"; then
    echo "'build-essential' is not installed. Installing..."
    sudo apt update && sudo apt install -y build-essential
else
    echo "'build-essential' is already installed."
fi

# Check if the 'libgtk-3-dev' package is installed
if ! dpkg -l | grep -q "^ii  libgtk-3-dev"; then
    echo "'libgtk-3-dev' is not installed. Installing..."
    sudo apt update && sudo apt install -y libgtk-3-dev
else
    echo "'libgtk-3-dev' is already installed."
fi

# Check if cmake is installed
if ! command -v cmake &> /dev/null; then
    echo "cmake not found, installing..."

    # Update the package list
    sudo apt update

    # Install cmake
    sudo apt install -y cmake

    # Confirm installation
    if command -v cmake &> /dev/null; then
        echo "cmake installed successfully."
    else
        echo "Failed to install cmake."
    fi
else
    echo "cmake is already installed."
fi

# Variable for status of wxWidgets
# 0 = installed with a high enough version
# 1 = not installed
# 2 = installed, but version not high enough
WXWIDGETS_STATUS=0

#Check if wxWidgets is installed
if ! command -v wx-config &> /dev/null; then
    WXWIDGETS_STATUS=1
else
    # Get the wxWidgets version
    WX_VERSION=$(wx-config --version)

    # Compare the version with 3.2.6
    if [[ "$(printf '%s\n' "$WX_VERSION" "3.2.6" | sort -V | head -n1)" == "3.2.6" ]]; then
        WXWIDGETS_STATUS=0
    else
        WXWIDGETS_STATUS=2
    fi
fi

if [ "$WXWIDGETS_STATUS" -eq 2 ]; then
    echo "WxWidgets version too low, removing it"
    # List of wxWidgets-related packages
    PACKAGES=("libwxgtk3.0-gtk3-dev" "libwxgtk3.0-0v5" "libwxbase3.0-0v5" "wx-common")

    for PACKAGE in "${PACKAGES[@]}"; do
        # Check if the package is installed
        if dpkg -l | grep -q "^ii  $PACKAGE"; then
            echo "Removing $PACKAGE..."
            sudo apt-get remove --purge -y $PACKAGE
        else
            echo "$PACKAGE is not installed."
        fi
    done

    # Clean up unused packages and dependencies
    sudo apt-get autoremove -y
    sudo apt-get clean
    echo "wxWidgets and related packages have been removed."
    WXWIDGETS_STATUS==1
fi

if [ "$WXWIDGETS_STATUS" -eq 0 ]; then
    echo "WxWidgets installation found and version high enough"
else
    echo "installing wxWidgets"
    cd ~
    git clone --recurse-submodules https://github.com/wxWidgets/wxWidgets.git
    cd ./wxWidgets
    mkdir buildgtk
    cd buildgtk
    ../configure --with-gtk
    make -j$(nproc)
    sudo make install
    sudo ldconfig
fi

# Search for the "dominion" directory, starting from current directory and going up
DIR="$HOME"
while [ "$DIR" != "/" ]; do
    if [ -d "$DIR/dominion" ]; then
        DOMINION_DIR="$DIR/dominion"
        break
    fi
    DIR=$(dirname "$DIR")
done

if [ -z "$DOMINION_DIR" ]; then
    echo "Error: Could not find dominion directory" 1>&2
    exit 1
fi

if [ ! -d "$DOMINION_DIR/build" ]; then
    echo "Creating build directory"
    mkdir "$DOMINION_DIR/build"
fi

EXEC_DIR="$DOMINION_DIR/build"

# build dominion
cd $EXEC_DIR
cmake ..
make -j$(nproc)

