#include "src/AVL_TREE.hpp"
#include "src/BinaryHeap.hpp"
#include "src/Quick_sort.hpp"
#include "src/math.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <iostream>
#include <string>
#include <chrono>

const int NUMBER_OF_RUNS = 5;

std::vector<std::string> initialize_vector(){

    std::ifstream file("dataset.txt");
    if (!file.is_open()){
        std::cerr<<"failed access";
    }

    std::vector<std::string> words;
    words.reserve(100000);

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        words.push_back(line);
    }

    file.close();

    return words;
}

void quicksort(){
    std::vector<int> vec;

    for (int i = 0; i < NUMBER_OF_RUNS; i++){

        std::vector<std::string> words = initialize_vector();
        auto begin = std::chrono::high_resolution_clock::now();
        quick_sort(words);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        vec.push_back(duration.count());

        std::cout<<"Run "<< i + 1 << ": "<< duration.count() <<std::endl;

    }

    auto [mean, deviation] = mean_and_dev(vec);

    std::cout<<"Quick sort performance:"<<std::endl;
    std::cout<<"Mean: "<<mean<<std::endl;
    std::cout<<"Deviation: "<<deviation<<std::endl;


}


void heapsort(){
    std::vector<int> vec;

    for (int i = 0; i < NUMBER_OF_RUNS; i++){

        std::vector<std::string> words = initialize_vector(), sorted_words;
        auto begin = std::chrono::high_resolution_clock::now();

        Binary_heap<std::string> my_heap(std::move(words));
        sorted_words = my_heap.sort_and_retrieve();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        vec.push_back(duration.count());

        std::cout<<"Run "<< i + 1 << ": "<< duration.count() <<std::endl;
       
    }

    auto [mean, deviation] = mean_and_dev(vec);

    std::cout<<"Heap sort performance:"<<std::endl;
    std::cout<<"Mean: "<<mean<<std::endl;
    std::cout<<"Deviation: "<<deviation<<std::endl;

}

void avltree(){
    std::vector<int> vec;

    for (int i = 0; i < NUMBER_OF_RUNS; i++){

        std::vector<std::string> words = initialize_vector();
        AVL_tree<std::string> my_tree;
        auto begin = std::chrono::high_resolution_clock::now();

        for (std::string& word: words){
            my_tree.insert(std::move(word));
        }

        words = my_tree.sort_and_retrieve();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        vec.push_back(duration.count());

        std::cout<<"Run "<< i + 1 << ": "<< duration.count() <<std::endl;
    
    }

    auto [mean, deviation] = mean_and_dev(vec);

    std::cout<<"AVL tree performance:"<<std::endl;
    std::cout<<"Mean: "<<mean<<std::endl;
    std::cout<<"Deviation: "<<deviation<<std::endl;
}

int main(int argc, char* argv[]) {


    bool flags[3] = {}; 

    if (argc == 1) {
        for (int i = 0; i < 3; i++) {
            flags[i] = true;
        }
    } else {
        for (int i = 1; i < argc; i++) {
            try {
                int index = std::stoi(argv[i]) - 1;
                if (index >= 0 && index < 3) {
                    flags[index] = true;
                } else {
                    std::cerr << "Advertencia: Argumento fuera de rango ignorado: " << argv[i] << "\n";
                }
            } 
            catch (...) {
                std::cerr << "Advertencia: Argumento invalido ignorado: " << argv[i] << "\n";
            }
        }
    }

    
    if (flags[0]) {
        avltree();
    }

    if (flags[1]) {
        heapsort();
    }

    if (flags[2]){
        quicksort();
    }

}
