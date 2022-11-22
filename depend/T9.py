class Node:
    def __init__(self) -> None:
        self.children = dict()
        self.isLeaf = False


class Levenstein:
    def __init__(self) -> None:
        self.__root = Node

    def add(self, word: str):
        word = word.lower()
        curNode = self.__root
        for index in range(len(word)):
            

    fun addWordToDictionary(wordToAdd: String) {
        val word = wordToAdd.lowercase()
        var currentNode = root
        // currentWordIndex позволяет работать с суффиксом слова, избегая рассмотрения префикса
        var currentWordIndex = 0
        for (i in 0 until word.length) {
            val transitSymbol = word[i]
            if (currentNode.kids.contains(transitSymbol)) {
                currentNode = currentNode.kids[transitSymbol]!!
            } else {
                currentNode.kids[transitSymbol] = Node()
                currentNode = currentNode.kids[transitSymbol]!!
            }
        }
        currentNode.isLeaf = true
    }

    private fun search(wordToSearch: String): Boolean {
        var currentNode = root
        val word = wordToSearch.lowercase()
        for (transitSymbol in word) {
            if (currentNode.kids.contains(transitSymbol)) {
                currentNode = currentNode.kids[transitSymbol]!!
            } else {
                return false
            }
        }
        if (currentNode.isLeaf) return true
        return false
    }
    fun check(wordToCheck: String): List<String> {
        matched.clear()
        var currentNode = root
        val word = wordToCheck.lowercase()
        //TODO change exc
        if (search(wordToCheck)) throw Exception()
        checkRecursive(root, word, 0, false, "")
        matched.sort()
        return matched
    }

    private fun checkRecursive(
        currentNode: Node,
        word: String,
        index: Int,
        hasMistakeInParent: Boolean,
        code: String
    ) {
        if(word == "dui") {
            1
        }

        if (index == word.length && currentNode.isLeaf) {
            matched.add(code)
            return
        }
        if (index == word.lastIndex && !hasMistakeInParent && currentNode.isLeaf) {
            matched.add(code)
        }
        if (index == word.length && !hasMistakeInParent && currentNode.kids.isNotEmpty()) {
            for ((key, node) in currentNode.kids) {
                if (node.isLeaf) {
                    matched.add(code + key)
                }
            }
            return
        }
        for ((key, node) in currentNode.kids) {
            if (index == word.length) {
                return
            }
            if (key != word[index]) {
                if (index < word.lastIndex && key == word[index + 1]) {
                    // ошибка вставки лишнего символа
                    if (hasMistakeInParent) continue
                    checkRecursive(currentNode, word, index + 1, true, code)
                }
                if (index < word.lastIndex && key == word[index + 1] && node.kids.contains(word[index])) {
                    // ошибка перестановки
                    if (hasMistakeInParent) continue
                    checkRecursive(node.kids[word[index]]!!, word, index + 2, true, code + key + word[index])
                }
                if (node.kids.contains(word[index])) {
                    // ошибка пропуск символа
                    if (hasMistakeInParent) continue
                    checkRecursive(node, word, index, true, code + key)
                }
                if (key != word[index]) {
                    // ошибка неправильного символа
                    if (hasMistakeInParent) continue
                    checkRecursive(node, word, index + 1, true, code + key)
                }
            } else {
                checkRecursive(node, word, index + 1, hasMistakeInParent, code + key)
            }
        }
    }
}


fun main() {
    val scanner = Scanner(System.`in`)
    val autocorrector = Autocorrector()
    autocorrector.addWordToDictionary("hel")
    autocorrector.addWordToDictionary("hep")
    val list = autocorrector.check("ohel")
    for (elem in list) {
        println(elem)
    }
}