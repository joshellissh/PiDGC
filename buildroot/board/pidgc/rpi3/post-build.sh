#!/bin/sh

# Copy over binaries
echo "Optimizing display binary..."
strip --strip-all "../source/rpi3-display/display"
echo "Copying display binary to image..."
rm -f "$TARGET_DIR/root/display"
cp "../source/rpi3-display/display" "$TARGET_DIR/root/display"
