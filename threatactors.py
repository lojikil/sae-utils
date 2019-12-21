import random
import sys


rawl = """
China
India
Pakistan
Cyber-jihadi Activists
Non-Nation State Actors
Your Mom
My Mom
Israel
The French
Unit 8200
ex-Unit 8200
Black Cube
Anonymoose
The Hacker Known as Four Chan
The Hacker formerly-known as Four Chan
Russia
Ukraine
Russia Dressed Up In Fake Mustaches as Ukrainians
Actually a Small Child
Teenagers! (how do you do fellow kids?)
Three Words: Oh Gee Aaaaaaaaaaaaay
NorK
Bitcoin Maximalists
Buttcoin Minimalists"""
actors = filter(lambda x: x != "", rawl.split("\n"))

if len(sys.argv) > 1 and sys.argv[1] == "-r":
    print "It was {0}".format(random.choice(actors))
else:
    for actor in actors:
        print "It was {0}".format(actor)
