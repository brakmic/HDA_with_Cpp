# Setting up the demo database

The SQLite database `demo.db` is not tracked in version control. Each developer or deployment target must create it locally.

## Prerequisites

The `sqlite3` command-line tool. Download it from [sqlite.org](https://www.sqlite.org/download.html) if it is not already installed.

## Create the database

From the project root:

```bash
sqlite3 demo.db "CREATE TABLE contacts (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  firstname TEXT,
  lastname TEXT,
  email TEXT,
  phone TEXT
);"
```

## Import sample data

The repository includes `contacts.csv` with sample records. Import it:

```bash
sqlite3 demo.db ".mode csv" ".import contacts.csv contacts"
```

The first row of the CSV is the header. Remove it from the table:

```bash
sqlite3 demo.db "DELETE FROM contacts WHERE rowid = 1;"
```

## Verify

```bash
sqlite3 demo.db ".tables"
sqlite3 demo.db "SELECT COUNT(*) FROM contacts;"
```

The output should show the `contacts` table with a non-zero row count.

## Where the database is expected

The server reads the database path from `server_config.json`:

```json
{
  "database": {
    "type": "sqlite3",
    "file": "demo.db"
  }
}
```

This means `demo.db` must be in the working directory when the server starts. When running from the Meson build directory, copy both `server_config.json` and `demo.db` into `builddir/`. The Meson build script does this automatically for `server_config.json` but not for `demo.db`.
