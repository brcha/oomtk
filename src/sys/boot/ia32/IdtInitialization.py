#! /usr/bin/python

# Initialize IDT

NUM_IRQ = 255 # change if desired

for i in range(0, NUM_IRQ+1):
    print "  IdtTable[" + str(i) + "].bits.present = 0;"
    print "  SetHardwareVector(" + str(i) + ", intEntry_" + str(i) + ");"

