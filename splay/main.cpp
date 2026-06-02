#include "splay_tree_api.h"
#include <iostream>
#include <string>

int main() {
    SplayTree<int, std::string> tree;
    
    tree.insert(5, "Вставка выполнена");
    tree.insert(3, "3");
    tree.insert(4, "4");
    tree.insert(19, "19");
    
    auto* val = tree.search(5);
    if (val) {
        std::cout << *val << std::endl;
    }
    
    std::cout << "Size: " << tree.size() << std::endl;
    
    return 0;
}
