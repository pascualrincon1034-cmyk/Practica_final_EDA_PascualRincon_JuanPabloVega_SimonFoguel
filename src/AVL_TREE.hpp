#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <stdexcept>
#include <vector>
#include <stack>

template <typename Comparable>
class AVL_tree{

    private:

        class AVLnode;

    public:

        std::vector<Comparable> sort_and_retrieve(){
            std::vector<Comparable> vec;
            vec.reserve(theSize);

            std::vector<AVLnode*> storage;
            storage.reserve(height() + 1);
            std::stack<AVLnode*, std::vector<AVLnode*>> nodes(std::move(storage));
            
            AVLnode* current = root;
            
            while (current != nullptr || !nodes.empty()){
                
                while (current != nullptr){
                    nodes.push(current);
                    current = current->left;
                }

                current = nodes.top(); nodes.pop();
                vec.push_back(std::move(current->element));

                current = current->right;
            }
            return vec;
        }


        AVL_tree():
        root{nullptr}, theSize{0}{}

        ~AVL_tree(){
            theSize = 0;
            delete_tree(root);
        }

        AVL_tree(AVL_tree&& rhs) noexcept:
        root{nullptr}, theSize{std::exchange(rhs.theSize, 0)}{
            std::swap(root, rhs.root);
        }

        AVL_tree& operator=(AVL_tree&& rhs) noexcept{
            if (this != &rhs){
                std::swap(root, rhs.root);
                std::swap(theSize, rhs.theSize);
            }
            return *this;
        }

        AVL_tree(const AVL_tree& rhs):
        root{nullptr}, theSize{0}{
            if (rhs.root != nullptr){
                theSize = rhs.theSize;
                root = new AVLnode(rhs.root->element);
                copy_tree(root, rhs.root);
            }
        }

        AVL_tree& operator=(const AVL_tree& rhs){
            if (this != &rhs){
                AVL_tree temp = rhs;
                std::swap(temp.root, root);
                std::swap(temp.theSize, theSize);
            }
            return *this;
        }

        bool insert(const Comparable& obj){
            bool flag = true;
            insert_helper(root, obj, flag);
            if (flag) theSize++;
            return flag;
        }

        bool insert(Comparable&& obj){
            bool flag = true;
            insert_helper(root, std::move(obj), flag);
            if (flag) theSize++;
            return flag;
        }

        bool remove(const Comparable& obj){
            bool flag = true;
            remove_helper(obj, root, flag);
            if (flag) --theSize;
            return flag;
        }

        const Comparable& find_max() const{
            if (root == nullptr) throw std::out_of_range("empty tree");
            const AVLnode* node = root;
            while (node->right != nullptr){
                node = node->right;
            }
            return node->element;
        }

        const Comparable& find_min() const{
            if (root == nullptr) throw std::out_of_range("empty tree");
            const AVLnode* node = root;
            while (node->left != nullptr){
                node = node->left;
            }
            return node->element;
        }

        bool search(const Comparable& target) const{
            const AVLnode* node = root;
            while(node != nullptr){
                if (target < node->element) node = node->left;
                else if (node->element < target) node = node->right;
                else return true;
            }
            return false;
        }

        bool empty() const{
            return theSize == 0;
        }

        int size() const{
            return theSize;
        }

        int height() const{
            return get_node_height(root);
        }

        bool is_valid_avl() const{
            int h;
            return is_valid_avl_helper(root, nullptr, nullptr, h);
        }

        static constexpr std::size_t node_size(){
            return sizeof(AVLnode);
        }

        std::size_t memory_estimate() const{
            return static_cast<std::size_t>(theSize) * sizeof(AVLnode) + sizeof(*this);
        }

    private:

        int childs(const AVLnode* node) const{
            int childs = 0;
            if (node->right != nullptr) ++childs;
            if (node->left != nullptr) ++childs;
            return childs;
        }

        void remove_helper(const Comparable& obj, AVLnode*& node, bool& flag){
            if (node == nullptr){
                flag = false;
                return;
            }

            if (obj < node->element){
                remove_helper(obj, node->left, flag);

            } else if (node->element < obj){
                remove_helper(obj, node->right, flag);

            } else{
                AVLnode* old = node;
                int c = childs(old);

                if (c == 0){
                    node = nullptr;

                } else if (c == 1){
                    node = (old->left != nullptr) ? old->left : old->right;

                } else {
                    AVLnode* successor = remove_helper_helper(old->right);
                    successor->left = old->left;
                    successor->right = old->right;
                    node = successor;
                }
                delete old;
            }

            if (!flag) return;

            balance(node);
        }

        // Detaches and returns the minimum node of the subtree, rebalancing the path.
        AVLnode* remove_helper_helper(AVLnode*& node){
            if (node->left == nullptr){
                AVLnode* min = node;
                node = node->right;
                return min;
            }
            AVLnode* min = remove_helper_helper(node->left);
            balance(node);
            return min;
        }

        void insert_helper(AVLnode*& node, const Comparable& obj, bool& flag){
            if (node == nullptr) {
                node = new AVLnode(obj);
                return;
            }

            if (obj < node->element) {
                insert_helper(node->left, obj, flag);
            } else if (node->element < obj) {
                insert_helper(node->right, obj, flag);
            } else {
                flag = false;
                return;
            }

            if (!flag) return;

            balance(node);
        }

        void insert_helper(AVLnode*& node, Comparable&& obj, bool& flag){
            if (node == nullptr) {
                node = new AVLnode(std::move(obj));
                return;
            }

            if (obj < node->element) {
                insert_helper(node->left, std::move(obj), flag);
            } else if (node->element < obj) {
                insert_helper(node->right, std::move(obj), flag);
            } else {
                flag = false;
                return;
            }

            if (!flag) return;

            balance(node);
        }

        void balance(AVLnode*& node){
            if (node == nullptr) return;

            int bf = balancing_factor(node);
            if (bf > 1){
                if (balancing_factor(node->right) < 0){
                    RL_rotation(node);
                } else {
                    RR_rotation(node);
                }
            } else if (bf < -1){
                if (balancing_factor(node->left) > 0){
                    LR_rotation(node);
                } else {
                    LL_rotation(node);
                }
            } else {
                node->theHeight = std::max(get_node_height(node->left), get_node_height(node->right)) + 1;
            }
        }

        void LL_rotation(AVLnode*& node){
            AVLnode* temp = node;
            node = node->left;
            temp->left = node->right;
            node->right = temp;
            temp->theHeight = std::max(get_node_height(temp->left), get_node_height(temp->right)) + 1;
            node->theHeight = std::max(get_node_height(node->left), get_node_height(node->right)) + 1;
        }

        void RR_rotation(AVLnode*& node){
            AVLnode* temp = node;
            node = node->right;
            temp->right = node->left;
            node->left = temp;
            temp->theHeight = std::max(get_node_height(temp->left), get_node_height(temp->right)) + 1;
            node->theHeight = std::max(get_node_height(node->left), get_node_height(node->right)) + 1;
        }

        void RL_rotation(AVLnode*& node){
            LL_rotation(node->right);
            RR_rotation(node);
        }

        void LR_rotation(AVLnode*& node){
            RR_rotation(node->left);
            LL_rotation(node);
        }

        int balancing_factor(const AVLnode* node) const{
            return get_node_height(node->right) - get_node_height(node->left);
        }

        int get_node_height(const AVLnode* node) const{
            return node == nullptr ? -1 : node->height();
        }

        void copy_tree(AVLnode*& node1, const AVLnode* node2){
            node1->theHeight = node2->theHeight;

            if (node2->left != nullptr){
                node1->left = new AVLnode(node2->left->element);
                copy_tree(node1->left, node2->left);
            }

            if (node2->right != nullptr){
                node1->right = new AVLnode(node2->right->element);
                copy_tree(node1->right, node2->right);
            }
        }

        void delete_tree(AVLnode*& node){
            if (node == nullptr) return;

            delete_tree(node->left);
            delete_tree(node->right);

            delete node;
            node = nullptr;
        }

        // Checks BST ordering, real heights, and balance.
        bool is_valid_avl_helper(const AVLnode* node, const Comparable* lo,
                                 const Comparable* hi, int& h) const{
            if (node == nullptr){
                h = -1;
                return true;
            }
            if (lo != nullptr && !(*lo < node->element)) return false;
            if (hi != nullptr && !(node->element < *hi)) return false;

            int lh, rh;
            if (!is_valid_avl_helper(node->left, lo, &node->element, lh)) return false;
            if (!is_valid_avl_helper(node->right, &node->element, hi, rh)) return false;

            h = std::max(lh, rh) + 1;
            return h == node->theHeight && std::abs(rh - lh) < 2;
        }

        class AVLnode{

            public:

                AVLnode(const Comparable& obj):
                theHeight{0}, element{obj}, left{nullptr}, right{nullptr}{}

                AVLnode(Comparable&& obj):
                theHeight{0}, element{std::move(obj)}, left{nullptr}, right{nullptr}{}

                int height() const{
                    return theHeight;
                }

            private:

                int theHeight;
                Comparable element;
                AVLnode* left;
                AVLnode* right;

                friend class AVL_tree;

        };

        AVLnode* root;
        int theSize;

};

#endif