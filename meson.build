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
    '-g3',
    '-ggdb',
    '-std=c++20',
]

os = host_machine.system()

# change this to your own vcgpkg triplet
# more info: vcpkg help triplet
triplet = 'x64-osx'
if os == 'windows'
    triplet = 'x64-mingw-static'
endif

src_root = 'src'
# change to your vcpkg root
vcpkg_root = '/Users/brakmic/projects/vcpkg'

# vendor sources (htmx, bootstrap)
vendor_dir = 'vendor'

db_dir = src_root / 'database'
controllers_dir = src_root / 'controllers'
models_dir = src_root / 'models'
dtos_dir = src_root / 'dtos'
views_dir = src_root / 'views'
view_templates_dir = src_root / 'templates'

main_cpp = src_root / 'demo_web_server.cpp'
controllers_src = [
    controllers_dir / 'home/home.cpp',
    controllers_dir / 'contacts/contacts.cpp',
]
db_src = db_dir / 'db_mgr.cpp'
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
sources += views_src
sources += dtos_src

# includes
incdir = include_directories(
    [
        project_includes,
        db_dir,
        models_dir,
        views_dir,
        dtos_dir,
        drogon_includes,
        mingw_includes,
        trantor_includes,
        jsoncpp_includes,
        # vcpkg_root / 'packages/drogon_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-utility_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-config_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-io_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-throw-exception_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-assert_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-functional_' + triplet + '/include',
        # vcpkg_root / 'packages/boost-container_' + triplet + '/include',
        # vcpkg_root / 'packages/trantor_' + triplet + '/include',
        # vcpkg_root / 'packages/jsoncpp_' + triplet + '/include',
        vcpkg_root / 'packages/fmt_' + triplet + '/include',
        vcpkg_root / 'packages/sqlite3_' + triplet + '/include',
        vcpkg_root / 'packages/soci_' + triplet + '/include',
    ],
)
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

brotlicommon_static_name = 'brotlicommon-static'
if os == 'windows'
    brotlicommon_static_name = 'brotlicommon'
endif

brotlicommon_path = vcpkg_root / 'packages/brotli_' + triplet + '/lib/'
if os == 'windows'
    brotlicommon_path = mingw_libs
endif
libbrotli_common = cpp.find_library(
    brotlicommon_static_name,
    dirs: [brotlicommon_path],
)

brotlienc_static_name = 'brotlienc-static'
if os == 'windows'
    brotlienc_static_name = 'brotlienc'
endif

brotlienc_path = vcpkg_root / 'packages/brotli_' + triplet + '/lib/'
if os == 'windows'
    brotlienc_path = mingw_libs
endif
libbrotli_enc = cpp.find_library(
    brotlienc_static_name,
    dirs: [brotlienc_path],
)

brotlidec_static_name = 'brotlidec-static'
if os == 'windows'
    brotlidec_static_name = 'brotlidec'
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

librpcrt4 = ''
if os == 'windows'
    librpcrt4 = cpp.find_library('rpcrt4', dirs: [mingw_libs])
endif

libcrypt32 = ''
if os == 'windows'
    libcrypt32 = cpp.find_library('crypt32', dirs: [mingw_libs])
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

    # copy vendor sources to build directory
    run_command(
        copy,
        '-r',
        join_paths(meson.source_root(), vendor_dir),
        join_paths(meson.build_root(), vendor_dir),
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

link_args = ''
if os == 'windows'
    link_args = ['-lws2_32', '-lssl', '-lcrypto', '-lrpcrt4', '-lcrypt32']
endif

exe = executable(
    'demo_web_server',
    sources,
    cpp_args: cpp_args,
    link_args: link_args,
    install: true,
    include_directories: [incdir],
    dependencies: deps,
)

test('basic', exe)
