## A hypermedia-driven app based on htmx and Drogon C++ web framework

![htmx_with_cpp](videos/htmx_with_cpp.gif)

This repository contains an [HDA](https://htmx.org/essays/hypermedia-driven-applications/) based on [htmx](https://htmx.org/) (frontend) and [Drogon C++ framework](https://drogon.org/) (backend). 

The aim was to create a responsive "web app" without using any of the usual JavaScript frameworks.

The idea for this project came while reading the excellent book [Hypermedia Systems](https://hypermedia.systems/). In it, the authors talk about alternative ways for writing `modern` web applications. Unlike most of the other books on web development, the authors don't rely on any JavaScript framework, but instead go back to the roots of the hypermedia architecture that is `the web` itself.

Instead of using JavaScript *to overcome* HTML, a strategy that basically reproduces thick-clients of the 90es, the authors use `htmx` **to augment** it. They make it capable of doing *more* without falling back to clever JavaScript tricks. Of course, JS isn't forbidden and `htmx` itself relies on it for its own development, but JS is not visible as there is no actual need for it.

Instead of using JS to replace seemingly "insufficient" hypermedia controls, **htmx** extends them. It makes them capable of doing 
*more* as originally defined. An anchor tag (`<a>`), for example, can be "upgraded" so that it can execute POST, PUT, PATCH, or even DELETE requests. A `<form>` tag doesn't have to be the only hypermedia control for sending data via POST requests. How about writing your own controls that can do exactly the same? Or maybe `<form>`s that can PATCH existing entries on the server? What usually demands explicit JS code can now be done *declaratively* with *upgraded* hypermedia controls.

Here's an example from this project. Two buttons (*Cancel* & *Save*) which can be found in almost every sufficiently complex web app.

```html
<button hx-get="/contacts"
        hx-target="#main"
        hx-swap="innerHTML">
        Cancel
</button>
<button hx-post="/contacts/{%contact.ID%}/edit"
        hx-include="input"
        hx-target="#main"
        hx-swap="innerHTML">
        Save
</button>
```

Believe it or not, but these two utilize the following functionalities:

* Executing AJAX requests.
* Using HTTP verbs that are usually not available for `<button>` controls.
* Passing of additional element values in AJAX requests.
* Transclusion (that is, *where* and *how* to insert the server response data)

And not a single line of JavaScript was needed to make it work. This is how powerful hypermedia architecture actually is.

-----

Instead of sending JSONs back and forth (*and each time parsing them according to some internal logic*), we can use HTML as originally designed: as a vehicle for meaningful hypermedia applications. The HTTP protocol exists because of HTML, but these days we mostly transfer JSON over it. This actually makes little sense, because JSON can't transport application semantics, which effectively cripples the original meaning of the *client-server archiecture* of the web. No wonder we need massive JS frameworks on our frontends, because our servers are mostly just data providers with JSON APIs. And JSON APIs aren't "RESTful".

-----

#### A few words regarding the usage of C++ for the backend.

The book's example backend source code is written in Python and it can be used instead of C++. In fact, I have tried to mimic the original Python APIs, so that there should be no big gaps in understanding them both. I was writing the C++ code while reading the respective chapters.

But as `htmx` is very language agnostic, there is no problem of using any language whatsoever, so I used C++. This is also good from the learning perspective as it forces me to double check everything.

And just like we should remove bloat from our frontends [*put any massive JS framework in here*], the same should be applied on our backends [*put any massive backend framework in here*] as well. Massive software consumes massive amounts of time and energy. **Human** time and energy, but also **CPU** cycles and electricity.

## Setup

### C++ libraries
  
A few C++ libraries are needed for the compilation to succeed. This project uses [vcpkg](https://vcpkg.io/en/index.html) as its package manager, but you are free to choose any other instead.

To install a package, simply invoke `vcpkg install PACKAGE_NAME`.

The following packages are needed:

    drogon
    trantor
    jsoncpp
    openssl
    brotli
    cares
    zlib
    fmt
    sqlite3
    soci[core]
    soci[sqlite3]

The search for them is easy: `vcpkg search PACKAGE_NAME`

### C++ build system

My build system of choice is [Meson](https://mesonbuild.com/), because `Makefiles` are hard to maintain and I simply don't want to learn how to use `CMake`. Life is too short for user-hostile software.

There is a small bash script, `buildall.sh`, available that:

* Configures Meson build environment
* Converts Drogon's CSP files into C++ source files and puts them into the `views` directory
* Compiles the application and puts the binary into `builddir`

A C++20 compiler is needed. I'm using GNU C++ v12.1.0.

Before trying to build the project, please, adapt these two variables in the `meson.build` file:

* triplet
* vcpkg_root

The `triplet` carries the information about the host machine, e.g. `x64-osx`.

The `vcpkg_root` is the root folder containing packages installed by `vcpkg`.

![compile_project](videos/compile_project.gif)

## Application architecture

The frontend uses the **htmx** library and some `Bootstrap` resources for styling. There is no hand-written JavaScript running as **htmx** already provides the `responsive` stuff we expect any `modern` web app to offer.

The backend is based on the very fast C++ web framework called `Drogon`.

The database in use is SQLite3 but it can be replaced easily with any other SQL database. Simply adjust the `src/database/db_mgr.cpp` class. The library for accessing SQLite3 is [SOCI](https://soci.sourceforge.net/doc/release/4.0/) and it supports many other database backends. The root of this project contains a SQLite3 file, `demo.db`, that the app uses by default. There is also a CSV file available, `contacts.csv`, that contains a few entries that can be used to populate a new table.

## Project structure

![project_structure](images/project_structure.png)

* `controllers` contains classes that Drogon uses to map client calls to functions in the backend.
* `database` contains a small wrapper class for accessing the SQLite3 instance.
* `dtos` contains `Data Transfer Objects` that are used for data tansfers between frontend and backend.
* `templates` contains [CSPs](https://github.com/drogonframework/drogon-docs/blob/master/ENG-06-View.md) (C++ Server Pages), which are templates that `drogon_ctl` uses to generate C++ sources. These sources will be used to create HTML outputs.
* `views` contains Drogon-generated C++ classes. These files **should not be edited manually**. They will be replaced on every build. To change their behavior or contents, use CSPs from `templates` folder instead.

## HTML / Web App

The web application starts by loading the `index.html` which contains a `div` tag with *id="main"*. Throughout the app, this tag will be used by other controls to dynamically replace its contents without any page refreshes. However, unlike other typical `modern` web apps, we use no JS frameworks like React or Angular to achieve make the app responsive. Instead, we load `htmx` which is the only required scripting library. There are also three `bootstrap` resources involved, but this is just make the app look better. Bootstrap is not a requirement and can be replaced by any other library or own stylesheets.