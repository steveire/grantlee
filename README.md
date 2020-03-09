
The Grantlee Libraries
======================

[![OSX/Linux Build Status](https://travis-ci.org/steveire/grantlee.svg?branch=master)](https://travis-ci.org/steveire/grantlee)
[![Windows Build status](https://ci.appveyor.com/api/projects/status/github/steveire/grantlee?branch=master&svg=true)](https://ci.appveyor.com/project/steveire/grantlee/branch/master)

* About Grantlee
* Installation
* Licensing
* Contributing

About Grantlee
--------------

Grantlee is a set of Free Software libraries written using the [Qt framework](http://code.qt.io).
Currently two libraries are shipped with Grantlee: Grantlee Templates and Grantlee
TextDocument.

The goal of Grantlee Templates is to make it easier for application developers to
separate the structure of documents from the data they contain, opening the door
for theming and advanced generation of other text such as code.

The syntax uses the syntax of the [Django template system](https://docs.djangoproject.com/en/1.9/ref/templates/language/), and
the core design of Django is reused in Grantlee.

    <ul>
    {% for athlete in athlete_list %}
    <li>{{ athlete.name }}{% if athlete.isCaptain %} (C){% endif %}</li>
    {% endfor %}
    </ul>

Part of the design of both template systems is that application developers can
extend the syntax by implementing their own tags and filters. For details of how
to do that, see the [API documentation](http://www.grantlee.org/apidox/extension.html).

Installation
------------

To build Grantlee, you need at least Qt5.3 (with development packages) and CMake 3.1.
Out-of-source builds are recommended:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=$prefix ..
    cmake --build .
    cmake --build . --target install

Licensing
---------

Grantlee is covered by the GNU Lesser General Public License Version 2.1, or
at your option, any later version. All contributions to Grantlee must be
covered by the same license.

The details of the license are in the COPYING file in the source distribution.

Contributing
------------

Grantlee is hosted on github, and welcomes pull requests and issues.

