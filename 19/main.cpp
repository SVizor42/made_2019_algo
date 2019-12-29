/*

19. Поиск точек в прямоугольнике

На вход подаются точки и прямоугольники. Точка задается двумя координатами (x, y). Прямоугольник - четверкой чисел [left, bottom, right, top]. Точка (left, bottom) - принадлежит прямоугольнику, точка (right, top) - нет. (left < right, bottom < top) Для каждого прямоугольника нужно вывести, сколько добавленных точек он содержит.
X  в диапазоне [-180, 180)
Y -  [-90, 90)
Количество точек <= 100000, Количество прямоугольников <= 1000 

Формат входного файла
Количество точек N. Далее N строк с координатами точек (x пробел y). Далее количество запросов M. Прямоугольники в виде 4 координат.  
Формат выходного файла
Количество точек в каждом прямоугольнике.

*/

#include <assert.h>
#include <iostream>
#include <vector>
#include <memory>

struct Point {
    double lon;
    double lat;
};

struct Bbox {
    double left;
    double bottom;
    double right;
    double top;

    bool Contains(const Point& point) const {
        return left <= point.lon && bottom <= point.lat &&
            point.lon < right && point.lat < top;
    }
};

class KDTree {
public:
    explicit KDTree(std::vector<Point>& points);

    int CountPointsInBbox(const Bbox& bbox);

private:
    struct KDNode {
        Point median;
        std::unique_ptr<KDNode> left;
        std::unique_ptr<KDNode> right;

        explicit KDNode(const Point& median_) : median(median_) {}
    };

    std::unique_ptr<KDNode> root;

    static std::unique_ptr<KDNode> BuildTree(
        std::vector<Point>& points, size_t left, size_t right, int depth);
    static int CountPointsInBbox(
        const Bbox& bbox, const std::unique_ptr<KDNode>& node, int depth);
};

KDTree::KDTree(std::vector<Point>& points) {
    root = BuildTree(points, 0, points.size(), 0);
}

namespace {
template<typename comparator>
size_t Partition(
    std::vector<Point>& points, size_t left, size_t right, comparator cmp) {
    int pivot_pos = static_cast<int>(right) - 1;
    int i = static_cast<int>(left), j = pivot_pos - 1;
    while (i <= j) {
        for (; cmp(points[i], points[pivot_pos]); ++i) {}
        for (; j >= i && !cmp(points[j], points[pivot_pos]); --j) {}
        if (i < j) std::swap(points[i], points[j]);
    }
    std::swap(points[i], points[pivot_pos]);
    return static_cast<size_t>(i);
}

void LocateMedian(
    std::vector<Point>& points, size_t left, size_t right, int depth) {
    const size_t med = (left + right) / 2;

    while (true) {
        size_t pivot_pos = Partition(points, left, right,
            [depth](const Point& first, const Point& second){
            if(depth % 2 == 0) {
                return first.lon < second.lon;
            } else {
                return first.lat < second.lat;
            }
        });
        if (pivot_pos == med) return;
        if (pivot_pos < med) left = pivot_pos + 1;
        else right = pivot_pos;
    }
}
} // unnamed namespace

std::unique_ptr<KDTree::KDNode> KDTree::BuildTree(
    std::vector<Point>& points, size_t left, size_t right, int depth) {
    assert(left < right);
    const size_t med = (left + right) / 2;
    LocateMedian(points, left, right, depth);
    auto node = std::make_unique<KDNode>(points[med]);

    if (med > left) {
        node->left = BuildTree(points, left, med, depth + 1);
    }
    if (med + 1 < right) {
        node->right = BuildTree(points, med + 1, right, depth + 1);
    }
    return node;
}

int KDTree::CountPointsInBbox(
    const Bbox& bbox, const std::unique_ptr<KDNode>& node, int depth) {
    int count = 0;
    if (bbox.Contains(node->median)) {
        ++count;
    }
    if (depth % 2 == 0) {
        if (node->left && bbox.left <= node->median.lon) {
            count += CountPointsInBbox(bbox, node->left, depth + 1);
        }
        if (node->right && node->median.lon < bbox.right) {
            count += CountPointsInBbox(bbox, node->right, depth + 1);
        }
    } else {
        if (node->left && bbox.bottom <= node->median.lat) {
            count += CountPointsInBbox(bbox, node->left, depth + 1);
        }
        if (node->right && node->median.lat < bbox.top) {
            count += CountPointsInBbox(bbox, node->right, depth + 1);
        }
    }
    return count;
}

int KDTree::CountPointsInBbox(const Bbox& bbox) {
    if (!root) return 0;
    return CountPointsInBbox(bbox, root, 0);
}

int main() {

    size_t point_count = 0;
    std::cin >> point_count;
    std::vector<Point> points(point_count);
    for (size_t i = 0; i < point_count; i++) {
        std::cin >> points[i].lon >> points[i].lat;
    }
    
    KDTree tree(points);
    
    size_t rect_count = 0;
    std::cin >> rect_count;
    Bbox rect;
    for (size_t i = 0; i < rect_count; i++) {
        std::cin >> rect.left >> rect.bottom >> rect.right >> rect.top;
        std::cout << tree.CountPointsInBbox(rect) << std::endl;
    }

    return 0;
}