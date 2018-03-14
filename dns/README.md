# Overview

a set of DNS-related utilities, mostly for OSINT via living off the land...

- `dig2sqlite.c`: eats the output from `dig` and stores it in a sqlite DB
- `gen_dig.py`: eats a list of host names and generates `dig` commands with a round robin list of DNS servers
- `gen_names.py`: generates a list of host names, basically joining top-level to a 2nd-level domain
- `fuzz_names.py`: generates a list of fuzzed host names, basically turning `www` into `www, www0, www1, www-dev, www-test...`
