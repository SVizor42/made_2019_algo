/*
8. Хеш-таблица
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству. 
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

const size_t a = 31;

struct Hash {
    size_t operator()(const std::string& key, size_t size) {
        size_t hash = 0;
        for (char sym: key) {
            hash = (hash * a + sym) % size;
        }
        return hash;
    }
};

template <class T, class THash>
class HashTable {
public:
    explicit HashTable(size_t initial_size, T init_empty, T init_deleted);
    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    bool Has(const T& key) const;
    bool Add(const T& key);
    bool Remove(const T& key);

private:
    void Rehash();
    std::vector<T> table;
    size_t table_size = 0;
    T empty;
    T deleted;
};

template <class T, class THash>
HashTable<T, THash>::HashTable(size_t initial_size, T init_empty, T init_deleted) 
    : empty(init_empty)
    , deleted(init_deleted) 
{
    table = std::vector<T>(initial_size, empty);        
}

template <class T, class THash>
bool HashTable<T, THash>::Has(const T& key) const {
    assert(key != empty);

    THash hash_func;
    size_t hash = hash_func(key, table.size()), iter = 0;
    while (table[hash] != empty && table[hash] != deleted) {
        if (table[hash] == key)
            return true;
        
        iter++;
        hash = (hash + iter + 1) % table.size();
    }
    
    return false;
}

template <class T, class THash>
bool HashTable<T, THash>::Add(const T& key) {
    assert(key != empty);
    
    if (key == deleted)
        return false;
    
    if (4 * table_size >= 3 * table.size())
        Rehash();

    THash hash_func;
    size_t hash = hash_func(key, table.size()), iter = 0;
    int index = -1;
    while (iter < table.size()) {

        if (table[hash] == empty) {
            table[hash] = key;
            table_size++;      
            return true;
        }
        
        if (table[hash] == key)
            return false;
            
        if (table[hash] == deleted && index < 0)
            index = hash;
            
        iter++;
        hash = (hash + iter + 1) % table.size();
    }
    
    if (index >= 0) {
        table[hash] = key;
        table_size++;      
        return true;            
    }
    
    return false;
}

template <class T, class THash>
bool HashTable<T, THash>::Remove(const T& key) {
    assert(key != empty);
    
    if (key == deleted)
        return false;

    THash hash_func;
    size_t hash = hash_func(key, table.size()), iter = 0;
    while (table[hash] != empty) {
        if (table[hash] == key) {
            table[hash] = deleted;
            table_size--;
            return true;
        }
        
        iter++;
        hash = (hash + iter + 1) % table.size();
    }
    
    return false;
}

template <class T, class THash>
void HashTable<T, THash>::Rehash() {
    HashTable new_table(2 * table.size(), empty, deleted);

    for (size_t i = 0; i < table.size(); i++) {
        if (table[i] != empty && table[i] != deleted)
            new_table.Add(table[i]);
    }
    
    this->table = std::move(new_table.table);
    this->table_size = new_table.table_size;
}

int main() {
    HashTable<std::string, Hash> table(8, "", "Deleted");
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
        case '?':
            std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
            break;
        case '+':
            std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}
