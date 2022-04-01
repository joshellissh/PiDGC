#!/bin/sh

# Copy over binaries
echo "Optimizing display binary..."
strip --strip-all "../source/build-display-RPi3-Release/display"
echo "Copying display binary to image..."
rm -f "$TARGET_DIR/root/display"
cp "../source/build-display-RPi3-Release/display" "$TARGET_DIR/root/display1"
