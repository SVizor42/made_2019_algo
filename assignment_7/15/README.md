«Приближенное решение метрической неориентированной задачи коммивояжера». Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1. Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
Автоматизируйте запуск экспериментов.
В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.

Результаты:
Experiment N=2: mean=1 std=0
Experiment N=3: mean=1 std=0
Experiment N=4: mean=0.923162 std=0.185021
Experiment N=5: mean=0.874592 std=0.187602
Experiment N=6: mean=0.719402 std=0.236344
Experiment N=7: mean=0.734223 std=0.282536
Experiment N=8: mean=0.725931 std=0.231945
Experiment N=9: mean=0.537451 std=0.31281
Experiment N=10: mean=0.586146 std=0.342374
