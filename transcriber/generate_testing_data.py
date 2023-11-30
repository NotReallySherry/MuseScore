import random
import sys

# python3 generate_testing_data.py <output file> <test size> <tempo> <key range>

outfile = open(sys.argv[1], 'w')
test_size = int(sys.argv[2])
tempo = int(sys.argv[3])
keyrange = int(sys.argv[4])

pc = 0

# output string format: pc, tempo, note1:power1, note2:power2, note3:power3 ...
for i in range(0, test_size):
    outstr = str(pc) + ',' + str(tempo)
    
    for note in range(0, keyrange):
        power = random.randint(0, 3) # 0 not presssed, 1-3 pressed
        outstr += ',' + str(note) + ':' + str(power)
        
    outstr += '\n'
    pc += 1
    
    # write to output file
    outfile.write(outstr)