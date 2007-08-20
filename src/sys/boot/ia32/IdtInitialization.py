#! /usr/bin/python

# Initialize IDT

NUM_IRQ = 48 # change if desired

for i in range(0, NUM_IRQ+1):
    print "  SetHardwareVector(" + str(i) + ", intEntry_" + str(i) + ");"

