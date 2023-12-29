# Практическая работа №4
Срок выполнения: с 2023-12-05 по 2023-12-05. 
Срок сдачи: 2023-12-05.

## Потоки ввода-вывода языка Си++

### Ответвление
Необходимо создать ответвление (fork) моего репозитория Work-04 (название
оставить без изменения, а видимость поставить приватной). Назначить меня в
новом репозитории соавтором с правами администратора. Вики, Задачи а также
Проекты отключить.

### Получения рабочей копии
Получить локальную копию файлов репозитория.

### Создание рабочей ветки
Создать новую ветку (branch) в локальной копии.

### Этап разработки
Разработать класс `StreamProcessor` выполняющий кодирование данных 
поступающих из входного потока по алгоритму RLE. Результат кодирования 
поступает в выходной поток. Формат записи результата: если старший бит 
сброшен, то младшие семь бит содержат количество повторений следующего 
байта; если старший бит установлен, то младшие семь бит и последующий 
байт содержат количество повторений следующего байта. 

Для класса `StreamProcessor` разработать конструктор позволяющий задать 
входной (`std::istream`) и выходной (`std::ostream`) потоки (по умолчанию 
входным и выходным потоком считаются `std::cin` и `std::cout`).

Реализовать в классе `StreamProcessor` метод `void Process()` выполняющий 
преобразование до тех пор пока во входном потоке есть данные. Предусмотреть 
возможность многократного последовательного вызова метода. 

В основной функции продемонстрировать работу класса `StreamProcessor`.

### Запрос слияния
Выполнить запрос на слияние (pull request) и назначить меня рецензентом.
