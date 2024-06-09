#include <iostream>
#include <vector> // динамический массив (вектор)
#include <forward_list> // односвязный список
#include <cstdarg> // переменное число аргументов
#include <random> // генерация случайных чисел
#include <set> // использование контейнера множество (set)

template<class T>
class queue
{
public:
    class q // Внутренний класс, представляющий узел очереди
    {
    public:
        T value; // Значение, хранимое в узле
        q* next; // Указатель на следующий узел
    };
    q* f = nullptr; // указатель на первый элемент очереди
    q* last = nullptr; // указатель на последний элемент очереди
public:
    void push(T a) // Метод для добавления элемента в очередь
    {
        q* p = new q(); // Создаем новый узел
        p->value = a; // Присваиваем ему значение
        p->next = nullptr; // Устанавливаем указатель на следующий узел как nullptr
        if (f == nullptr) // Если очередь пуста
        {
            f = p; // Первый и последний элемент указывают на новый узел
            last = p;
        }
        else // Если очередь не пуста
        {
            last->next = p; // Указываем, что последний элемент теперь указывает на новый узел
            last = p; // Последним элементом становится новый узел
        }
    }
    bool pop(T& a) // удаление элемента из очереди
    {
        q* p;
        if (f == nullptr) // Если очередь пуста, возвращаем false
            return false;
        else
        {
            a = f->value; // Получаем значение первого элемента
            p = f; // Временный указатель на первый элемент
            f = f->next; // Первый элемент теперь указывает на следующий узел
            delete p; // Удаляем старый первый элемент
            return true; // Возвращаем true, так как элемент был успешно удален
        }
    }
    class Iterator // Внутренний класс итератора для перебора элементов очереди
    {
    private:
        q* current; // Указатель на текущий элемент
    public:
        Iterator(q* cell) : current(cell) {} // Конструктор итератора
        Iterator& operator++() // Оператор инкремента для перехода к следующему элементу
        {
            current = current->next; // Переход к следующему узлу
            return *this; // Возврат текущего итератора
        }
        bool operator!=(const Iterator& other) const // неравенство итераторов
        {
            return current != other.current;
        }
        bool operator==(const Iterator& other) const // равенство итераторов
        {
            return current == other.current;
        }
        T& operator*() // Оператор разыменования для доступа к значению узла
        {
            return current->value;
        }
    };
    Iterator begin() // получение итератора на начало очереди
    {
        return Iterator(f);
    }
    Iterator end() // получение итератора на конец очереди
    {
        return Iterator(nullptr);
    }
    bool empty() // проверка, пуста ли очередь
    {
        return begin() == end();
    }
};

template<typename T>
class stack
{
public:
    struct stack_item // Структура, представляющая узел стека
    {
        T value; // Значение, хранимое в узле
        stack_item *next; // Указатель на следующий узел
    };
    stack_item *beginning = nullptr; // Указатель на верхний элемент стека
    void output(stack_item *a) // Рекурсивный метод для вывода элементов стека
    {
        if (a == NULL) return; // Если узел пустой, ничего не делаем
        output(a->next); // Рекурсивный вызов для следующего узла
        std::cout << a->value << " "; // Вывод значения узла
    }
public:
    void push(T item) // Метод для добавления элемента в стек
    {
        stack_item *c = new stack_item(); // Создаем новый узел
        c->value = item; // Присваиваем ему значение
        c->next = beginning; // Устанавливаем указатель на следующий узел как текущий верхний
        beginning = c; // Новый узел становится верхним
    }
    T pop() // Метод для удаления элемента из стека
    {
        if (beginning != NULL) // Если стек не пуст
        {
            stack_item *a = beginning; // Временный указатель на верхний элемент
            beginning = a->next; // Верхним элементом становится следующий узел
            T value = a->value; // Сохраняем значение удаляемого узла
            delete a; // Удаляем верхний узел
            return value; // Возвращаем значение удаленного узла
        }
        else
        {
            throw std::out_of_range("Stack is empty!"); // Если стек пуст, выбрасываем исключение
        }
    }
    class Iterator // Внутренний класс итератора для перебора элементов стека
    {
    private:
        stack_item* current; // Указатель на текущий элемент
        stack_item* prev; // Указатель на предыдущий элемент
    public:
        Iterator(stack_item* cell) : current(cell) {} // Конструктор итератора
        Iterator& operator++() // Оператор инкремента для перехода к следующему элементу
        {
            prev = current; // Сохраняем текущий элемент как предыдущий
            current = current->next; // Переход к следующему узлу
            return *this; // Возврат текущего итератора
        }
        bool operator!=(const Iterator& other) const // неравенство итераторов
        {
            return current != other.current;
        }
        T& operator*() // разыменование для доступа к значению узла
        {
            return current->value;
        }
    };
    Iterator begin() // получение итератора на начало стека
    {
        return Iterator(beginning);
    }
    Iterator end() // получение итератора на конец стека
    {
        return Iterator(nullptr);
    }
    bool empty() // проверка, пуст ли стек
    {
        return !(begin() != end());
    }
};

template<typename T>
class vec {
public:
    void push(const T& value) // добавление элемента в вектор
    {
        vector.push_back(value); // добавление элемента в конец вектора
    }
    typename std::vector<T>::iterator begin() // получение итератора на начало вектора
    {
        return vector.begin(); // возврат итератора на первый элемент вектора
    }
    typename std::vector<T>::iterator end() // получение итератора на конец вектора
    {
        return vector.end(); // возврат итератора, указывающего на позицию после последнего элемента вектора
    }
public:
    std::vector<T> vector; // хранение элементов
    bool empty() // проверка, пуст ли вектор
    {
        return begin() == end(); // Вектор пуст, если итератор на начало равен итератору на конец
    }
};

template<typename T>
class forwardList {
public:
    void push(const T& value) // добавление элемента в список
    {
        list.push_front(value); // добавление элемента в начало списка
    }
    typename std::forward_list<T>::iterator begin() // получение итератора на начало списка
    {
        return list.begin(); // возврат итератора на первый элемент списка
    }
    typename std::forward_list<T>::iterator end() // получение итератора на конец списка
    {
        return list.end(); // возврат итератор, указывающего на позицию после последнего элемента списка
    }
    bool empty() // проверка, пуст ли список
    {
        return begin() == end(); // список пуст, если итератор на начало равен итератору на конец
    }
private:
    std::forward_list<T> list; // односвязный список для хранения элементов
};

class ticket
{
public:
    std::pair<int, int> num_prize; // Пара чисел, представляющая билет: первое число - номер билета, второе число - приз

    bool operator<(const ticket& other) const // перегрузка оператора для сравнения билетов
    {
        if (num_prize.first != other.num_prize.first) // сравниваем первые элементы пар
        {
            return num_prize.first < other.num_prize.first; // если первые элементы не равны, возвращаем результат их сравнения
        }
        return num_prize.second < other.num_prize.second; // если первые элементы равны, сравниваем вторые элементы
    }
    ticket(int num, int p) : num_prize{num, p} {} // конструктор, инициализирующий билет номером и призом
    ticket() {}
    void swap(ticket& t) // обмен содержимого с другим билетом
    {
        std::swap(num_prize, t.num_prize); // swap для обмена пар
    }
    friend void swap(ticket& t1, ticket& t2);
};

void swap(ticket& t1, ticket& t2)
{
    std::swap(t1.num_prize, t2.num_prize); // обмен билетами
}

template<typename T>
class results
{
public:
    int amount_of_all_tickets; // количество всех билетов
    int amount_of_sold_tickets; // количество проданных билетов
    T winners; // контейнер для хранения победителей
    results(int all, int sold) : amount_of_all_tickets(all), amount_of_sold_tickets(sold) {} // Конструктор для инициализации полей
};

template <typename T>
void calculate_the_winners(T& winners, int amount_of_sold_tickets, T tickets)
{
    if (amount_of_sold_tickets <= 0 || tickets.empty()) // Проверка на валидность аргументов
    {
        throw std::invalid_argument("invalid argument in calculate_the_winners()");
    }

    int holder = amount_of_sold_tickets; // подсчет оставшихся билетов
    for (auto i = tickets.begin(); i != tickets.end(); ++i) // перебор всех билетов с использованием итератора
    {
        if (holder == 0) // Если проданные билеты закончились, выходим из цикла
        {
            break;
        }
        else if ((*i).num_prize.second == 0) // Если приз за билет равен нулю, пропускаем этот билет
        {
            continue;
        }
        else
        {
            winners.push(*i); // Если есть приз, добавляем билет в список победителей
        }
        holder--; // Уменьшаем счетчик проданных билетов
    }
}

template <typename T>
void tickets_generation(std::mt19937 g, int tikets_amount, T& tickets, int prizes_types, ...)
{
    if (tikets_amount <= 0 || prizes_types <= 0 )
    {
        throw std::invalid_argument("invalid argument in tickets_generation()");
    }
    std::vector<int> prizes_value, amount_prize_type; // векторы для хранения призовых сумм и количества билетов каждого типа
    va_list args;
    va_start(args, prizes_types);
    for (int i = 0; i < prizes_types; ++i)
    {
        int a = va_arg(args, int); // Извлечение следующего аргумента, представляющего количество билетов данного типа
        amount_prize_type.push_back(a); // Добавление количества билетов в вектор
        int hold = va_arg(args, int); // Извлечение следующего аргумента, представляющего призовую сумму
        for (auto i = 0; i < a; ++i)
        {
            prizes_value.push_back(hold); // Добавление призовой суммы в вектор соответствующее количество раз
        }
    }
    va_end(args);
    std::vector<int> sh_numbers; // Вектор для хранения номеров билетов
    for (auto i = 1; i <= tikets_amount; ++i )
    {
        sh_numbers.push_back(i); // Заполнение вектора номерами билетов от 1 до tikets_amount
    }
    int amount_of_prize_tickets = 0; // Переменная для хранения общего количества билетов с призами
    for (auto i = amount_prize_type.cbegin(); i != amount_prize_type.cend(); ++i)
    {
        amount_of_prize_tickets += *i; // Суммирование количества билетов с призами
    }
    std::set<int> numbersContain; // Множество для хранения уникальных номеров билетов с призами
    while (numbersContain.size() < amount_of_prize_tickets)
    {
        std::uniform_int_distribution<int> distribution(1, tikets_amount); // Создание распределения для случайного выбора номера билета
        int random = distribution(g); // Генерация случайного номера билета
        numbersContain.insert(random); // Добавление случайного номера в множество (если номер уже существует, он не будет добавлен)
    }
    int counter = 0; // Счетчик для призовых сумм
    for (auto i = 1; i <= tikets_amount; ++i)
    {
        auto it = numbersContain.find(sh_numbers[i-1]); // Проверка, есть ли текущий номер билета в множестве призовых номеров
        if (it != numbersContain.end())
        {
            ticket ticket(sh_numbers[i-1], prizes_value[counter]); // Создание билета с призом
            tickets.push(ticket); // Добавление билета в контейнер
            counter++; // Увеличение счетчика призов
        }
        else
        {
            ticket ticket(sh_numbers[i-1], 0); // Создание билета без приза
            tickets.push(ticket); // Добавление билета в контейнер
        }
    }
}

int main()
{
    try
    {
        std::random_device rd; // генерация случайных чисел
        std::mt19937 g(rd()); // Инициализация генератора случайных чисел
        int tikets_amount = 25000000; // Общее количество билетов
        int sold = 20000000; // Количество проданных билетов
        while (true) {
            int option;
            std::cout << "1. Using a stack.\n";
            std::cout << "2. Using the forward list.\n";
            std::cout << "3. Using a queue based on a singly linked list.\n";
            std::cout << "4. Using vector.\n";
            std::cout << "5. Exit.\n";
            std::cout << "Enter an option: ";
            while (!(std::cin >> option) || option < 1 || option > 5) {
                std::cout << "Invalid input, please enter a number from 1 to 5: ";
                std::cin.clear(); // Сброс флага ошибки потока
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            if (option == 5) break;
            std::cout << "Please, wait a little bit\n";
            switch (option) {
                case 1: {
                    stack<ticket> tickets1;
                    stack<ticket> winners1;
                    results<stack<ticket>> edition1(tikets_amount, sold);
                    tickets_generation(g, edition1.amount_of_all_tickets, tickets1, 4, 1, 4, 2, 3, 3, 2, 4, 1);
                    calculate_the_winners(winners1, edition1.amount_of_sold_tickets, tickets1);
                    for (auto i = winners1.begin(); i != winners1.end(); ++i)
                    {
                        std::cout << "Ticket:" << (*i).num_prize.first << " -- Prize is " << (*i).num_prize.second << " BTC" << std::endl;
                    }
                    break;
                }
                case 2: {
                    forwardList<ticket> tickets2;
                    forwardList<ticket> winners2;
                    results<forwardList<ticket>> edition2(tikets_amount, sold);
                    tickets_generation(g, edition2.amount_of_all_tickets, tickets2, 4, 1, 4, 2, 3, 3, 2, 4, 1);
                    calculate_the_winners(winners2, edition2.amount_of_sold_tickets, tickets2);
                    for (auto i = winners2.begin(); i != winners2.end(); ++i)
                    {
                        std::cout << "Ticket:" << (*i).num_prize.first << " -- Prize is " << (*i).num_prize.second << " BTC" << std::endl;
                    }
                    break;
                }
                case 3: {
                    queue<ticket> tickets3;
                    queue<ticket> winners3;
                    results<queue<ticket>> edition3(tikets_amount, sold);
                    tickets_generation(g, edition3.amount_of_all_tickets, tickets3, 4, 1, 4, 2, 3, 3, 2, 4, 1);
                    calculate_the_winners(winners3, edition3.amount_of_sold_tickets, tickets3);
                    for (auto i = winners3.begin(); i != winners3.end(); ++i)
                    {
                        std::cout << "Ticket:" << (*i).num_prize.first << " -- Prize is " << (*i).num_prize.second << " BTC" << std::endl;
                    }
                    break;
                }
                case 4: {
                    vec<ticket> tickets4;
                    vec<ticket> winners4;
                    results<vec<ticket>> edition4(tikets_amount, sold);
                    tickets_generation(g, edition4.amount_of_all_tickets, tickets4, 4, 1, 4, 2, 3, 3, 2, 4, 1);
                    calculate_the_winners(winners4, edition4.amount_of_sold_tickets, tickets4);
                    for (auto i = winners4.begin(); i != winners4.end(); ++i) {
                        std::cout << "Ticket:" << (*i).num_prize.first << " -- Prize is " << (*i).num_prize.second << " BTC" << std::endl;
                    }
                    break;
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n'; // Вывод сообщения об ошибке, если исключение было выброшено
    }
    return 0;
}