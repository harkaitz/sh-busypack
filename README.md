BUSYPACK
========

Create standalone windows 64bit executables from shell scripts
written with [Busybox for Windows](http://frippery.org/busybox/).

The files are extracted in %LOCALAPPDATA%\BusyPack by default.

## Help

busypack

    Usage: busypack [-V] SCRIPT
    
    Pack a shell script into a self extracting binary. If found
    it will use "hsh" to bundle the script.
    
    If a script containes '#busypack: F1 F2' then F1 and F2 will be
    searched for in "~/.busypack" and added to the binary.

## Collaborating

For making bug reports, feature requests and donations visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)
