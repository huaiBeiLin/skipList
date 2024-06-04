#include<stdlib.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

template<typename K, typename V>
class Node {
private:
    K key;
    V val;
    int node_level;
    
public:
    Node** forwards;
    
public:
    Node(K inkey, V value, int level): key(inkey), val(value), 
        node_level(level),forwards(nullptr) 
    {
        this->forwards = new Node<K, V>*[node_level+1];
        memset(this->forwards, 0, sizeof(Node<K,V>*)*(node_level+1));
    }
    ~Node(){
        delete forwards;
    };
    K getKey() const {
        return key;
    }
    V get_value() const {
        return val;
    }
    int getLevel() const {
        return node_level;
    }
    void set_value(V value) {
        val = value;
    }
    
};

template<typename K, typename V>
class SkipList {
private:
    Node<K,V>* HeadNode = nullptr;
    int _maxlevel = 0;
    int temp_level = 0;
public:
    SkipList(int maxlevel = 500) {
        _maxlevel = maxlevel;
        HeadNode = new Node<K, V>(K(),V(),maxlevel);
    }
    ~SkipList() { delete HeadNode; _maxlevel = 0; temp_level = 0; }
    int getRandomLevel() {
        srand((int)time(0));
        int k = 1;
        while (rand() % 2) {
            k++;
        }
        k = (k < _maxlevel) ? k : _maxlevel;
        return k;
    }
    int insert_element(const K key, const V value) {
        int flag = 0;
        Node<K,V>* cur = HeadNode;
        int curlevel = temp_level;
        while (curlevel) {
            cur = HeadNode;
            while (cur) {
                if (cur->getKey() == key) {
                    flag = 1;
                    cur->set_value(value);
                }
                cur = cur->forwards[curlevel]; 
            }
            curlevel--;
        }
        if (flag == 1) return 1;
        Node<K, V>* newNode = new Node<K,V>(key, value, getRandomLevel());
        if (temp_level < newNode->getLevel()) temp_level = newNode->getLevel();
        curlevel = newNode->getLevel();
        while (curlevel) {
            Node<K,V>* prev = HeadNode;
            cur = HeadNode->forwards[curlevel];
            while (cur) {
                if (cur->getKey() > newNode->getKey()) {
                    prev->forwards[curlevel] = newNode;
                    newNode->forwards[curlevel] = cur;
                    break;
                } 
                cur = cur->forwards[curlevel];
                prev = prev->forwards[curlevel];
            }   
            if (!cur)
                prev->forwards[curlevel] = newNode;
            curlevel--;
        }
        return 0;
    }
    bool search_element(K key) {
        Node<K,V>* cur = HeadNode;
        int curlevel = temp_level;
        while (curlevel) {
            cur = HeadNode->forwards[curlevel];
            while (cur) {
                if (cur->getKey() == key) return true;
                cur = cur->forwards[curlevel]; 
            }
            curlevel--;
        }
        return false;
    }
    void delete_element(K key) {
        Node<K,V>* cur = HeadNode;
        int curlevel = temp_level;
        while (curlevel) {
            Node<K,V>* prev = HeadNode;
            cur = HeadNode->forwards[curlevel];
            while (cur) {
                if (cur->getKey() == key) {
                    prev->forwards[curlevel] = cur->forwards[curlevel];
                    break;
                }
                cur = cur->forwards[curlevel];
                prev = prev->forwards[curlevel];
            }
            curlevel--;
        }
    }
    void print() {
        Node<K,V>* cur = HeadNode;
        int curlevel = temp_level;
        while (curlevel) {
            cur = HeadNode->forwards[curlevel];
            std::cout << "Level " << curlevel-1 << ": ";
            while (cur) {
                std::cout << cur->getKey() << ":" << cur->get_value() << ';';
                cur = cur->forwards[curlevel];
            }
            std::cout << std::endl;
            curlevel--;
        }
    }
    void dump_file() {
        std::ofstream out("D:\\dev_c++\\destination\\out.txt");
        if (out.is_open()) 
        {
            Node<K,V>* cur = HeadNode;
            int curlevel = temp_level;
            while (curlevel) {
                cur = HeadNode->forwards[curlevel];
                while (cur) {
                    out << cur->getKey() << ":" << cur->get_value() << '\n';
                    cur = cur->forwards[curlevel];
                }
                curlevel--;
            }
            out.close();
        }
    }
    void load_file() {
        std::ifstream in("D:\\dev_c++\\destination\\out.txt");
        char buffer[256];
        if (!in.is_open()) {
            std::cout << "open error";
        } else {
            while (!in.eof()) {
                std::string s;
                in >> s;
                int index = 0;
                for (int i = 0; i < s.length(); i++) {
                    if (s[i] == ':') {
                        index = i;
                        break;
                    }
                }
                int key = stoi(s.substr(0, index));
                int value = stoi(s.substr(index, s.length() - 1 - index));
                insert_element(key, value);
            }
        }
    }
};


int main() {
    SkipList<int, int> mySkipList;
    // int n,k,m;
    // std::cin >> n >> k >> m;
    // while (n--) {
    //     int key, v, r;
    //     std::cin >> key >> v;
    //     r = mySkipList.insert_element(key,v);
    //     if (r == 0) std::cout << "Insert Success" << std::endl;
    //     else std::cout << "Insert Failed" << std::endl;
    // }
    // while (k--) {
    //     int key;
    //     std::cin >> key;
    //     mySkipList.delete_element(key);
    // }
    // while (m--) {
    //     int k;
    //     std::cin >> k;
    //     if (mySkipList.search_element(k)) std::cout << "Search Success" << std::endl;
    //     else std::cout << "Search Failed" << std::endl;
    // }
    // mySkipList.dump_file();
    mySkipList.load_file();
    mySkipList.print();
}
