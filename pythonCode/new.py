import struct
import csv

# Note: If you want to change the number od data points in a row when decoding you
# should change the below variable named 
# number of data points in a row numOfDataPoints
noOfDataPoints = 6

# path and the file name to be read
file = open("DAT1_NEW.BIN","rb") 

# reading four bytes at the begining
byte = file.read(1)

count = 0
res = []

# reading four byte by four byte until the eof 
while byte:
    res.append(byte)
    if (byte== b'a'):
        count += 1
    else:
        count = 0

    if (count == 4):

        if (len(res) == noOfDataPoints*4+4):
            for section in range (noOfDataPoints):
                sectionArr = res[section*4:section*4+4]
                #print(sectionArr)                
                for x in range (len(sectionArr)):
                    sectionArr[x] =  int.from_bytes(sectionArr[x],byteorder='big')

                #print(sectionArr)
                aa = bytearray(sectionArr)
                print(round(float(struct.unpack('>f', bytearray(sectionArr))[0]), 4), end=',')  #little endian
            print()
        else:
            print(res)
        res = []
        count = 0

    # reading the new byte
    byte = file.read(1)


# close the file after read 
file.close()