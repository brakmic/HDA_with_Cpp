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
]

# change this to your own vcgpkg triplet
# more info: vcpkg help triplet
triplet = 'x64-osx'

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

# ------------------------------------------
# Generate Drogon views from CSP templates
# ------------------------------------------
# For every new CSP file you must add a new 
# run_command, so that drogon_ctl can generate
# the corresponding C++ files.
drogon_ctl = find_program('drogon_ctl')
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
message('[ContactNew] ' + r.stdout().strip())

# sources
sources = [main_cpp]
sources += controllers_src
sources += db_src
sources += views_src
sources += dtos_src

# includes
incdir = include_directories(
    [
        'include',
        db_dir,
        models_dir,
        views_dir,
        dtos_dir,
        vcpkg_root / 'packages/drogon_' + triplet + '/include',
        vcpkg_root / 'packages/boost-utility_' + triplet + '/include',
        vcpkg_root / 'packages/boost-config_' + triplet + '/include',
        vcpkg_root / 'packages/boost-io_' + triplet + '/include',
        vcpkg_root / 'packages/boost-throw-exception_' + triplet + '/include',
        vcpkg_root / 'packages/boost-assert_' + triplet + '/include',
        vcpkg_root / 'packages/boost-functional_' + triplet + '/include',
        vcpkg_root / 'packages/boost-container_' + triplet + '/include',
        vcpkg_root / 'packages/trantor_' + triplet + '/include',
        vcpkg_root / 'packages/jsoncpp_' + triplet + '/include',
        vcpkg_root / 'packages/fmt_' + triplet + '/include',
        vcpkg_root / 'packages/sqlite3_' + triplet + '/include',
        vcpkg_root / 'packages/soci_' + triplet + '/include',
    ],
)
# find libraries
cpp = meson.get_compiler('cpp')
libdrogon = cpp.find_library(
    'drogon',
    dirs: [vcpkg_root / 'packages/drogon_' + triplet + '/lib'],
)

libtrantor = cpp.find_library(
    'trantor',
    dirs: [vcpkg_root / 'packages/trantor_' + triplet + '/lib'],
)

libjsoncpp = cpp.find_library(
    'jsoncpp',
    dirs: [vcpkg_root / 'packages/jsoncpp_' + triplet + '/lib'],
)

libopenssl = cpp.find_library(
    'ssl',
    dirs: [vcpkg_root / 'packages/openssl_' + triplet + '/lib'],
)

libcrypto = cpp.find_library(
    'crypto',
    dirs: [vcpkg_root / 'packages/openssl_' + triplet + '/lib'],
)

libbrotli_common = cpp.find_library(
    'brotlicommon-static',
    dirs: [vcpkg_root / 'packages/brotli_' + triplet + '/lib/'],
)

libbrotli_enc = cpp.find_library(
    'brotlienc-static',
    dirs: [vcpkg_root / 'packages/brotli_' + triplet + '/lib/'],
)

libbrotli_dec = cpp.find_library(
    'brotlidec-static',
    dirs: [vcpkg_root / 'packages/brotli_' + triplet + '/lib/'],
)

libcares = cpp.find_library(
    'cares',
    dirs: [vcpkg_root / 'packages/c-ares_' + triplet + '/lib'],
)

libz = cpp.find_library(
    'z',
    dirs: [vcpkg_root / 'packages/zlib_' + triplet + '/lib'],
)

libfmt = cpp.find_library(
    'fmt',
    dirs: [vcpkg_root / 'packages/fmt_' + triplet + '/lib'],
)

libsqlite3 = cpp.find_library(
    'sqlite3',
    dirs: [vcpkg_root / 'packages/sqlite3_' + triplet + '/lib'],
)

libsoci_core = cpp.find_library(
    'soci_core',
    dirs: [vcpkg_root / 'packages/soci_' + triplet + '/lib'],
)

libsoci_sqlite3 = cpp.find_library(
    'soci_sqlite3',
    dirs: [vcpkg_root / 'packages/soci_' + triplet + '/lib'],
)

libresolv = cpp.find_library('resolv')

# copy index.html to build directory
copy = find_program('cp')
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

exe = executable(
    'demo_web_server',
    sources,
    cpp_args: cpp_args,
    install: true,
    include_directories: [incdir],
    dependencies: [
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
        libresolv,
    ],
)

test('basic', exe)
