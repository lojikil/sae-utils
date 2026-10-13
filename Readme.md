# sae&apos;s Utils

- xf.c: simple hex filter
- ipcalc.c: cidr calculator
- hex2ip.c: convert hex address into dotted quads
- ip2hex.c: convert dotted quads into a hex address
- scs.c: simple connect scanner
- seq.c: a quick &amp; dirty implementation of seq(1) for systems that don&apos;t have it
- tt.c: set the terminal title via VT control codes
- uue.c: quick &amp; dirty implementation of UUEncode
- xteaofb.c: use XTea in Output Feedback mode
- rc4.c: quick & dirty implementation of the RC4 algorithm
- dumptar.c: dump metadata from a tar file to standard out
- gettar.c: fetches file contents from a tarfile, in a ISO-C standard way (no POSIX)
- lstar.c: lists file contents from a tarfile, in a ISO-C standard way (no POSIX)
- nuke.c: gutmann&apos;s secure removal algorithm
- mungpdf.py: using ReportLab Toolkit & PIL, mung all JPGs in cwd into one PDF
- tdc.c: tiny dc clone I wrote *forever* ago
- ms.c: microscopic version of the standard `strings` program.
- czu.c: a tiny LBR[0] file format dumper

# License

All code is licensed under the zlib/png license.

Normally I use ISC licenses, but I wrote this a long time ago, and so I'm sticking with the same license here.

# Building

Most of these utilities should be completely standalone, and thus a simple `make` command should
suffice for any of them. Just use `make [program name]` to compile, e.g. `make tdc` will build
`tdc.c` into a binary named `tdc`.

# Footnotes

- [0]: https://en.wikipedia.org/wiki/LBR_%28file_format%29
