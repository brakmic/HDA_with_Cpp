# build script for Windows
Remove-Item builddir -Force -Recurse
meson setup builddir
Copy-Item -Path .\index.html -Destination .\builddir\index.html -Force
Copy-Item -Path .\cpp_favicon.ico -Destination .\builddir\cpp_favicon.ico -Force
Copy-Item -Path .\styles.css -Destination .\builddir\styles.css -Force
Copy-Item -Path .\vendor -Destination .\builddir\ -Recurse -Force
Copy-Item -Path .\videos -Destination .\builddir\ -Recurse -Force
meson compile -C builddir
