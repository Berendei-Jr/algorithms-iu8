import sys

class Lib:
    def __init__(self) -> None:
        self.name = ''
        self.dependencies = set()
        self.isVulnerable = False
        self.hasDependencies = False

vanurableLibs = set(input().split())
if len(vanurableLibs) == 0:
    sys.exit()
directDeps = set(input().split())
if len(directDeps) == 0:
    sys.exit()

allDeps = set()
for line in sys.stdin:
    words = line.split()
    for i in allDeps:
        if i.name == words[0] and len(words > 1):
            i.hasDependencies = True
            for j in range(1, len(words)):
                i.dependencies.add(words[j])
        lib = Lib()