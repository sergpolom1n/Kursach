#include <iostream>
#include <stdexcept>        // обработка исключений
#include <string>
#include <unordered_set>    // хранение уникальных элементов
#include <sstream>          // поток строк
#include <fstream>          // файловые потоки
#include <vector>           // динамический массив
#include <unordered_map>    // хранение пар ключ-значение
#include <stack>            // работа с конструкцией стека


// работа с синонимами
class SynonymDictionary {
private:
    // Хеш-таблица для хранения синонимов и их канонических слов. Ключ — это синоним, значение — каноническое слово.
    std::unordered_map<std::string, std::string> synonym_map;

    // Хеш-таблица для хранения канонических слов и их синонимов. Ключ — каноническое слово, значение — вектор синонимов.
    std::unordered_map<std::string, std::vector<std::string>> canonical_map;

public:
    SynonymDictionary() = default;
    ~SynonymDictionary() = default;

    void loadFromFile(const std::string& filename) {
        // Открытие файла для чтения
        std::ifstream file(filename);
        if (!file.is_open()) {
            // Если файл не удалось открыть, выбрасывается исключение
            throw std::runtime_error("Could not open file: " + filename);
        }

        std::string line;
        // Чтение файла построчно
        while (std::getline(file, line)) {
            // Разбор и обработка каждой строки файла
            parseLine(line);
        }
        file.close();
    }

    // Метод для сохранения данных в файл
    void saveToFile(const std::string& filename) const {
        // Открытие файла для записи
        std::ofstream file(filename);

        if (!file.is_open()) {
            // Если файл не удалось открыть, выбрасывается исключение
            throw std::runtime_error("Could not open file: " + filename);
        }

        // Проход по всем элементам в "canonical_map"
        for (const auto& entry : canonical_map) {
            // Запись канонического слова в файл вместе с открывающей скобкой
            file << entry.first << "{";
            // Проход по всем синонимам для текущего канонического слова
            for (size_t i = 0; i < entry.second.size(); ++i) {
                file << entry.second[i];  // Запись текущего синонима в файл
                if (i < entry.second.size() - 1) { // Если текущий синоним не последний, добавление запятой и пробела
                    file << ", ";
                }
            }
            file << "}\n";  // Добавление закрывающей скобки и перевода строки
        }
        file.close();
    }


    // Метод для получения канонического слова для заданного слова
    // Если слово является синонимом, вернуть его каноническое слово, иначе вернуть само слово
    std::string getCanonicalWord(const std::string& word) const {
        auto it = synonym_map.find(word); // Ищем слово в хеш-таблице синонимов

        // Если слово найдено, возвращаем каноническое слово
        // Если не найдено, возвращаем само слово
        return it != synonym_map.end() ? it->second : word;
    }

    // Метод для добавления новой записи (каноническое слово с его синонимами)
    void addEntry(const std::string& canonical_word, const std::vector<std::string>& synonyms) {
        // Проход по всем синонимам
        for (const auto& synonym : synonyms) {
            // Добавляем каждое слово-синоним в хеш-таблицу синонимов с каноническим словом
            synonym_map[synonym] = canonical_word;
        }
        // Добавляем каноническое слово в хеш-таблицу канонических слов со списком его синонимов
        canonical_map[canonical_word] = synonyms;
    }

    // Метод для добавления синонима к существующему каноническому слову
    void addSynonym(const std::string& canonical_word, const std::string& synonym) {
        // Добавляем слово-синоним в хеш-таблицу с каноническим словом
        synonym_map[synonym] = canonical_word;
        // Добавляем синоним в вектор синонимов для данного канонического слова
        canonical_map[canonical_word].push_back(synonym);
    }

// Метод для удаления синонима для заданного канонического слова
    void removeSynonym(const std::string& canonical_word, const std::string& synonym) {
        // Ищем синоним в хеш-таблице синонимов
        auto it = synonym_map.find(synonym);
        // Проверяем, что синоним найден и его каноническое слово совпадает с заданным
        if (it != synonym_map.end() && it->second == canonical_word) {
            // Удаляем синоним из хеш-таблицы
            synonym_map.erase(it);

            // Удаляем синоним из вектора синонимов канонического слова
            auto& synonyms = canonical_map[canonical_word];
            synonyms.erase(std::remove(synonyms.begin(), synonyms.end(), synonym), synonyms.end());
        }
    }

// Метод для разбора строки из файла и добавления данных в словарь
// Строка имеет формат: каноническое слово { синоним1, синоним2, ... }
    void parseLine(const std::string& line) {
        // Создание потокового объекта для чтения строки
        std::istringstream stream(line);
        std::string canonical_word;

        // Извлечение канонического слова, ограниченного символом '{'
        if (std::getline(stream, canonical_word, '{')) {
            std::string synonyms_list;

            // Извлечение списка синонимов, ограниченного символом '}'
            if (std::getline(stream, synonyms_list, '}')) {
                // Создание потокового объекта для чтения списка синонимов
                std::istringstream synonyms_stream(synonyms_list);
                std::string synonym;
                std::vector<std::string> synonyms;

                // Чтение каждого синонима, разделенного запятыми
                while (std::getline(synonyms_stream, synonym, ',')) {
                    // Удаление пробелов в начале и в конце синонима
                    synonym.erase(0, synonym.find_first_not_of(" \t\n\r"));
                    synonym.erase(synonym.find_last_not_of(" \t\n\r") + 1);

                    // Добавление синонима в хеш-таблицу с каноническим словом
                    synonym_map[synonym] = canonical_word;

                    // Добавление синонима во временный вектор синонимов
                    synonyms.push_back(synonym);
                }

                // Добавление канонического слова и его синонимов в хеш-таблицу канонических слов
                canonical_map[canonical_word] = synonyms;
            }
        }
    }
};

// Класс для обработки текста
class TextProcessor {
private:
    // Ссылка на объект SynonymDictionary, который будет использоваться для обработки слов
    SynonymDictionary& dictionary;

public:
    // Конструктор класса, принимающий ссылку на SynonymDictionary, и инициализирующий поле dictionary
    TextProcessor(SynonymDictionary& dict) : dictionary(dict) {}

    // Аргументы:
    // input_filename - имя входного файла
    // output_filename - имя выходного файла
    // automatic_mode - режим автоматической обработки неизвестных слов
    // unknown_words - множество для хранения неизвестных слов
    void processFile(const std::string& input_filename, const std::string& output_filename, bool automatic_mode, std::unordered_set<std::string>& unknown_words) {
        std::ifstream input_file(input_filename);
        if (!input_file.is_open()) {
            // Если файл не удалось открыть, выбрасываем исключение с сообщением об ошибке
            throw std::runtime_error("Could not open input file: " + input_filename);
        }

        // Открытие выходного файла для записи
        std::ofstream output_file(output_filename);

        if (!output_file.is_open()) {
            // Закрытие входного файла перед выбросом исключения
            input_file.close();
            // Если файл не удалось открыть, выбрасываем исключение с сообщением об ошибке
            throw std::runtime_error("Could not open output file: " + output_filename);
        }

        std::string line;
        // Чтение файла построчно
        while (std::getline(input_file, line)) {
            // Создание потокового объекта для чтения строки
            std::istringstream line_stream(line);
            std::string word;
            // Чтение каждого слова из строки
            while (line_stream >> word) {
                // Получение канонического слова для текущего слова
                std::string canonical_word = dictionary.getCanonicalWord(word);
                // Если слово не найдено в словаре синонимов
                if (canonical_word == word && dictionary.getCanonicalWord(word) == word) {
                    // Добавление неизвестного слова в множество unknown_words
                    unknown_words.insert(word);
                    // Если режим автоматической обработки выключен
                    if (!automatic_mode) {
                        // Вывод сообщения о неизвестном слове
                        std::cout << "Word not in dictionary: " << word << std::endl;
                        char option;
                        // Запрос на добавление слова в словарь
                        std::cout << "Add to dictionary? (y/n): ";
                        std::cin >> option;
                        // Если пользователь согласен добавить слово в словарь
                        if (option == 'y' || option == 'Y') {
                            // Запрос на каноническую форму для данного слова
                            std::cout << "Enter canonical word for " << word << ": ";
                            std::string canon;
                            std::cin >> canon;
                            // Добавление неизвестного слова в словарь
                            dictionary.addSynonym(canon, word);
                        }
                    }
                }
                // Запись канонического слова в выходной файл
                output_file << dictionary.getCanonicalWord(word) << " ";
            }
            // Запись символа новой строки в выходной файл
            output_file << "\n";
        }
        input_file.close();
        output_file.close();
    }
};


// Класс для управления отменой действий
class UndoManager {
private:
    // Стек для хранения функций, которые будут вызываться для отмены действий
    std::stack<std::function<void()>> undo_stack;

public:
    // Метод для добавления действия отмены в стек
    void addUndoAction(const std::function<void()>& action) {
        undo_stack.push(action);
    }

    // Метод для отмены последних N действий
    void undoLastActions(int N) {
        // Циклично выполняем и удаляем с верхушки стека, пока не достигнем N или стек не станет пустым
        for (int i = 0; i < N && !undo_stack.empty(); ++i) {
            // Выполнение фунции отмены
            undo_stack.top()();
            // Удаление выполненной функции отмены из стека
            undo_stack.pop();
        }
    }
    // Метод для проверки, пуст ли стек
    bool isEmpty() const {
        return undo_stack.empty();
    }
};

// Функция для ввода синонима и добавления его в словарь
void inputSynonym(SynonymDictionary& dict, UndoManager& undoManager) {
    std::string canon_word;
    std::string synonym;

    // Запрос у пользователя ввода канонического слова и синонима
    std::cout << "Enter canonical word: ";
    std::cin >> canon_word;
    std::cout << "Enter synonym: ";
    std::cin >> synonym;

    // Сохранение состояния для Undo (удаление добавленного синонима при отмене)
    undoManager.addUndoAction([canon_word, synonym, &dict]() {
        dict.removeSynonym(canon_word, synonym);
    });
    // Добавление синонима в словарь
    dict.addSynonym(canon_word, synonym);
}

// Функция для добавления нового слова с списком синонимов в словарь
void addNewWord(SynonymDictionary& dict, UndoManager& undoManager) {
    std::string canon_word;
    std::vector<std::string> synonyms;
    std::string synonym;

    // Запрос у пользователя ввода канонического слова
    std::cout << "Enter canonical word: ";
    std::cin >> canon_word;

    // Запрос у пользователя ввода списка синонимов, окончание по слову 'end'
    std::cout << "Enter synonyms (type 'end' to finish): ";
    while (std::cin >> synonym && synonym != "end") {
        synonyms.push_back(synonym);
    }

    // Сохранение состояния для Undo (удаление добавленных синонимов при отмене)
    undoManager.addUndoAction([canon_word, synonyms, &dict]() {
        for (const auto& s : synonyms) {
            dict.removeSynonym(canon_word, s);
        }
    });

    // Добавление записи в словарь
    dict.addEntry(canon_word, synonyms);
}

// Функция для обработки текста
void processText(bool automatic_mode, SynonymDictionary& dict, UndoManager& undoManager) {
    std::string input_filename, output_filename;

    // Запрос у пользователя ввода имен входного и выходного файлов
    std::cout << "Enter input filename: ";
    std::cin >> input_filename;
    std::cout << "Enter output filename: ";
    std::cin >> output_filename;

    // Создание экземпляра TextProcessor для обработки текста
    TextProcessor processor(dict);
    // Множество для хранения слов, которые не найдены в словаре
    std::unordered_set<std::string> unknown_words;

    // Обработка файла
    processor.processFile(input_filename, output_filename, automatic_mode, unknown_words);

    // Если автоматический режим отключен и есть неизвестные слова
    if (!automatic_mode && !unknown_words.empty()) {
        // Сообщаем пользователю о всех неизвестных словах
        std::cout << "Remaining unknown words: " << std::endl;
        for (const auto& word : unknown_words) {
            std::cout << word << std::endl;
        }
    }
}


int main(int argc, char* argv[]) {
    // Установка максимального количества действий для отмены. Если передан аргумент командной строки,
    // то используется его значение, иначе по умолчанию 10.
    const int MAX_UNDO = argc > 1 ? std::stoi(argv[1]) : 10;

    // Создание экземпляров класса словаря синонимов и менеджера отмены
    SynonymDictionary dict;
    UndoManager undoManager;

    // Попытка загрузки словаря синонимов из файла "synonyms.txt"
    try {
        dict.loadFromFile("synonyms.txt");
    } catch (const std::runtime_error& e) {
        // В случае ошибки загрузки вывести сообщение об ошибке и завершить программу
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    while (true) {
        std::cout << "1. Process text (automatic mode)" << std::endl;
        std::cout << "2. Process text (learning mode)" << std::endl;
        std::cout << "3. Add a new synonym" << std::endl;
        std::cout << "4. Add a new canonical word with synonyms" << std::endl;
        std::cout << "5. Undo last action" << std::endl;
        std::cout << "6. Save and exit" << std::endl;

        int choice;
        // Запрос у пользователя выбора опции
        std::cout << "Enter option: ";
        std::cin >> choice;

        try {
            // Выбор действия в зависимости от введенной опции
            switch (choice) {
                case 1:
                    // Обработка текста в автоматическом режиме
                    processText(true, dict, undoManager);
                    break;
                case 2:
                    // Обработка текста в режиме обучения
                    processText(false, dict, undoManager);
                    break;
                case 3:
                    // Добавление нового синонима
                    inputSynonym(dict, undoManager);
                    break;
                case 4:
                    // Добавление нового канонического слова с синонимами
                    addNewWord(dict, undoManager);
                    break;
                case 5: {
                    int N;
                    // Запрос у пользователя количества действий для отмены
                    std::cout << "Enter number of actions to undo: ";
                    std::cin >> N;
                    // Проверка, не превышает ли количество действий для отмены максимальное значение
                    if (N > MAX_UNDO) {
                        std::cout << "Cannot undo more than " << MAX_UNDO << " actions." << std::endl;
                        N = MAX_UNDO;
                    }
                    // Отмена последних N действий
                    undoManager.undoLastActions(N);
                    break;
                }
                case 6:
                    // Сохранение словаря в файл и завершение программы
                    dict.saveToFile("synonyms.txt");
                    return 0;
                default:
                    // Обработка неверного ввода опции
                    std::cout << "Unknown option! Please try again." << std::endl;
                    break;
            }
        } catch (const std::runtime_error& e) {
            // Обработка исключений и вывод сообщения об ошибке
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
}




//  I felt so happy and content today!
//  The rabbit was very quick and fast.
//  He was sad and unhappy about the news.

//  happy{joyful, content, pleased}
//  sad{unhappy, sorrowful, dejected}
//  fast{quick, speedy, rapid}