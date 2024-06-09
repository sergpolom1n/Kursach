#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <exception>
#include <optional>


struct Message {
    std::string username;
    std::string time;
    std::string content;

    Message(const std::string& uname, const std::string& tm, const std::string& msg)
        : username(uname), time(tm), content(msg) {}

    Message& operator=(const Message& other) {
        if (this != &other) { // Защита от самоприсваивания
            username = other.username;
            time = other.time;
            content = other.content;
        }
        return *this;
    }

    bool operator<(const Message& other) const {
        return std::tie(time, username, content) < std::tie(other.time, other.username, other.content);
    }

    bool operator==(const Message& other) const {
        return username == other.username && time == other.time && content == other.content;
    }

    void print() const {
        std::cout << username << " " << time << ": " << content << std::endl;
    }

};

class MessageDatabase {
public:
    void addMessage(const std::shared_ptr<Message>& msgPtr) {
        messages.insert(msgPtr);
    }

    void removeMessage(const std::shared_ptr<Message>& msgPtr) {
        auto it = std::find_if(messages.begin(), messages.end(), [&](const std::shared_ptr<Message>& m) {
            return *m == *msgPtr;
        });

        if (it != messages.end()) {
            messages.erase(it);
        } else {
            std::cerr << "Message not found for removal.\n";
        }
    }

    void removeAllMessagesFromUser(const std::string& username) {
        auto it = messages.begin();
        while (it != messages.end()) {
            if ((*it)->username == username) {
                it = messages.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::optional<std::shared_ptr<Message>> findMessage(const std::string& username, const std::string& time, const std::string& content) const {
        for (const auto& msg : messages) {
            if (msg->username == username && msg->time == time && msg->content == content) {
                return msg;
            }
        }
        return std::nullopt;
    }

    void printAllMessagesFromUser(const std::string& username) const {
        for (const auto& message : messages) {
            if (message->username == username) {
                message->print();
            }
        }
    }

    void printMessagesFromUserInTimeRange(const std::string& username, const std::string& startTime, const std::string& endTime) const {
        for (const auto& message : messages) {
            if (message->username == username && message->time >= startTime && message->time <= endTime) {
                message->print();
            }
        }
    }

    void printMessagesInTimeRange(const std::string& startTime, const std::string& endTime) const {
        for (const auto& message : messages) {
            if (message->time >= startTime && message->time <= endTime) {
                message->print();
            }
        }
    }

private:
    std::set<std::shared_ptr<Message>> messages;
};

bool parseDateTime(const std::string& date, const std::string& time) {
    std::istringstream dateStream(date);
    std::istringstream timeStream(time);
    int year, month, day, hour, minute, second, millisecond;
    char dash, colon, dot;

    // Разбираем дату
    dateStream >> year >> dash >> month >> dash >> day;
    if (dateStream.fail() || month > 12 || day > 31 || dash != '-') return false;

    // Разбираем время
    timeStream >> hour >> colon >> minute >> colon >> second >> dot >> millisecond;
    if (timeStream.fail() || hour > 24 || minute > 60 || second > 60 || millisecond > 999 || dot != '.' || colon != ':') return false;

    return true;
}

std::optional<std::shared_ptr<Message>> parseMessageLine(const std::string& line) {
    std::istringstream ss(line);
    std::string username, date, time, content;

    if (!(ss >> username >> date >> time)) {
        throw std::invalid_argument("Failed to parse message line: " + line);
    }

    if (!parseDateTime(date, time)) {
        throw std::invalid_argument("Invalid date/time format.");
    }

    std::string dateTime = date + " " + time;



    ss.ignore(1);
    std::getline(ss, content);



    return std::make_shared<Message>(username, date + " " + time, content);
}

void loadMessagesFromFile(const std::string& filename, MessageDatabase& db) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        auto msgOpt = parseMessageLine(line);
        if (msgOpt) {
            db.addMessage(*msgOpt);
        }
    }
}

int main() {
    try {
        MessageDatabase db;

        loadMessagesFromFile("dialogs.txt", db);

        std::cout << "All messages from user Alice:\n";
        db.printAllMessagesFromUser("Alice");

        std::cout << "\nMessages from user Bob between 2023-10-07 10:00:00.000 and 2023-10-07 12:00:00.000:\n";
        db.printMessagesFromUserInTimeRange("Bob", "2023-10-07 10:00:00.000", "2023-10-07 12:00:00.000");

        std::cout << "\nAll messages between 2023-10-07 10:00:00.000 and 2023-10-07 12:00:00.000:\n";
        db.printMessagesInTimeRange("2023-10-07 10:00:00.000", "2023-10-07 12:00:00.000");

        std::cout << "\nRemoving a specific message from Alice:\n";
        auto msgOpt = db.findMessage("Alice", "2023-10-07 11:00:00.000:", "Hello Bob!");
        if (msgOpt) {
            db.removeMessage(*msgOpt);
        } else {
            std::cerr << "Message not found.\n";
        }

        std::cout << "\nAll messages after removing a specific message:\n";
        db.printMessagesInTimeRange("2023-10-07 10:00:00.000", "2023-10-07 12:00:00.000");

        std::cout << "\nRemoving all messages from Alice:\n";
        db.removeAllMessagesFromUser("Alice");

        std::cout << "\nAll messages after removing all messages from Alice:\n";
        db.printMessagesInTimeRange("2023-10-07 10:00:00.000", "2023-10-07 12:00:00.000");

    } catch (const std::invalid_argument& ia) {
        // Обработка исключений, связанных с недопустимым аргументом
        std::cerr << "Invalid argument: " << ia.what() << std::endl;
        return 2;
    } catch (const std::runtime_error& re) {
        // Обработка ошибок времени выполнения
        std::cerr << "Runtime error: " << re.what() << std::endl;
        return 3;
    } catch (const std::ios_base::failure& iof) {
        // Обработка ошибок ввода/вывода
        std::cerr << "IO failure: " << iof.what() << std::endl;
        return 4;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;

    }
    return 0;
}










