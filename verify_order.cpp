#include "src/AVL_TREE.hpp"
#include "src/BinaryHeap.hpp"
#include "src/Quick_sort.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Words = std::vector<std::string>;

Words load_words(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "No se pudo abrir " << path << std::endl;
        std::exit(1);
    }

    Words words;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        words.push_back(line);
    }

    return words;
}

bool verify(const Words& words, const std::string& name) {
    bool ordered = std::is_sorted(words.begin(), words.end());

    std::cout << name << ": "
              << (ordered ? "ORDENADO" : "ERROR")
              << " (" << words.size() << " palabras)" << std::endl;

    return ordered;
}

int main() {
    Words original = load_words("dataset.txt");

    std::cout << "Palabras leidas: " << original.size() << std::endl;

    Words quick = original;
    quick_sort(quick);

    Words heap_input = original;
    Binary_heap<std::string> heap(std::move(heap_input));
    Words heap_out = heap.sort_and_retrieve();

    AVL_tree<std::string> tree;
    for (std::string& word : original) {
        tree.insert(std::move(word));
    }
    Words avl_out = tree.sort_and_retrieve();

    bool all_ok = true;

    all_ok &= verify(quick, "Quicksort");
    all_ok &= verify(heap_out, "Heapsort");
    all_ok &= verify(avl_out, "Arbol AVL");

    std::cout << std::endl
              << (all_ok ? "TODOS LOS ALGORITMOS ORDENARON CORRECTAMENTE"
                         : "ALGUN ALGORITMO PRESENTA ERRORES")
              << std::endl;

    return all_ok ? 0 : 1;
}