import sys

servers = [ '208.67.222.222', '8.8.8.8', '209.244.0.3', '216.146.35.35', '74.82.42.42']

if len(sys.argv) == 2:
    domfile = sys.argv[1]
    prefile = 'prefixes'
elif len(sys.argv) == 3:
    domfile = sys.argv[1]
    prefile = sys.argv[2]
else:
    domfile = 'domains.final'
    prefile = 'prefixes'

with file(domfile) as fh:
    domains = [x.strip() for x in fh.readlines()]

with file(prefile) as fh:
    prefixes = [x.strip() for x in fh.readlines()]

idx = 0

for domain in domains:
    print "echo ", domain
    print "echo '; BEGIN {0}' >> dnsreport".format(domain)
    print "dig @{0} {1} >> dnsreport".format(servers[idx], domain)
    print "echo '; END {0}' >> dnsreport".format(domain)
    for prefix in prefixes:
        name = "{0}.{1}".format(prefix, domain)
        print "echo ", name
        print "echo '; BEGIN {0}' >> dnsreport".format(name)
        print "dig @{0} {1} >> dnsreport".format(servers[idx], name)
        print "echo '; END {0}' >> dnsreport".format(name)

        idx += 1

        if idx >= len(servers):
            print "sleep 10"
            idx = 0
