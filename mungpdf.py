#!/usr/bin/env python2.7
#@(#) mung all JPGs in cwd into one PDF

from reportlab.pdfgen import canvas
from glob import iglob
import Image
import os

c = canvas.Canvas("0.pdf",pagesize=(1024,768))
for i in iglob("*.JPG"):
    t = Image.open(i)
    z = "{0}.2.{1}".format(*tuple(i.split(".")))
    width,height = t.size
    g = t.resize((width/2,height/2))
    # reportlab gives me an error here on Arc's mac, so
    # I save the image in a temporary file & pass that
    # filename to reportlab
    g.save(z)
    c.drawImage(z,0,0,width/2,height/2)
    os.remove(z)    
    c.showPage()
c.save()
