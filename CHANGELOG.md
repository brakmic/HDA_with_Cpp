# CHANGELOG

- 27/05/2026 — Modernization
  - fix SQL injection vulnerability: replace string-concatenated queries with SOCI parameterized bindings and column whitelist
  - add input validation: enforce max lengths (name 100, email 320, phone 30) and email format in ContactDto
  - check save_contact and update_contact return values in contacts controller, return 500 on failure
  - redirect home controller (/) to /contacts instead of returning JSON
  - migrate from Bootstrap 3.4.1 to Bootstrap 5.3.5, Font Awesome 6.2.1 to 6.7.2, remove jQuery
  - modernize build system: Meson with dependency() lookups, vcpkg manifest, compiler-agnostic flags
  - add cross-platform meson.build with CMake target fallbacks to pkg-config for Linux compatibility
  - add Docker support: multi-stage build (Ubuntu 24.04), Drogon built from CMake source, runtime image ~145MB
  - add test suite: 29 tests (13 DTO + 9 DB manager + 6 controller + 1 demo) run inside Docker on every build
  - add Criterion test framework integration via Docker builder stage
  - update CI workflow (.github/workflows/build-test.yml) for Docker-based build and test
  - add DOCKER.md with build, run, volume persistence, and production guidance
  - add docker/ directory with pkg-config files for Drogon, Trantor, and SOCI
  - fix listener output: print address once inline in main() instead of per-thread registerBeginningAdvice
  - fix landing page: complete Bootstrap 5 migration in index.html (jumbotron, panel, offset classes)
  - fix Font Awesome: download full package with webfonts, remove CDN dependency
  - fix empty contacts page: H2 heading always visible, empty-state fallback with Reload button
  - add CSV auto-reseed: DbManager::import_from_csv re-populates database when last contact is deleted

- 29/05/2025
  - update to HTMX v2.0.4 & _hyperscript v0.9.14
  - add libuuid to deps in meson.build
  - fix brotli library installation under linux
  - improve linux compilation in meson.build
  - set default triplet in meson.build to x64-linux
  - fix Criterion installation under linux
  - update README

- 02/01/2023:
  - added Criterion test library
  - added server configuration facility
  - added server configuration JSON
  - updated README regarding testing

- 01/01/2023:
  - added TOC to README
  - added FontAwesome
  - added style.css
  - updated index.html
  - updated buildall.ps1
  - updated meson.build
  - included Drogon's config.json
  - use Drogon's AOP to display active Listeners

- 31/12/2022:
  - Windows compilation support

- 30/12/2022:
  - added _hyperscript scripts in index.html
  - added jQuery in index.html (bootstrap needs it)
  - changed button behavior when deleting contacts
  - added example with _hyperscript
  - added gif demo showcasing _hyperscript
