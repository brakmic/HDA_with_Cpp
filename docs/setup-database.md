# Setting up the demo database

The server bootstraps the database automatically on startup: it creates the
`contacts` table if it does not exist and imports `contacts.csv` when the table
is empty. If the CSV file is present in the working directory alongside
`server_config.json`, no manual steps are needed.

The steps below are a fallback for troubleshooting or for environments where
the CSV file is not available.

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

The repository includes `contacts.csv` with sample records. The table has an
autoincrement `id` column that the CSV does not provide, so import into a
temporary table first:

```bash
sqlite3 demo.db <<'EOSQL'
CREATE TABLE _csv_tmp ("FirstName" TEXT, "LastName" TEXT, "EMail" TEXT, "Phone" TEXT);
.mode csv
.import contacts.csv _csv_tmp
INSERT INTO contacts(firstname, lastname, email, phone)
  SELECT FirstName, LastName, EMail, Phone FROM _csv_tmp WHERE FirstName != 'FirstName';
DROP TABLE _csv_tmp;
EOSQL
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
