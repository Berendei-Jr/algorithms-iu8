#include <iostream>
#include <unordered_set>
#include <list>
#include <algorithm>

typedef std::unordered_set<std::list<std::string>> paths;

struct Lib {
    std::string _name;
    bool _hasDependencies = false;
    bool _isVulnerable;
    std::unordered_set<std::string> _dependencies;
    inline bool operator==(const Lib& l) const {
        return l._name == _name;
    }
};

bool operator==(const std::list<Lib>& l1, const std::list<Lib>& l2) {
    if (l1.size() == l2.size()) {
        for (auto i1 = l1.begin(), i2 = l2.begin(); i1 != l1.end(); ++i1, ++i2) {
            if (i1->_name != i2->_name)
                return false;
        }
        return true;
    }
    return false;
}
std::ostream& operator << (std::ostream& out, const std::list<std::string>& l) {
    auto it = l.begin();
    for (std::size_t i = 0; i < l.size() - 1; ++i) {
        out << *it << " ";
        ++it;
    }
    out << l.back();
    return out;
}

template<>
struct std::hash<Lib>
{
    std::size_t operator()(Lib const& l) const noexcept
    {
        return std::hash<std::string>{}(l._name);;
    }
};

template<>
struct std::hash<std::list<std::string>>
{
    std::size_t operator()(const std::list<std::string>& s) const noexcept
    {
        std::size_t h = 0;
        int counter = 0;
        for (auto& i: s) {
            if (counter % 2) {
                h += std::hash<std::string>{}(i);
            } else {
                h += 2 * std::hash<std::string>{}(i);
            }
            ++counter;
        }
        return h;
    }
};

void dependencyHandling (paths& p, const Lib& lib, const std::unordered_set<std::string>& vulLibs, std::list<std::string>& curPath, const std::unordered_set<Lib>& alldeps) {
    if (lib._isVulnerable) {
        p.insert(curPath);
    }
    if (lib._hasDependencies) {
        for (auto& i: lib._dependencies) {
            // Проверка, было ли такое имя раньше

            bool cycleDetected = false;
            auto firstCollision = std::find_if(curPath.begin(), curPath.end(), [&i](const std::string& s){ return s == i; });
            if (firstCollision != curPath.end()) { // найдена коллизия
                if (curPath.size() == 1) {
                    cycleDetected = true;
                } else {
                    if (firstCollision != curPath.begin()) {
                        auto fcDecremented = --firstCollision;
                        ++firstCollision;
                        auto second = --curPath.end();
                        bool cycle = true;
                        while (fcDecremented != curPath.begin() && second != firstCollision) {
                            if (*fcDecremented != *second) {
                                cycle = false;
                                break;
                            }
                            --fcDecremented;
                            --second;
                        }
                        cycleDetected = cycle;
                    }
                }
            }
            if (cycleDetected) {
                return;
            }

            Lib tmp {i, false, false, {}};
            if (vulLibs.find(i) != vulLibs.end())
                tmp._isVulnerable = true;
            auto iter = alldeps.find(tmp);
            if (iter != alldeps.end()) {
                tmp._hasDependencies = iter->_hasDependencies;
                tmp._dependencies = iter->_dependencies;
            }

            auto newPath = curPath;
            newPath.push_back(i);
            dependencyHandling(p, tmp, vulLibs, newPath, alldeps);
        }
    }
}

int main() {
    std::string str;
    std::getline(std::cin, str);
    if (str.empty())
        return 0;
    std::unordered_set<std::string> vulnerableLibs;
    auto begin = str.begin();
    auto it = begin;
    do {
        it = std::find_if(begin, str.end(), isspace);
        std::string tmp = std::string(begin, it);
        if (!tmp.empty()) {
            vulnerableLibs.emplace(tmp);
        }
        begin = it + 1;
    } while (it != str.end());

    std::getline(std::cin, str);
    if (str.empty())
        return 0;
    std::unordered_set<std::string> directDeps;
    begin = str.begin();
    do {
        it = std::find_if(begin, str.end(), isspace);
        std::string tmp = std::string(begin, it);
        if (!tmp.empty()) {
            directDeps.insert(tmp);
        }
        begin = it + 1;
    } while (it != str.end());

    std::unordered_set<Lib> allDeps;
    while (std::getline(std::cin, str)) {
        if (str.empty())
            break;
        begin = str.begin();
        it = std::find_if(begin, str.end(), isspace);
        std::string name = std::string(begin, it);
        if (name.empty())
            break;

        bool isVul = false;
        bool hasDeps = false;
        std::unordered_set<std::string> deps;

        for (auto &i: vulnerableLibs) {
            if (name == i) {
                isVul = true;
                break;
            }
        }
        if (it != str.end()) {
            do {
                begin = it + 1;
                it = std::find_if(begin, str.end(), isspace);
                std::string tmp = std::string(begin, it);
                if (!tmp.empty()) {
                    hasDeps = true;
                    deps.emplace(tmp);
                }
            } while (it != str.end());
        }
        allDeps.emplace( Lib{name, hasDeps, isVul, deps} );
    }

    paths allPathsToVulLibs;

    for (auto &i: directDeps) {
        bool isVul = false;
        bool hasDep = false;
        std::unordered_set<std::string> deps;
        if (std::find_if(vulnerableLibs.begin(), vulnerableLibs.end(), [&i](const std::string& s){ return s == i; }) != vulnerableLibs.end())
            isVul = true;
        for (auto& d: allDeps) {
            if (i == d._name) {
                hasDep = d._hasDependencies;
                deps = d._dependencies;
                break;
            }
        }
        Lib lib {i, hasDep, isVul, deps};
        std::list<std::string> l { lib._name };
        dependencyHandling(allPathsToVulLibs, lib, vulnerableLibs, l, allDeps);
    }

    if (allPathsToVulLibs.empty())
        return 0;
    auto iter = allPathsToVulLibs.begin();
    std::cout << *(iter++);
    for (; iter != allPathsToVulLibs.end(); ++iter) {
        std::cout << std::endl << *iter;
    }

}
