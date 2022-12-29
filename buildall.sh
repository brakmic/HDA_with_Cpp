#!/bin/bash
rm -rf builddir
meson setup builddir
meson compile -C builddir