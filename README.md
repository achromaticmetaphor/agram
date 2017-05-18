agram is an Android application which displays anagrams.


Instructions for Installation:
==============================

Pre-built Android Binaries
--------------------------
Pre-built binaries are likely available from
[F-Droid](https://f-droid.org/repository/browse/?fdid=us.achromaticmetaphor.agram)
and may be available from other application repositories.

Building Android Binaries
-------------------------
To build agram from source using an Android SDK and NDK, execute:

    ./gradlew build

This will produce build/outputs/apk/agram-debug.apk.

Building POSIX Binaries
-----------------------
A command-line version of agram for POSIX-compliant systems can be built:

    cmake cli
    make

This will produce agram. This version also requires a word list at run-time.
A suitable word list is included as src/main/assets/words, but must be installed.
You may also already have a usable word list on your system.
