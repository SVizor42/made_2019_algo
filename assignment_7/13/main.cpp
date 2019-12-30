/*

13. «Пятнашки»
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12] 
[ 13 14 15 0 ]
где 0 задает пустую ячейку.
Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
Формат входных данных:
Начальная расстановка.
Формат выходных данных:
Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек, которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую последовательность ходов: L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.

*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

const size_t HashParameter = 37;

struct Position {
    std::vector<char> chips;
    char zero_place;
    size_t distance;

    bool IsFinish() const;
    std::vector<Position> Siblings() const;
    void ManhattanDist();

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const { return !operator==(other); }
    bool operator<(const Position& other) const { return distance > other.distance; }
};

namespace std {
    template <>
    struct hash<Position> {
        size_t operator()(const Position& key) const {
            size_t value = 0;
            for (auto chip : key.chips) {
                value = HashParameter * value + static_cast<size_t>(chip);
            }
            return value;
        }
    };
}

const Position FinishPosition{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, 15};

bool Position::IsFinish() const {
    return operator==(FinishPosition);
}

std::vector<Position> Position::Siblings() const {
    std::vector<Position> result;
    if (zero_place < 12) {
        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 4]);
        sibling.zero_place += 4;
        result.emplace_back(sibling);
    }
    if (zero_place > 3) {
        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 4]);
        sibling.zero_place -= 4;
        result.emplace_back(sibling);
    }
    if (zero_place % 4 != 0) {
        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 1]);
        sibling.zero_place--;
        result.emplace_back(sibling);
    }
    if (zero_place % 4 != 3) {
        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 1]);
        sibling.zero_place++;
        result.emplace_back(sibling);
    }
    
    return result;
}

void Position::ManhattanDist() {
    distance = 0;
    for (size_t i = 0; i < 16; i++) {
        if (chips[i]) {
            distance += (abs(i % 4 - (chips[i] - 1) % 4) + abs(i / 4 - (chips[i] - 1) / 4));
        }
    }
}

bool Position::operator==(const Position& other) const {
    for (size_t i = 0; i < chips.size(); ++i ) {
        if (chips[i] != other.chips[i]) 
            return false;
    }
    
    return true;
}

char GetMoveSymbol(const Position& from, const Position& to) {
    char zero_diff = to.zero_place - from.zero_place;
    switch (zero_diff) {
        case 1:
            return 'L'; // Ноль вправо -> фишка влево
        case -1:
            return 'R';
        case 4:
            return 'U';
        case -4:
            return 'D';
        default:
            assert(false);
    }
    
    return 0;
}

bool BFS(const Position& start, std::unordered_map<Position, Position>& parents) {
    std::priority_queue<Position> positions_queue;
    positions_queue.push(start);
    while (!positions_queue.empty()) {
        
        Position current = positions_queue.top();
        positions_queue.pop();
        for (auto sibling : current.Siblings()) {
            if (parents.count(sibling)) {
                continue;
            }
            parents[sibling] = current;
            if (sibling.IsFinish()) {
                return true;
            }
            
            parents.insert(std::make_pair(sibling, current));
            sibling.ManhattanDist();
            positions_queue.push(sibling);
        }
    }
    
    return false;
}

std::vector<char> GetPath(const Position& start,
    const std::unordered_map<Position, Position>& parents) {
    
    std::vector<char> result;
    Position current = FinishPosition;
    while (current != start) {
        const Position parent = parents.at(current);
        result.push_back(GetMoveSymbol(parent, current));
        current = parent;
    }
    // Развернем result, так как собирали его с конца.
    std::reverse(result.begin(), result.end());
    
    return result;
}

std::vector<char> SolvePuzzle15(const Position& start) {
    // Запустим BFS, запоминая предков всех пройденных позиций.
    std::unordered_map<Position, Position> parents;
    if (!BFS(start, parents)) {
        return std::vector<char>();
    }
    
    return GetPath(start, parents);
}

bool HasSolution(const Position& position) {
    // Проверка существования решения
    size_t inv = 0;
    for (size_t i = 0; i < 16; i++) {
    	if (position.chips[i]) {
    		for (size_t j = 0; j < i; j++) {
    			if (position.chips[j] > position.chips[i])
    				inv++;
    		}
    	}
    }
    for (size_t i = 0; i < 16; i++) {
        if (position.chips[i] == 0)
            inv += (1 + i / 4);
    }
     
    return !(inv & 1);    
}

int main() {
    Position start{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, 15};
    
    int cell = 0;
    for (size_t i = 0; i < 16; i++) {
        std::cin >> cell;
        start.chips[i] = cell;
        if (!start.chips[i])
            start.zero_place = i;
    }

    bool solvable = HasSolution(start);
    if (solvable) {
        start.ManhattanDist();
        const auto result = SolvePuzzle15(start);
        std::cout << result.size() << std::endl;
        if (!result.empty()) {
            for (const auto move : result)
                std::cout << move;
        } 
    }
    else {
        std::cout << -1 << std::endl;
    }
    
    return 0;
}
