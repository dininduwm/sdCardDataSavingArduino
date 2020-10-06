import struct
import csv

# Note: If you want to change the number od data points in a row when decoding you
# should change the below variable named 
# number of data points in a row numOfDataPoints
numOfDataPoints = 4

# path and the file name to be read
file = open("DAT1_NEW.BIN","rb") 

# reading four bytes at the begining
byte = file.read(4)

count = 1
res = []

# opening the csv file to dump data values
with open('dumpData.csv', 'w', newline='') as writeFile:
    writer = csv.writer(writeFile)
    # reading four byte by four byte until the eof 
    while byte:
        # appending new value to the res array        
        res.append(round(float(struct.unpack('>f', byte)[0]), 4))   #little endian

        if count == 4:
            writer.writerow(res)
            # resetting the row
            res = []
            count = 1
        else:            
            count +=1

        # reading the new byte
        byte = file.read(4)


# close the file after read 
file.close()