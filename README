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

    android update project -p . -n agram
    ndk-build
    ant debug

This will produce bin/agram-debug.apk.

Building POSIX Binaries
-----------------------
A command-line version of agram for POSIX-compliant systems can be built:

    make -C jni

This will produce jni/agram. This version also requires a word list at run-time.
A suitable word list is included as assets/words, but must be installed.
You may also already have a usable word list on your system.
