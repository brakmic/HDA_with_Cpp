# Docker

The application can be built and run in a Docker container.

## Build the Image

```bash
docker build -t hda-with-cpp .
```

The build uses a multi-stage Dockerfile:

- Stage 1 (builder): Ubuntu 24.04, installs build tools, compiles Drogon from source,
  builds the application, and runs the test suite
- Stage 2 (runtime): minimal Ubuntu 24.04, copies the binary and static assets only

## Run the Container

```bash
docker run -p 3000:3000 hda-with-cpp
```

Open <http://127.0.0.1:3000> in a browser.

## Persist the Database

The SQLite database is stored inside the container by default and is lost when the
container is removed. To persist contacts between restarts, mount a volume:

```bash
# First run — creates an empty database in the volume
docker run -p 3000:3000 -v hda-data:/app hda-with-cpp

# On subsequent runs, the existing database is preserved
docker run -p 3000:3000 -v hda-data:/app hda-with-cpp
```

The database file is `demo.db` in the application directory (`/app`).

## Import Sample Data

The repository includes a `contacts.csv` file. After starting the container, you can
import it manually through the web interface at <http://127.0.0.1:3000>.

## Development vs Production

This Dockerfile is suitable for development and demonstration. For production use:

- Add a reverse proxy (nginx, Caddy) for TLS termination
- Configure Drogon to listen on a Unix socket instead of TCP
- Use a managed database (PostgreSQL) instead of SQLite
- Set appropriate resource limits (`--memory`, `--cpus`)
- Enable Drogon's production logging settings

## Image Contents

The runtime image contains only:

- The `demo_web_server` binary
- Drogon and Trantor shared libraries
- Static assets (htmx, _hyperscript, Bootstrap 5, Font Awesome)
- Configuration files
- Runtime dependencies (SQLite, OpenSSL, Brotli, etc.)

No build tools, source code, documentation, or CI configuration are included.
