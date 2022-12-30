## A hypermedia-driven app based on htmx and Drogon C++ web framework

![htmx_with_cpp](videos/htmx_with_cpp.gif)

This repository contains an [HDA](https://htmx.org/essays/hypermedia-driven-applications/) based on [htmx](https://htmx.org/) (frontend) and [Drogon C++ framework](https://drogon.org/) (backend). 

The aim was to create a responsive "web app" without using any of the usual JavaScript frameworks.

The idea for this project came while reading the excellent book [Hypermedia Systems](https://hypermedia.systems/). In it, the authors talk about alternative ways for writing `modern` web applications. Unlike most of the other books on web development, the authors don't rely on any JavaScript framework, but instead go back to the roots of the hypermedia architecture that is `the web` itself.
### htmx

Instead of using JavaScript *to overcome* HTML, a strategy that basically reproduces thick-clients of the 90es, the authors use `htmx` **to augment** it. They make it capable of doing *more* without falling back to clever JavaScript tricks. Of course, JS isn't forbidden and `htmx` itself relies on it for its own development, but JS is not visible as there is no actual need for it.

We don't need to use JS to replace seemingly "insufficient" hypermedia controls, because **htmx** is here to extend them. It makes them capable of doing 
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

### Hyperscript

We also use [_hyperscript](https://hyperscript.org/), a small library for event handling and DOM manipulation. With it, we can listen to and dispatch events, manipulate DOM objects, all without leaving HTML.

Here's an example from this project:

```html
<button id="edit-c" class="btn btn-primary"
      hx-get="/contacts/{%c.ID%}/edit"
      hx-target="#main"
      hx-swap="innerHTML">Edit</button>
<button class="btn btn-danger"
      hx-delete="/contacts/{%c.ID%}/delete"
      hx-confirm="Are you sure you wish to delete this contact?"
      hx-target="this"
      hx-swap="none"
      _="on click remove #edit-c
                  then remove me"
      >Delete</button>
<button class="btn btn-info"
      hx-get="/contacts"
      hx-target="#main"
      hx-swap="innerHTML">Back</button>
```

In the second `<button>` control we have a few bits of _hyperscript that does the following:

* reacts to click events
* then removes the control with *id=edit-c*
* then removes the button that reacted to click event (it removes itself)

The final result is the removal of the buttons `Edit` and `Delete`. Only the button `Back` remains.

![using_hyperscript](videos/using_hyperscript.gif)

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
    drogon[ctl]
    sqlite3
    soci[core]
    soci[sqlite3]

The search for them is easy: `vcpkg search PACKAGE_NAME`

Windows users will have to use the `recurse` flag for some of the packages. Fo example: `vcpkg install soci[sqlite3] --recurse`

Windows users will need to manually add the path to `drogon_ctl` binary. `vcpkg` installs it under its default package location, e.g. `C:\SOME_ROOT_PATH\vcpkg\packages\drogon_x64-windows\tools\drogon`

However, I haven't been able to compile it under Windows as the [MinGW](https://github.com/microsoft/vcpkg/blob/master/docs/users/mingw.md) compiler breaks during the Drogon compilation.

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

`drogon_ctl` will be used by `Meson` to convert CSP templates into C++ files.

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

The web application starts by loading the `index.html` which contains a `div` tag with *id="main"*. Throughout the app, this tag will be used by other controls to dynamically replace its contents without any page refreshes. However, unlike other typical `modern` web apps, we use no JS frameworks like React or Angular to make the app responsive. Instead, we only use `htmx` as our scripting library.

There are also three `bootstrap` resources involved, but this is just make the app look better. Bootstrap is not a requirement and can be replaced by any other library or own stylesheets. The same applies to `jQuery` that is included as a bootstrap dependency. Any of those libraries can be safely removed as they don't affect `htmx` or `_hyperscript`.

The web app communicates with the server in a standard request-response fashion. But unlike so many other web apps out there, no JSON is being used. Instead, the server is only sending pieces of HTML code that the client uses to update the current state of the app.

## CHANGELOG
* 30/12/2022:    
    -  added _hyperscript scripts in index.html
    -  added jQuery in index.html (bootstrap needs it)
    -  changed button behavior when deleting contacts
    -  added example with _hyperscript
    -  added gif demo showcasing _hyperscript

## LICENSE

[MIT](LICENSE)