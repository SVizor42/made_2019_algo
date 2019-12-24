/*

16. Поиск подстроки
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­ n. Время O(n + p), доп. память – O(p).
Вариант 2. С помощью z-функции.
Формат входного файла:
Шаблон, символ перевода строки, строка.
Формат выходного файла:
Позиции вхождения шаблона в строке.

*/

#include <cstring>
#include <iostream>
#include <vector>

std::vector<int> zFunction(const std::string& str) {
    int str_size = str.length();
    std::vector<int> z(str_size);
    int left = 0, right = 0;
    for (int i = 1; i < str_size; i++) {
        if (i <= right) {
            z[i] = std::min(right - i + 1, z[i - left]);
        }
        while (i + z[i] < str_size && str[z[i]] == str[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }
    
    return z;    
}

std::vector<int> substringSearch(const std::string& pattern, const std::string& text) {
    int p_size = pattern.length();
    int t_size = text.length();   
    std::vector<int> zf_pattern = zFunction(pattern);
    
    std::vector<int> result;
    int left = 0, right = 0, zf = 0;
    for (int i = 0; i < t_size; i++) {
        zf = 0;
        if (i <= right) {
            zf = std::min(right - i + 1, zf_pattern[i - left]);
        }
        while (i + zf < t_size && pattern[zf] == text[i + zf]) {
            zf++;
            if (zf == p_size) {
                result.push_back(i);
                break;
            }
        }
        if (i + zf - 1 > right) {
            left = i;
            right = i + zf - 1;
        }
    }
    
    return result;
}

int main() {
    
    std::string text = "", pattern = "";
    std::cin >> pattern >> text;

    std::vector<int> result = substringSearch(pattern, text);
    
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << " ";
    }
    
    return 0;
}
