import sys

if __name__ == "__main__":

    inputStream = open(sys.argv[1], 'r')
    outputStream = open(sys.argv[2], 'w')

    sum = 0

    for line in inputStream:
        try:
            sum += (int(line) % 256)
        except ValueError:
            continue
        
    outputStream.write(str(sum % 256))

    inputStream.close()
    outputStream.close()
