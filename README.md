# Labcontrol

## Building and Installation

The easiest way to build and install _Labcontrol_ is to open the `labcontrol.pro` file with _Qt Creator_ and build the project. Afterwards _Labcontrol_ can be started by running the created executable file. To build correctly _Qt5_ has to be available and must be know to _Qt Creator_.

To work properly _Labcontrol_ requires a file named `labcontrol.json` to be copied from the data directory into the directory of the binary. This also applies to the "icons" directory which can be also found in the data directory. All _z-Tree_ executables should reside in sub-directories of `/opt/z-Leaves` following the naming scheme `zTree_X.Y.ZZ`.

Receipt creation requires at least one receipt template in `/usr/local/share/labcontrol`. The name of the header file should match the pattern `NAMETHEHEADERSHALLHAVE_header.tex` to be recognized.
