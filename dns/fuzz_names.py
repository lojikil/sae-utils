base = ['www'] # add others here; this started from a client- spcific script
mung = ['dev', 'uat', 'ua', 'stage', 'staging',
        'test', 'testing', 'qa', 'qat', 'internal',
        'int']

for b in base:
    for m in mung:
        print "{0}-{1}".format(b, m)
        print "{1}-{0}".format(b, m)
        print "{0}{1}".format(b, m)
        print "{1}{0}".format(b, m)
