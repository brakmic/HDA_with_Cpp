project(
    'demo_web_server',
    'cpp',
    version: '0.1',
    default_options: ['warning_level=3', 'cpp_std=c++20'],
)

cpp = meson.get_compiler('cpp')

# Compiler-agnostic warning flags.
# Meson's warning_level=3 maps to -Wall -Wextra -Wpedantic on GCC/Clang
# and /W4 on MSVC. Additional flags are compiler-specific.
cpp_args = []
if cpp.get_id() == 'msvc'
    cpp_args += ['/permissive-', '/Zc:__cplusplus', '/utf-8']
elif cpp.get_id() == 'gcc' or cpp.get_id() == 'clang'
    cpp_args += [
        '-Wshadow',
        '-Wformat=2',
        '-Wconversion',
        '-Wnull-dereference',
        '-Wsign-conversion',
        '-D_FORTIFY_SOURCE=2',
        '-D__STDC_LIMIT_MACROS',
    ]
endif

os = host_machine.system()
src_root = 'src'
test_root = 'test'

# Windows system libraries for static linking
win_sys_libs = []
if os == 'windows'
    win_sys_libs = [
        cpp.find_library('ws2_32', required: true),
        cpp.find_library('crypt32', required: true),
        cpp.find_library('rpcrt4', required: true),
        cpp.find_library('secur32', required: true),
        cpp.find_library('shlwapi', required: true),
        cpp.find_library('iphlpapi', required: true),
        cpp.find_library('cryptui', required: true),
        cpp.find_library('ncrypt', required: true),
    ]
    # c-ares is needed by trantor but Meson doesn't resolve the
    # $<LINK_ONLY:c-ares::cares> generator expression in trantor's CMake config.
    c_ares_dep = dependency('c-ares', method: 'cmake', modules: ['c-ares::cares'], required: false)
    if not c_ares_dep.found()
        c_ares_dep = dependency('libcares', required: false)
    endif
else
    # On Linux, c-ares is already linked via drogon.pc
    c_ares_dep = dependency('', required: false)
endif

# Dependencies via vcpkg or system packages
# Pattern: try CMake target first (vcpkg), fall back to pkg-config (Linux)
drogon_dep = dependency('drogon', required: true)
fmt_dep = dependency('fmt', required: true)
argparse_dep = dependency('argparse', required: true)

# SOCI: vcpkg uses CMake targets; system packages use pkg-config
soci_dep = dependency('soci', modules: ['SOCI::soci_core_static', 'SOCI::soci_sqlite3_static'], required: false)
if not soci_dep.found()
    soci_dep = dependency('soci', modules: ['SOCI::soci_core', 'SOCI::soci_sqlite3'], required: false)
endif
if not soci_dep.found()
    soci_core_dep = dependency('soci_core', required: true)
    soci_sqlite3_dep = dependency('soci_sqlite3', required: true)
    soci_dep = [soci_core_dep, soci_sqlite3_dep]
endif

# SQLite3: vcpkg uses unofficial-sqlite3 CMake target; Linux uses sqlite3 pkg-config
sqlite3_dep = dependency('unofficial-sqlite3', modules: ['unofficial::sqlite3::sqlite3'], required: false)
if not sqlite3_dep.found()
    sqlite3_dep = dependency('sqlite3', required: true)
endif

# JSON, OpenSSL, Zlib are standard pkg-config names on all platforms
jsoncpp_dep = dependency('jsoncpp', required: true)
openssl_dep = dependency('openssl', required: true)
zlib_dep = dependency('zlib', required: true)

libcriterion = dependency('criterion', required: false)
build_tests = libcriterion.found()

# Brotli: vcpkg uses unofficial-brotli CMake target; Linux uses pkg-config
brotli_dep = dependency('unofficial-brotli', modules: ['unofficial::brotli::brotlienc', 'unofficial::brotli::brotlidec', 'unofficial::brotli::brotlicommon'], required: false)
if not brotli_dep.found()
    brotli_enc_dep = dependency('libbrotlienc', required: true)
    brotli_dec_dep = dependency('libbrotlidec', required: true)
    brotli_com_dep = dependency('libbrotlicommon', required: true)
    brotli_dep = [brotli_enc_dep, brotli_dec_dep, brotli_com_dep]
endif

# Find drogon_ctl on PATH. Users of vcpkg should add
#   <vcpkg_root>/installed/<triplet>/tools/drogon
# to their PATH before running meson.
drogon_ctl = find_program('drogon_ctl', required: true)

db_dir = src_root / 'database'
controllers_dir = src_root / 'controllers'
models_dir = src_root / 'models'
server_dir = src_root / 'server'
dtos_dir = src_root / 'dtos'
views_dir = src_root / 'views'
view_templates_dir = src_root / 'templates'

main_cpp = src_root / 'demo_web_server.cpp'
controllers_src = [
    controllers_dir / 'home/home.cpp',
    controllers_dir / 'contacts/contacts.cpp',
]
db_src = db_dir / 'db_mgr.cpp'
server_src = [server_dir / 'server_config.cpp']
dtos_src = [dtos_dir / 'contact_dto.cpp']

# Drogon view generation from CSP templates
drogon_ctl_dirs = []
if os == 'windows'
    # Use VCPKG_ROOT env var if set, otherwise rely on PATH
    vcpkg_root = run_command(
        'powershell', '-NoProfile', '-Command', '$env:VCPKG_ROOT',
        check: false,
    ).stdout().strip()
    if vcpkg_root != ''
        # Search under x64-windows-static-md and x64-mingw-dynamic triplets
        drogon_ctl_dirs += join_paths(vcpkg_root, 'installed', 'x64-windows-static-md', 'tools', 'drogon')
        drogon_ctl_dirs += join_paths(vcpkg_root, 'installed', 'x64-mingw-dynamic', 'tools', 'drogon')
    endif
endif
drogon_ctl = find_program('drogon_ctl', dirs: drogon_ctl_dirs, required: true)

csp_templates = [
    'contact.csp',
    'contacts.csp',
    'contact_new.csp',
    'contact_edit.csp',
]

views_src = []
foreach template : csp_templates
    r = run_command(
        drogon_ctl,
        'create',
        'view',
        join_paths(meson.project_source_root(), view_templates_dir, template),
        '-o',
        join_paths(meson.project_source_root(), views_dir),
        check: true,
    )
    message(template + ': ' + r.stdout().strip())
    view_name = template.substring(0, -4)
    views_src += views_dir / (view_name + '.cc')
endforeach

# Executable
all_deps = [
    drogon_dep,
    fmt_dep,
    argparse_dep,
    soci_dep,
    sqlite3_dep,
    jsoncpp_dep,
    openssl_dep,
    brotli_dep,
    zlib_dep,
] + win_sys_libs
if c_ares_dep.found()
    all_deps += c_ares_dep
endif

executable(
    'demo_web_server',
    main_cpp,
    controllers_src,
    db_src,
    server_src,
    dtos_src,
    views_src,
    cpp_args: cpp_args,
    include_directories: include_directories('src'),
    dependencies: all_deps,
    install: true,
)

# Tests — only built when Criterion is available
if build_tests
test_src = [
    test_root / 'test_demo_web_server.cpp',
    test_root / 'test_contact_dto.cpp',
    test_root / 'test_contacts_controller.cpp',
]

test_exe = executable(
    'test_demo_web_server',
    test_src,
    server_src,
    dtos_src,
    db_src,
    cpp_args: cpp_args,
    include_directories: include_directories('src'),
    dependencies: [
        drogon_dep,
        fmt_dep,
        argparse_dep,
        soci_dep,
        sqlite3_dep,
        jsoncpp_dep,
        openssl_dep,
        brotli_dep,
        zlib_dep,
        libcriterion,
    ] + win_sys_libs,
)

test('demo_web_server_tests', test_exe)
endif

# Copy static assets to build directory.
# configure_file with copy:true only supports flat output (no subdirs).
# Flat files use configure_file; structured vendor files use run_command.

# Flat root-level assets
flat_assets = [
    'config.json',
    'server_config.json',
    'index.html',
    'styles.css',
    'contacts.csv',
]

# Videos for the landing page
video_assets = [
    'videos/htmx_with_cpp.gif',
]
foreach fa : flat_assets
    configure_file(input: fa, output: fa, copy: true)
endforeach

# Vendor files with directory structure — copy with PowerShell
vendor_assets = [
    'vendor/htmx.min.js',
    'vendor/hyperscript.min.js',
    'vendor/bootstrap-5.3.5-dist/css/bootstrap.min.css',
    'vendor/bootstrap-5.3.5-dist/js/bootstrap.bundle.min.js',
    'vendor/fontawesome-free-6.7.2-web/css/fontawesome.min.css',
    'vendor/fontawesome-free-6.7.2-web/css/brands.min.css',
    'vendor/fontawesome-free-6.7.2-web/css/solid.min.css',
    'vendor/fontawesome-free-6.7.2-web/webfonts/fa-brands-400.woff2',
    'vendor/fontawesome-free-6.7.2-web/webfonts/fa-solid-900.woff2',
]
# Platform-specific copy command for vendor assets
copy_cmd = []
if os == 'windows'
    copy_cmd = ['powershell', '-Command']
endif

foreach va : vendor_assets
    if os == 'windows'
        # Ensure target directory exists
        run_command(
            'powershell', '-Command',
            'New-Item -ItemType Directory -Force -Path (Split-Path -Parent ' +
            join_paths(meson.project_build_root(), va) + ') | Out-Null',
            check: false,
        )
        run_command(
            'powershell', '-Command',
            'Copy-Item -Force ' +
            join_paths(meson.project_source_root(), va) + ' ' +
            join_paths(meson.project_build_root(), va),
            check: true,
        )
    else
        run_command(
            'sh', '-c',
            'mkdir -p "$(dirname ' + join_paths(meson.project_build_root(), va) + ')" && ' +
            'cp ' + join_paths(meson.project_source_root(), va) + ' ' +
            join_paths(meson.project_build_root(), va),
            check: true,
        )
    endif
endforeach

# Copy video assets
foreach va : video_assets
    if os == 'windows'
        run_command(
            'powershell', '-Command',
            'New-Item -ItemType Directory -Force -Path (Split-Path -Parent ' +
            join_paths(meson.project_build_root(), va) + ') | Out-Null',
            check: false,
        )
        run_command(
            'powershell', '-Command',
            'Copy-Item -Force ' +
            join_paths(meson.project_source_root(), va) + ' ' +
            join_paths(meson.project_build_root(), va),
            check: true,
        )
    else
        run_command(
            'sh', '-c',
            'mkdir -p "$(dirname ' + join_paths(meson.project_build_root(), va) + ')" && ' +
            'cp ' + join_paths(meson.project_source_root(), va) + ' ' +
            join_paths(meson.project_build_root(), va),
            check: true,
        )
    endif
endforeach
