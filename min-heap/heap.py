import copy
import sys

class Node:
    def __init__(self, key, value) -> None:
        self.key = key
        self.value = value

class minHeap:
    def __init__(self) -> None:
        self.__data = []
        self.__hashTable = dict()
    def __heapify(self, i):
        left = 2*i
        right = 2*i + 1
        largest = i
        if left <= self.size() and (self.__data[left].key < self.__data[largest].key):
            largest = left
        if right <= self.size() and (self.__data[right].key < self.__data[largest].key):
            largest = right
        if largest != i:
            self.__data[i], self.__data[largest] = self.__data[largest], self.__data[i]
            self.__heapify(self.__data, largest)
    def __sift_up(self, i):
        parent = (i-1)//2
        while i > 0 and self.__data[parent].key > self.__data[i].key:
            self.__hashTable[self.__data[i].key] = parent
            self.__hashTable[self.__data[parent].key] = i
            self.__data[i], self.__data[parent] = self.__data[parent], self.__data[i]
            i = parent
            parent = (i-1)//2
    def __sift_down(self, i):
        n = self.size()
        while 2*i < n:
            l = 2*i+1
            r = 2*i + 2
            if r < n:
                tmp = l
                if self.__data[r].key < self.__data[l].key:
                    tmp = r
                if self.__data[tmp].key > self.__data[i].key:
                    break
                self.__data[tmp], self.__data[i] = self.__data[i], self.__data[tmp]
                self.__hashTable[self.__data[tmp].key] = tmp
                self.__hashTable[self.__data[i].key] = i
                i = tmp  
            else:
                if l >= n:
                    break
                if self.__data[l].key > self.__data[i].key:
                    break
                self.__data[l], self.__data[i] = self.__data[i], self.__data[l]
                self.__hashTable[self.__data[l].key] = l
                self.__hashTable[self.__data[i].key] = i
                break

    def min(self):
        if self.size() == 0:
            raise IndexError()
        return self.__data[0].key, self.__data[0].value         
    def max(self):
        if self.size() == 0:
            raise IndexError()
        i = self.size() - 1
        tmpMax = i
        while i >= self.size()//2:
            if self.__data[i].key > self.__data[tmpMax].key:
                tmpMax = i
            i -= 1    
        return self.__data[tmpMax].key, self.__data[tmpMax].value, tmpMax  
    def add(self, key, value):
        try:
            self.__hashTable[key]
            raise LookupError()
        except KeyError:
            if self.size() == 0:
                self.__data.append(Node(key, value))
                self.__hashTable[key] = 0
                return
            self.__data.append(Node(key, value))
            i = self.size() - 1
            self.__hashTable[key] = i
            self.__sift_up(i)  
    def extract(self):
        if self.size() == 0:
            raise IndexError()
        out = copy.deepcopy(self.__data[0])
        self.__data[0] = self.__data.pop(self.size() - 1)
        self.__hashTable[self.__data[0].key] = 0
        self.__hashTable.pop(out.key, None)
        self.__sift_down(0)
        return out
    def delete(self, key):
        i = self.__hashTable[key]
        if self.size() == 1:
            self.__data.clear()
            self.__hashTable.clear()
            return   
        newKey = self.__data[0].key - 1
        self.__hashTable.pop(key, None)
        self.__hashTable[newKey] = i    
        self.__data[i].key = newKey
        self.__sift_up(i)
        self.extract()
    def search(self, key):
        try:
            i = self.__hashTable[key]
            return self.__data[i].value, i
        except KeyError:
            raise
    def set(self, key, value):
        i = self.__hashTable[key]
        self.__data[self.__hashTable[key]].value = value  
    def size(self):
        return len(self.__data)  
    def print(self, stream):
        if self.size() == 0:
            print('_')
            return
        stream.write(f'[{self.__data[0].key} {self.__data[0].value}]\n')            
        j = 2
        counter = 0
        out = ''
        for i in range(1, self.size()):
            if counter < j - 1:
                counter += 1
                out += f'[{self.__data[i].key} {self.__data[i].value} {self.__data[(i-1)//2].key}] '
                if i == self.size() - 1:
                    out += '_ '*(j-counter-1)
                    out += '_'
                    stream.write(out + '\n')
                continue
            out += f'[{self.__data[i].key} {self.__data[i].value} {self.__data[(i-1)//2].key}]'
            stream.write(out + '\n')
            out = ''
            counter = 0
            j *= 2      

h = minHeap()
for line in sys.stdin:
    if len(line) == 0:
        continue
    if line == 'print\n':
        h.print(sys.stdout)
    elif line == 'min\n':
        try:
            tmp = h.min()
            print(f'{tmp[0]} 0 {tmp[1]}')
        except IndexError:
            print('error')
    elif line == 'max\n':
        try:
            tmp = h.max()
            print(f'{tmp[0]} {tmp[2]} {tmp[1]}')   
        except IndexError:
            print('error')
    elif line == 'extract\n':
        try:
            tmp = h.extract()
            print(f'{tmp.key} {tmp.value}')
        except IndexError:
            print('error')        
    elif line.startswith('add '):
        tmp = line.split()
        if (len(tmp)) == 2:
            if line[len(tmp[0]) + 1 + len(tmp[1]):] == ' \n':
                try:
                    h.add(int(tmp[1]), '')
                except LookupError:
                    print('error')
            else:
                print('error')
                continue
        elif (len(tmp)) != 3:
            print('error')
            continue
        try:
            h.add(int(tmp[1]), tmp[2])
        except LookupError:
            print('error')
            continue
    elif line.startswith('set '):
        tmp = line.split()
        if (len(tmp)) == 2:
            if line[len(tmp[0]) + 1 + len(tmp[1]):] == ' \n':
                try:
                    h.set(int(tmp[1]), '')
                except KeyError:
                    print('error')
            else:
                print('error')
        elif (len(tmp)) != 3:
            print('error')
            continue
        try:
            h.set(int(tmp[1]), tmp[2])
        except KeyError:
            print('error') 
    elif line.startswith('search '):
        tmp = line.split()
        if (len(tmp)) != 2 or line[-1] == ' ':
            print('error')
            continue
        try:
            tmp = h.search(int(tmp[1]))
            print(f'1 {tmp[1]} {tmp[0]}')
        except KeyError:
            print(0)     
    elif line.startswith('delete '):
        tmp = line.split()
        if (len(tmp)) != 2 or line[-1] == ' ':
            print('error')
            continue
        try:
            h.delete(int(tmp[1]))
        except KeyError:
            print('error')    
    else:
        print('error')      
