import struct
import csv

# Note: If you want to change the number od data points in a row when decoding you
# should change the below variable named 
# number of data points in a row numOfDataPoints
noOfDataPoints = 6

# variables needed for file handling
file = 0

# taking the input file name 
while(1):
    print("Enter the file name (ex: 'DATA2.BIN'): ", end = '')
    fileName = input()
    try:
        # path and the file name to be read
        file = open(fileName,"rb") 
        print("File Successfully opened")
        break
    except:
        print("File not found please enter a valid file name")
        print()

# reading four bytes at the begining
byte = file.read(1)

# usefull tmp values
count = 0
res = []
finalData = []

# opening the csv file to dump data values
with open(fileName[:len(fileName)-4] + '_DUMP.csv', 'w', newline='') as writeFile:
    writer = csv.writer(writeFile)
    # reading four byte by four byte until the eof 
    while byte:
        res.append(byte)
        if (byte== b'a'):
            count += 1
        else:
            count = 0

        if (count == 4):

            if (len(res) == noOfDataPoints*4+5):
                #grabing the time stamp
                timeStamp = int.from_bytes(res[len(res)-5],byteorder='big')
                finalData.append(timeStamp)
                print(timeStamp, end=',')

                for section in range (noOfDataPoints):
                    sectionArr = res[section*4:section*4+4]
                    #print(sectionArr)                
                    for x in range (len(sectionArr)):
                        sectionArr[x] =  int.from_bytes(sectionArr[x],byteorder='big')

                    #print(sectionArr)
                    aa = bytearray(sectionArr)
                    data = round(float(struct.unpack('>f', bytearray(sectionArr))[0]), 2)
                    finalData.append(data)
                    print(data, end=',')  #little endian

                print()
                # writting to the csv file
                writer.writerow(finalData)
                # resetting the array
                finalData = []
            else:
                pass
            res = []
            count = 0

        # reading the new byte
        byte = file.read(1)


    # close the file after read 
    file.close()