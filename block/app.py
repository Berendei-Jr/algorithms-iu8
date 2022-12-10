from collections import deque
import sys

firstLine = input()
if not firstLine:
    sys.exit()
firstLine = firstLine.split()
attemptsNum = int(firstLine[0])
P = int(firstLine[1])
B = int(firstLine[2])
Bmax = int(firstLine[3])
curTime = int(firstLine[4])

array = deque()
for line in sys.stdin:
    if not line:
        continue
    if line == 'exit\n':
        break
    timeStamp = int(line)
    if timeStamp >= curTime - 2*Bmax and timeStamp <= curTime:
        array.append(timeStamp)

array = deque(sorted(array))
size = len(array)

if size < attemptsNum:
    print('ok')
    sys.exit()

blocked = False
blockPeriod = B
unblockTime = None
i = attemptsNum - 1
while True: 
    if i >= size:
        break
    if array[i] - array[i - attemptsNum + 1] <= P:
        if unblockTime:
            if array[i] >= unblockTime + P:
                unblockTime = array[i] + B
                continue
            blockPeriod *= 2
            if blockPeriod > Bmax:
                blockPeriod = Bmax
            unblockTime = array[i] + blockPeriod
        else:
            unblockTime = array[i] + B
        i += attemptsNum  
    else:
        if unblockTime:
            if array[i] >= unblockTime + P:
                unblockTime = None
        i += 1        

print(f'Unblock time: {unblockTime}')
if unblockTime:
    if curTime >= unblockTime:
        print('ok')
    else:
        print(unblockTime)    
else:
    print('ok')    
