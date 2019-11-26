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

size_t Hash(const std::string& key, size_t size) {
    size_t hash = 0;
    for (char sym: key) {
        hash = (hash * a + sym) % size;
    }
    return hash;
}

class HashTable {
public:
    explicit HashTable(size_t initial_size);
    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    bool Has(const std::string& key) const;
    bool Add(const std::string& key);
    bool Remove(const std::string& key);

private:
    void Rehash();
    std::vector<std::string> table;
    size_t table_size = 0;
};

HashTable::HashTable(size_t initial_size) : table(std::vector<std::string>(initial_size, "")) {}

bool HashTable::Has(const std::string& key) const {
    assert(!key.empty());

    size_t hash = Hash(key, table.size()), iter = 0;
    while (table[hash] != "" && table[hash] != "Deleted") {
        if (table[hash] == key)
            return true;
        
        iter++;
        hash = (hash + iter + 1) % table.size();
    }
    
    return false;
}

bool HashTable::Add(const std::string& key) {
    assert(!key.empty());
    
    if (4 * table_size >= 3 * table.size())
        Rehash();

    size_t hash = Hash(key, table.size()), iter = 0;
    while (table[hash] != "") {
        if (table[hash] == key)
            return false;
            
        iter++;
        hash = (hash + iter + 1) % table.size();
    }
    
    table[hash] = key;
    table_size++;
    return true;
}

bool HashTable::Remove(const std::string& key) {
    assert(!key.empty());

    size_t hash = Hash(key, table.size()), iter = 0;
    while (table[hash] != "") {
        if (table[hash] == key) {
            table[hash] = "Deleted";
            return true;
        }
        
        iter++;
        hash = (hash + iter + 1) % table.size();
    }
    
    return false;
}

void HashTable::Rehash() {
    HashTable new_table(2 * table.size());

    for (size_t i = 0; i < table.size(); i++) {
        if (table[i] != "" && table[i] != "Deleted")
            new_table.Add(table[i]);
    }
    
    this->table = std::move(new_table.table);
    this->table_size = new_table.table_size;
}

int main() {
    HashTable table(8);
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


