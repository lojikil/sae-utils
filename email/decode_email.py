import email
import sys

if len(sys.argv) != 2:
    print "usage: decode_email [file name]"
    sys.exit(0)

with file(sys.argv[1]) as fh:
    msg = email.message_from_file(fh)

for part in msg.walk():
    if part.get_content_maintype() == 'multipart':
        continue
    filename = part.get_filename()
    if not filename:
        filename="partone"
    with open(filename, "wb") as fh:
        fh.write(part.get_payload(decode=True))
