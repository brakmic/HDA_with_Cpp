project(
    'demo_web_server',
    'cpp',
    version: '0.1',
    default_options: ['warning_level=3', 'cpp_std=c++20'],
)

cpp_args = [
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-Wshadow',
    '-Wformat=2',
    '-Wconversion',
    '-Wnull-dereference',
    '-Wsign-conversion',
    '-D_FORTIFY_SOURCE=2',
    '-D__STDC_LIMIT_MACROS',
    '-std=c++20',
]

cpp_args_debug = [
    cpp_args,
    '-g3',
    '-ggdb',
]

os = host_machine.system()

# change this to your own vcgpkg triplet
# more info: vcpkg help triplet
triplet = 'x64-linux'
if os == 'windows'
    triplet = 'x64-mingw-static'
endif

src_root = 'src'
test_root = 'test'
# change to your vcpkg root
vcpkg_root = '/home/cppdev/vcpkg'
if os == 'windows'
    vcpkg_root = 'C:/Users/Administrator/projects/vcpkg'
endif

# vendor sources (htmx, bootstrap)
vendor_dir = 'vendor'

# gifs
videos_dir = 'videos'

# favicon
favicon_path = 'cpp_favicon.ico'

# config files
drogon_config = 'config.json'
server_config = 'server_config.json'

db_dir = src_root / 'database'
controllers_dir = src_root / 'controllers'
models_dir = src_root / 'models'
server_dir = src_root / 'server'
dtos_dir = src_root / 'dtos'
views_dir = src_root / 'views'
view_templates_dir = src_root / 'templates'

main_cpp = src_root / 'demo_web_server.cpp'
test_main_cpp = test_root / 'test_demo_web_server.cpp'
controllers_src = [
    controllers_dir / 'home/home.cpp',
    controllers_dir / 'contacts/contacts.cpp',
]
db_src = db_dir / 'db_mgr.cpp'
server_src = [server_dir / 'server_config.cpp']
dtos_src = [dtos_dir / 'contact_dto.cpp']
views_src = [
    views_dir / 'contact.cc',
    views_dir / 'contact_edit.cc',
    views_dir / 'contact_new.cc',
    views_dir / 'contacts.cc',
]

# ------------------------------
# For Windows compilations only
# ------------------------------
# put your local paths in here
drogon_root = 'C:/bin/drogon'
mingw_root = 'C:/bin/msys64/mingw64'
mingw_includes = join_paths(mingw_root, 'include')
drogon_includes = join_paths(drogon_root, 'include')
trantor_includes = join_paths(drogon_root, 'include')
project_includes = join_paths(meson.source_root(), 'include')
jsoncpp_includes = join_paths(mingw_root, 'include')
mingw_libs = join_paths(mingw_root, 'lib')
drogon_libs = join_paths(drogon_root, 'lib')

#--------------------------
# Criterion test framework
#--------------------------
# Ubuntu/Linux: Use system package (apt install libcriterion-dev)
# Windows: Build it locally with: 
# > meson -Dprefix=c:/bin/criterion build
# > ninja -C build install
# 
# macOs users can install it with:
# $ brew install criterion
criterion_root = '/usr/local/opt/criterion'
if os == 'windows'
    criterion_root = 'C:/bin/criterion'
elif os == 'linux'
    # Use system installation on Linux
    criterion_root = '/usr'
endif
if os == 'linux'
    criterion_root = '/usr/include/criterion'
endif

criterion_includes = join_paths(criterion_root, 'include')
if os == 'linux'
    criterion_libs = join_paths(criterion_root, 'lib/x86_64-linux-gnu')
else
    criterion_libs = join_paths(criterion_root, 'lib')
endif

# Criterion test library
if os == 'linux'
    # Use system package on Linux
    libcriterion = dependency('criterion', required: true)
else
    # Use manual library search for other platforms
    libcriterion_name = 'criterion'
    libcriterion = cpp.find_library(
        libcriterion_name,
        dirs: [criterion_libs],
    )
endif

if os != 'windows'
    drogon_root = '/usr/local/bin'
    mingw_root = ''
    mingw_includes = ''
    drogon_includes = vcpkg_root / 'packages/drogon_' + triplet + '/include'
    trantor_includes = vcpkg_root / 'packages/trantor_' + triplet + '/include'
    jsoncpp_includes = vcpkg_root / 'packages/jsoncpp_' + triplet + '/include'
    project_includes = ''
    mingw_libs = ''
    drogon_libs = vcpkg_root / 'packages/drogon_' + triplet + '/lib'
endif

# ------------------------------------------
# Generate Drogon views from CSP templates
# ------------------------------------------
# For every new CSP file you must add a new 
# run_command, so that drogon_ctl can generate
# the corresponding C++ files.
drogon_ctl = find_program('drogon_ctl', dirs: [join_paths(drogon_root, 'bin')])
r = run_command(
    drogon_ctl,
    'create',
    'view',
    join_paths(
        meson.source_root(),
        view_templates_dir / 'contact.csp',
    ),
    '-o',
    join_paths(
        meson.source_root(),
        views_dir,
    ),
)
message('[Contact] ' + r.stdout().strip())

r = run_command(
    drogon_ctl,
    'create',
    'view',
    join_paths(
        meson.source_root(),
        view_templates_dir / 'contacts.csp',
    ),
    '-o',
    join_paths(
        meson.source_root(),
        views_dir,
    ),
)
message('[Contacts] ' + r.stdout().strip())

r = run_command(
    drogon_ctl,
    'create',
    'view',
    join_paths(
        meson.source_root(),
        view_templates_dir / 'contact_new.csp',
    ),
    '-o',
    join_paths(
        meson.source_root(),
        views_dir,
    ),
)
message('[ContactNew] ' + r.stdout().strip())

r = run_command(
    drogon_ctl,
    'create',
    'view',
    join_paths(
        meson.source_root(),
        view_templates_dir / 'contact_edit.csp',
    ),
    '-o',
    join_paths(
        meson.source_root(),
        views_dir,
    ),
)
message('[ContactEdit] ' + r.stdout().strip())

# sources
sources = [main_cpp]
sources += controllers_src
sources += db_src
sources += server_src
sources += views_src
sources += dtos_src

#--------------
# test sources
#--------------
# Include those sources that need to be tested.
# Alternatively, create multiple partial tests
# to keep binaries smaller.
test_sources = [test_main_cpp]
#test_sources += controllers_src
#test_sources += db_src
test_sources += server_src
#test_sources += views_src
#test_sources += dtos_src

# includes
incdir = include_directories(
    [
        project_includes,
        db_dir,
        server_dir,
        models_dir,
        views_dir,
        dtos_dir,
        drogon_includes,
        mingw_includes,
        trantor_includes,
        jsoncpp_includes,
        vcpkg_root / 'packages/argparse_' + triplet + '/include',
        vcpkg_root / 'packages/fmt_' + triplet + '/include',
        vcpkg_root / 'packages/sqlite3_' + triplet + '/include',
        vcpkg_root / 'packages/soci_' + triplet + '/include',
    ],
)

test_incdir = [incdir, criterion_includes]

# find libraries
cpp = meson.get_compiler('cpp')
libdrogon = cpp.find_library(
    'drogon',
    dirs: [drogon_libs],
)

libtrantor_path = vcpkg_root / 'packages/trantor_' + triplet + '/lib'
if os == 'windows'
    libtrantor_path = drogon_libs
endif
libtrantor = cpp.find_library(
    'trantor',
    dirs: [libtrantor_path],
)

libjsoncpp_path = vcpkg_root / 'packages/jsoncpp_' + triplet + '/lib'
if os == 'windows'
    libjsoncpp_path = mingw_libs
endif
libjsoncpp = cpp.find_library(
    'jsoncpp',
    dirs: [libjsoncpp_path],
)

libopenssl_path = vcpkg_root / 'packages/openssl_' + triplet + '/lib'
if os == 'windows'
    libopenssl_path = mingw_libs
endif
libopenssl = cpp.find_library(
    'ssl',
    dirs: [libopenssl_path],
)

libcrypto_path = vcpkg_root / 'packages/openssl_' + triplet + '/lib'
if os == 'windows'
    libcrypto_path = mingw_libs
endif
libcrypto = cpp.find_library(
    'crypto',
    dirs: [libcrypto_path],
)

brotlicommon_static_name = 'brotlicommon'
if os == 'osx'
    brotlicommon_static_name = 'brotlicommon-static'
endif

brotlicommon_path = vcpkg_root / 'packages/brotli_' + triplet + '/lib/'
if os == 'windows'
    brotlicommon_path = mingw_libs
endif
libbrotli_common = cpp.find_library(
    brotlicommon_static_name,
    dirs: [brotlicommon_path],
)

brotlienc_static_name = 'brotlienc'
if os == 'osx'
    brotlienc_static_name = 'brotlienc-static'
endif

brotlienc_path = vcpkg_root / 'packages/brotli_' + triplet + '/lib/'
if os == 'windows'
    brotlienc_path = mingw_libs
endif
libbrotli_enc = cpp.find_library(
    brotlienc_static_name,
    dirs: [brotlienc_path],
)

brotlidec_static_name = 'brotlidec'
if os == 'osx'
    brotlidec_static_name = 'brotlidec-static'
endif

brotlidec_path = vcpkg_root / 'packages/brotli_' + triplet + '/lib/'
if os == 'windows'
    brotlidec_path = mingw_libs
endif
libbrotli_dec = cpp.find_library(
    brotlidec_static_name,
    dirs: [brotlidec_path],
)

cares_path = vcpkg_root / 'packages/c-ares_' + triplet + '/lib'
if os == 'windows'
    cares_path = mingw_libs
endif
libcares = cpp.find_library(
    'cares',
    dirs: [cares_path],
)

zlib_name = 'z'
zlib_path = vcpkg_root / 'packages/zlib_' + triplet + '/lib'
if os == 'windows'
    zlib_path = mingw_libs
endif
libz = cpp.find_library(
    zlib_name,
    dirs: [zlib_path],
)

libfmt = cpp.find_library(
    'fmt',
    dirs: [vcpkg_root / 'packages/fmt_' + triplet + '/lib'],
)

libsqlite3 = cpp.find_library(
    'sqlite3',
    dirs: [vcpkg_root / 'packages/sqlite3_' + triplet + '/lib'],
)

soci_core_name = 'soci_core'
if os == 'windows'
    soci_core_name = 'soci_core_4_0'
endif
libsoci_core = cpp.find_library(
    soci_core_name,
    dirs: [vcpkg_root / 'packages/soci_' + triplet + '/lib'],
)

soci_sqlite3_name = 'soci_sqlite3'
if os == 'windows'
    soci_sqlite3_name = 'soci_sqlite3_4_0'
endif

libsoci_sqlite3 = cpp.find_library(
    soci_sqlite3_name,
    dirs: [vcpkg_root / 'packages/soci_' + triplet + '/lib'],
)

libresolv = ''
if os != 'windows'
    libresolv = cpp.find_library('resolv')
endif

libuuid = ''
if os == 'linux'
    # Use vcpkg UUID library with manual path
    libuuid = cpp.find_library(
        'uuid',
        dirs: [vcpkg_root / 'packages/libuuid_' + triplet + '/lib'],
        required: true
    )
endif

librpcrt4 = ''
if os == 'windows'
    librpcrt4 = cpp.find_library('rpcrt4', dirs: [mingw_libs])
endif

libcrypt32 = ''
if os == 'windows'
    libcrypt32 = cpp.find_library('crypt32', dirs: [mingw_libs])
endif

# Criterion test library
if os == 'linux'
    # Use system package on Linux
    libcriterion = dependency('criterion', required: true)
else
    # Use manual library search for other platforms
    libcriterion_name = 'criterion'
    libcriterion = cpp.find_library(
        libcriterion_name,
        dirs: [criterion_libs],
    )
endif


# Will be done only in POSIX systems. On Windows, the files will be 
# copied by the powershell script itself.
if os != 'windows'
    copy = find_program('cp')
    # copy index.html to build directory
    run_command(
        copy,
        join_paths(meson.source_root(), 'index.html'),
        join_paths(meson.build_root(), 'index.html'),
    )

    # copy styles.css to build directory
    run_command(
        copy,
        join_paths(meson.source_root(), 'styles.css'),
        join_paths(meson.build_root(), 'styles.css'),
    )

    # copy vendor sources to build directory
    run_command(
        copy,
        '-r',
        join_paths(meson.source_root(), vendor_dir),
        join_paths(meson.build_root(), vendor_dir),
    )

    # copy gifs to build directory
    run_command(
        copy,
        '-r',
        join_paths(meson.source_root(), videos_dir),
        join_paths(meson.build_root(), videos_dir),
    )

    # copy favicon to build directory
    run_command(
        copy,
        '-r',
        join_paths(meson.source_root(), favicon_path),
        join_paths(meson.build_root(), favicon_path),
    )

    # copy Drogon's config.json to build directory
    run_command(
        copy,
        '-r',
        join_paths(meson.source_root(), drogon_config),
        join_paths(meson.build_root(), drogon_config),
    )

    # copy server_config.json to build directory
    run_command(
        copy,
        '-r',
        join_paths(meson.source_root(), server_config),
        join_paths(meson.build_root(), server_config),
    )
endif

deps = [
    libdrogon,
    libtrantor,
    libjsoncpp,
    libopenssl,
    libcrypto,
    libbrotli_common,
    libbrotli_enc,
    libbrotli_dec,
    libcares,
    libz,
    libfmt,
    libsqlite3,
    libsoci_core,
    libsoci_sqlite3,
]

if os == 'windows'
    deps += [
        librpcrt4,
        libcrypt32,
    ]
endif

if os != 'windows'
    deps += libresolv
endif

if os == 'linux'
    deps += libuuid
endif

link_args = ''
if os == 'windows'
    link_args = ['-lws2_32', '-lssl', '-lcrypto', '-lrpcrt4', '-lcrypt32']
endif
if os == 'linux'
    link_args = ['-luuid']
endif

executable(
    'demo_web_server',
    sources,
    cpp_args: cpp_args,
    link_args: link_args,
    install: true,
    include_directories: incdir,
    dependencies: deps,
)

# Update test dependencies
if os == 'linux'
    test_deps = [deps, libcriterion]
else
    test_deps = [deps, libcriterion]
endif

test_exe = executable(
    'test_demo_web_server',
    test_sources,
    cpp_args: cpp_args_debug,
    link_args: link_args,
    install: true,
    include_directories: test_incdir,
    dependencies: test_deps,
)

test('basic', test_exe)
