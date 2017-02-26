import sys

try:
    count = int(sys.argv[1])
except:
    count = 10

with file('base') as fh:
    data = [x.strip() for x in fh.readlines()]

for item in data:
    print item
    for idx in range(0, count):
        print "{0}{1}".format(item, idx)
