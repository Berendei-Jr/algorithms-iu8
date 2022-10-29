from re import S


class Node:
    def __init__(self, father, value, key, isLeftChild = True):
        self.father = father
        self.value = value
        self.key = key
        self.isLeftChild = isLeftChild
        self.rightChild = None
        self.LeftChild = None
    def hasLeftChild(self):
        return not (self.LeftChild is None)
    def hasRightChild(self):
        return not (self.rightChild is None)         
    def isRoot(self):
        return self.father == None
    def changeIsLeft(self):
        self.isLeftChild = not self.isLeftChild    

class splayTree:
    root = None
    def __init__(self):
        pass
    def __zig(self, node, left = True):
        self.print()
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
            if node.hasLeftChild:
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
            if node.hasRightChild:
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
        elif depth == 1:
            self.root = node
        else:
            if node.isLeftChild:
                if node.father.isLeftChild:
                    self.__zig_zig(node)
                else:
                    self.__zig_zag(node, False)
                self.splay(node, depth - 2)
            else:
                if node.father.isLeftChild:
                    self.__zig_zag(node)
                else:
                    self.__zig_zig(node, False)
                self.splay(node, depth - 2)        
    def __insert(self, cur_node, depth, key, value):
        if cur_node.key == key:
            print("error")
            return cur_node, depth
        if key < cur_node.key:
            if cur_node.hasLeftChild():
                return self.__insert(cur_node.leftChild, depth + 1, key, value)
            else:
                new_node = Node(cur_node, value, key)
                cur_node.leftChild = new_node
                new_node.father = cur_node
                return new_node, depth + 1
        else:
            if cur_node.hasRightChild:
                return self.__insert(cur_node.rightChild, depth + 1, key, value)
            else:
                new_node = Node(cur_node, value, key, False)
                cur_node.rightChild = new_node
                new_node.father = cur_node
                return new_node, depth + 1                    
    def print(self):
        if self.root is None:
            print("_")
        else:
            print('[', self.root.key, ' ', self.root.value, ']')
            l = 1    
            while True:
                out = ""
                power = 2**l
                for i in range (power):
                    a = self.root
                    way = str(bin(i))[:l]
                    for j in range (l):
                        if not (a is None):
                            break
                        if way[j] == '0':
                            a = a.leftChild
                        else:
                            a = a.rightChild    
                    if not (a is None):
                        out += "_ "
                    else:
                        out += '[' + str(a.key) + ' ' + str(a.value) + ' ' + str(a.father.key) + "] "
                if out == "_ " * power:
                    break
                else:
                    print(out[:len(out) - 1]) 
                l += 1
    def add(self, key, value):
        if self.root is None:
            self.root = Node(None, value, key)
        else:
            newNode = self.__insert(self.root, 1, key, value)
            if (not (newNode[0] is None)):
                self.__splay(newNode[0], newNode[1])

st = splayTree()
st.add(3, "3")
st.add(1, "1")
st.add(2, "2")
st.print()



