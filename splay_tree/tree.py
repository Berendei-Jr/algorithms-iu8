import sys
sys.setrecursionlimit(2050)

class Node:
    def __init__(self, father, value, key, isLeftChild = True):
        self.father = father
        self.value = value
        self.key = key
        self.isLeftChild = isLeftChild
        self.rightChild = None
        self.leftChild = None
    def hasLeftChild(self):
        return not (self.leftChild is None)
    def hasRightChild(self):
        return not (self.rightChild is None)         
    def isRoot(self):
        return self.father == None
    def changeIsLeft(self):
        self.isLeftChild = not self.isLeftChild    

class splayTree:
    def __init__(self):
        self.root = None
    def __zig(self, node, left = True):
        father = node.father
        if not father.isRoot():
            grandFather = father.father
            node.father = grandFather
            if father.isLeftChild:
                grandFather.leftChild = node
            else:
                grandFather.rightChild = node
        else:
            node.father = None
        if left:
            if node.hasLeftChild():
                father.rightChild = node.leftChild
                father.rightChild.changeIsLeft()
                father.rightChild.father = father
            else:
                father.rightChild = None
            if not father.isLeftChild:
                father.changeIsLeft()
            else:
                node.changeIsLeft()
            node.leftChild = father
        else:
            if node.hasRightChild():
                father.leftChild = node.rightChild
                father.leftChild.changeIsLeft()
                father.leftChild.father = father
            else:
                father.leftChild = None
            if father.isLeftChild:
                father.changeIsLeft()
            else:
                node.changeIsLeft()
            node.rightChild = father
        father.father = node
    def __zig_zig(self, node, left = True):
        self.__zig(node.father, not left)
        self.__zig(node, not left)
    def __zig_zag(self, node, left = True):
        self.__zig(node, left)
        self.__zig(node, not left)
    def __splay(self, node, depth):
        if depth == 2:
            if node.isLeftChild:
                self.__zig(node, False)
            else:
                self.__zig(node) 
            self.root = node               
        elif depth == 1:
            self.root = node
        else:
            if node.isLeftChild:
                if node.father.isLeftChild:
                    self.__zig_zig(node)
                else:
                    self.__zig_zag(node, False)
                self.__splay(node, depth - 2)
            else:
                if node.father.isLeftChild:
                    self.__zig_zag(node)
                else:
                    self.__zig_zig(node, False)
                self.__splay(node, depth - 2)        
    def __insert(self, cur_node, depth, key, value):
        if cur_node.key == key:
            print("error")
            return cur_node, depth
        if int(key) < int(cur_node.key):
            if cur_node.hasLeftChild():
                return self.__insert(cur_node.leftChild, depth + 1, key, value)
            else:
                new_node = Node(cur_node, value, key)
                cur_node.leftChild = new_node
                new_node.father = cur_node
                return new_node, depth + 1
        else:
            if cur_node.hasRightChild():
                return self.__insert(cur_node.rightChild, depth + 1, key, value)
            else:
                new_node = Node(cur_node, value, key, False)
                cur_node.rightChild = new_node
                new_node.father = cur_node
                return new_node, depth + 1                    
    def __find_min(self, cur_node, depth):
        if not cur_node.hasLeftChild():
            return cur_node, depth
        else:
            return self.__find_min(cur_node.leftChild, depth + 1)
    def __find_max(self, cur_node, depth):
        if not cur_node.hasRightChild():
            return cur_node, depth
        else:
            return self.__find_max(cur_node.rightChild, depth + 1)        
    def __find(self, cur_node, depth, key, printEnabled = True):
        while True:
            if cur_node.key == key:
                return cur_node, depth, True
            elif (int(cur_node.key) > int(key)) and cur_node.hasLeftChild():
                cur_node = cur_node.leftChild
                depth += 1
                continue
            elif (int(cur_node.key) < int(key)) and cur_node.hasRightChild():
                cur_node = cur_node.rightChild
                depth += 1
                continue
            else:
                return cur_node, depth, False     
    def __merge(self, left, right):
        if left is not None:
            left.father = None
            if right is None:
                return left
            left_max = self.__find_max(left, 1)
            self.__splay(left_max[0], left_max[1])
            if not left_max[0].isLeftChild:
                left_max[0].changeIsLeft()
            left_max[0].rightChild = right
            right.father = left_max[0]
            return left_max[0]
        else:
            if right is not None:
                right.father = None
                right.changeIsLeft()
                return right
            else:
                return None                
    def print(self, stream):
        if self.root is None:
            stream.write('_\n')
            return    
        stream.write(f'[{self.root.key} {self.root.value}]\n')
        level = []
        if self.root.hasLeftChild():
            level.append(self.root.leftChild)
        else:
            level.append(1)
        if self.root.hasRightChild():
            level.append(self.root.rightChild)
        else:
            if (level[-1] == 1):
                return
            else:
                level.append(1)   
       
        level_len = len(level)
        mayContinue = True
        while mayContinue:
            out = ""
            mayContinue = False
            newLevel = []
            for i in range(level_len - 1):
                if type(level[i]) == int:
                    out += '_ '*level[i]
                    if len(newLevel) == 0 or type(newLevel[-1]) != int:
                        newLevel.append(2*level[i])
                    else:  
                        newLevel[-1] += 2*level[i]
                else:
                    out += f'[{level[i].key} {level[i].value} {level[i].father.key}] '
                    if level[i].hasLeftChild():
                        newLevel.append(level[i].leftChild)
                        mayContinue = True
                    else:
                        if len(newLevel) == 0:
                            newLevel.append(1)    
                        elif type(newLevel[-1]) == int:
                            newLevel[-1] += 1
                        else:
                            newLevel.append(1)    
                    if level[i].hasRightChild():
                        newLevel.append(level[i].rightChild)
                        mayContinue = True
                    else:
                        if len(newLevel) == 0:
                            newLevel.append(1) 
                        elif type(newLevel[-1]) == int:
                            newLevel[-1] += 1
                        else:
                            newLevel.append(1)            
            i = level[-1]
            if type(i) == int:
                    out += '_ '*(i-1)
                    out += '_\n'
                    if len(newLevel) == 0 or type(newLevel[-1]) != int:
                        newLevel.append(2*i)
                    else:  
                        newLevel[-1] += 2*i        
            else:
                    out += f'[{i.key} {i.value} {i.father.key}]\n'
                    if i.hasLeftChild():
                        mayContinue = True
                        newLevel.append(i.leftChild)
                    else:
                        if len(newLevel) == 0:
                            newLevel.append(1) 
                        elif type(newLevel[-1]) == int:
                            newLevel[-1] += 1
                        else:
                            newLevel.append(1)    
                    if i.hasRightChild():
                        mayContinue = True
                        newLevel.append(i.rightChild)
                    else:
                        if len(newLevel) == 0:
                            newLevel.append(1) 
                        elif type(newLevel[-1]) == int:
                            newLevel[-1] += 1
                        else:
                            newLevel.append(1)            
            level = newLevel   
            level_len = len(level)     
            stream.write(out)

    def add(self, key, value):
        if self.root is None:
            self.root = Node(None, value, key)
        else:
            newNode = self.__insert(self.root, 1, key, value)
            if (not (newNode[0] is None)):
                self.__splay(newNode[0], newNode[1])
    def min(self):
        if self.root is None:
            raise KeyError()
        tmp = self.__find_min(self.root, 1)
        self.__splay(tmp[0], tmp[1])
        return tmp[0].key, tmp[0].value   
    def max(self):
        if self.root is None:
            raise KeyError('')
        tmp = self.__find_max(self.root, 1)
        self.__splay(tmp[0], tmp[1])
        return tmp[0].key, tmp[0].value   
    def search(self, key):
        if self.root is None:
            raise KeyError('')
        else:
            tmp = self.__find(self.root, 1, key, False)
            self.__splay(tmp[0], tmp[1])
            if not tmp[2]:
                raise KeyError('')
            return f'1 {tmp[0].value}'     
    def set(self, key, value):
        if self.root is None:
            raise KeyError('')
        tmp = self.__find(self.root, 1, key)
        self.__splay(tmp[0], tmp[1])    
        if not tmp[2]:
            raise KeyError('')
        tmp[0].value = value    
    def delete(self, key):
        if self.root is None:
            raise KeyError('')
        tmp = self.__find(self.root, 1, key)
        if not tmp[2]:
            self.__splay(tmp[0], tmp[1])
            raise KeyError('')
        if tmp[0] == self.root:
            self.root = self.__merge(self.root.leftChild, self.root.rightChild)
            return
        self.__splay(tmp[0], tmp[1])
        self.root = self.__merge(self.root.leftChild, self.root.rightChild)    

st = splayTree()

for line in sys.stdin:
    if len(line) == 0:
        continue
    if line == 'print\n':
        st.print(sys.stdout)
    elif line == 'min\n':
        try:
            min = st.min()
            print(f'{min[0]} {min[1]}')
        except KeyError:
            print('error')
            continue 
    elif line == 'max\n':
        try:
            max = st.max()
            print(f'{max[0]} {max[1]}')   
        except KeyError:
            print('error')
            continue  
    elif line.startswith('add '):
        tmp = line.split()
        if (len(tmp)) == 2:
            if line[len(tmp[0]) + 1 + len(tmp[1]):] == ' \n':
                try:
                    st.add(tmp[1], '')
                    continue
                except KeyError:
                    print('error')
                    continue
            else:
                print('error')
                continue
        elif (len(tmp)) != 3:
            print('error')
            continue
        try:
            st.add(tmp[1], tmp[2])
        except KeyError:
            print('error')
            continue
    elif line.startswith('set '):
        tmp = line.split()
        if (len(tmp)) == 2:
            if line[len(tmp[0]) + 1 + len(tmp[1]):] == ' \n':
                try:
                    st.set(tmp[1], '')
                    continue
                except KeyError:
                    print('error')
                    continue
            else:
                print('error')
                continue
        elif (len(tmp)) != 3:
            print('error')
            continue
        try:
            st.set(tmp[1], tmp[2])
        except KeyError:
            print('error')
            continue   
    elif line.startswith('search '):
        tmp = line.split()
        if (len(tmp)) != 2 or line[-1] == ' ':
            print('error')
            continue
        try:
            print(st.search(tmp[1]))
        except KeyError:
            print(0)
            continue 
    elif line.startswith('delete '):
        tmp = line.split()
        if (len(tmp)) != 2 or line[-1] == ' ':
            print('error')
            continue
        try:
            st.delete(tmp[1])
        except KeyError:
            print('error')
            continue     
    else:
        print('error')    
