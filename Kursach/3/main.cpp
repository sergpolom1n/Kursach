#include <iostream>
#include <string>
#include <vector>
#include <random>

// определение структуры Attributes
struct Attributes {
    int health = 0;
    int armor = 0;
    int intelligence = 0;
    int strength = 0;
    int agility = 0;
    int accuracy = 0;
    int luck = 0;
    int skill = 0;

    // Конструктор по умолчанию для инициализации всех атрибутов
    Attributes(int health = 0, int armor = 0, int intelligence = 0, int strength = 0, int agility = 0, int accuracy = 0, int luck = 0, int skill = 0) : health(health), armor(armor), intelligence(intelligence), strength(strength), agility(agility), accuracy(accuracy), luck(luck), skill(skill) {};

    void printInfo() const {
        std::cout << "Health: " << health << std::endl;
        std::cout << "Armor: " << armor << std::endl;
        std::cout << "Intelligence: " << intelligence << std::endl;
        std::cout << "Strength: " << strength << std::endl;
        std::cout << "Agility: " << agility << std::endl;
        std::cout << "Accuracy: " << accuracy << std::endl;
        std::cout << "Luck: " << luck << std::endl;
        std::cout << "Skill: " << skill << std::endl;
    }

};

// определение класса Equipment
class Equipment {
public:
    std::string name;
    Attributes attributes;

    // Конструктор по умолчанию
    Equipment() {}

    // Конструктор с параметрами для инициализации имени и атрибутов
    Equipment(const std::string& name, const Attributes& attributes)
        : name(name), attributes(attributes) {}

    // Геттеры для получения значений атрибутов
    const std::string& getName() const {
        return name;
    }

    const Attributes& getAttributes() const {
        return attributes;
    }

    // Сеттеры для установки значений атрибутов
    void setName(const std::string& newName) {
        name = newName;
    }

    void setAttributes(const Attributes& newAttributes) {
        attributes = newAttributes;
    }

    // Метод для вывода информации об экипировке
    void printInfo() const {
        std::cout << "Equipment Name: " << name << std::endl;
        std::cout << "Health: " << attributes.health << std::endl;
        std::cout << "Armor: " << attributes.armor << std::endl;
        std::cout << "Intelligence: " << attributes.intelligence << std::endl;
        std::cout << "Strength: " << attributes.strength << std::endl;
        std::cout << "Agility: " << attributes.agility << std::endl;
        std::cout << "Accuracy: " << attributes.accuracy << std::endl;
        std::cout << "Luck: " << attributes.luck << std::endl;
        std::cout << "Skill: " << attributes.skill << std::endl;
    }
};

//определение базового класса персонажей
class Character {
public:
    std::string name;
    std::string characterClass;
    Attributes baseAttributes;
    std::vector<std::shared_ptr<Equipment>> equipment;

    // Чисто виртуальная функция для расчета урона
    virtual void calculateDamage() = 0;

    void addEquipment(std::shared_ptr<Equipment> eq) {
        equipment.push_back(eq);
    }

    // Подсчет общих характеристик с учетом экипировки
    Attributes getTotalAttributes() {
        Attributes total = baseAttributes;
        for (auto& eq : equipment) {
            total.health += eq->attributes.health;
            total.armor += eq->attributes.armor;
            total.intelligence += eq->attributes.intelligence;
            total.strength += eq->attributes.strength;
            total.agility += eq->attributes.agility;
            total.accuracy += eq->attributes.accuracy;
            total.luck += eq->attributes.luck;
            total.skill += eq->attributes.skill;
        }
        return total;
    };
};

// определение класса защитника
class Defender : public Character {
public:
    Defender(const std::string& name) {
        this->name = name;
        this->characterClass = "Defender";
        // Определение базовых атрибутов
        this->baseAttributes = Attributes(100, 50, 10, 20, 15, 5, 5, 5); // Примерные значения
    }

    void calculateDamage() override {
        // Получаем общие атрибуты с учетом экипировки
        Attributes totalAttributes = getTotalAttributes();

        // Предполагаем, что урон защитника зависит от его здоровья и брони
        int damage = totalAttributes.health / 10 + totalAttributes.armor / 5;
        damage += damage * totalAttributes.skill / 100; // Увеличение урона в зависимости от мастерства

        // Выводим расчетный урон
        std::cout << this->name << " (" << this->characterClass << ") deals " << damage << " damage." << std::endl;
    }
};

//определение класса бойца ближнего боя
class MeleeFighter : public Character {
public:
    MeleeFighter(const std::string& name) {
        this->name = name;
        this->characterClass = "Melee Fighter";
        // Устанавливаем базовые атрибуты для бойца ближнего боя
        this->baseAttributes = Attributes(80, 30, 10, 25, 20, 10, 5, 10);
    }

    void calculateDamage() override {
        // Получаем общие атрибуты с учетом экипировки
        Attributes totalAttributes = getTotalAttributes();

        // Урон -- комбинация силы и ловкости
        int damage = totalAttributes.strength * 2 + totalAttributes.agility;
        damage += damage * totalAttributes.accuracy / 100; // Увеличение урона в зависимости от меткости
        damage += damage * totalAttributes.skill / 100; // Увеличение урона от мастерства

        // Шанс критического удара от везения
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        if (dis(gen) <= totalAttributes.luck) {
            damage *= 2; // Критический удар удваивает урон
        }

        // Выводим расчетный урон
        std::cout << this->name << " (" << this->characterClass << ") deals " << damage << " damage." << std::endl;
    }
};

class RangedFighter : public Character {
public:
    RangedFighter(const std::string& name) {
        this->name = name;
        this->characterClass = "Ranged Fighter";
        // Устанавливаем базовые атрибуты для бойца дальнего боя
        this->baseAttributes = Attributes(70, 25, 30, 15, 25, 20, 10, 15);
    }

    void calculateDamage() override {
        // Получаем общие атрибуты с учетом экипировки
        Attributes totalAttributes = getTotalAttributes();

        // Урон бойца дальнего боя -- ум и ловкость
        int damage = totalAttributes.intelligence * 2 + totalAttributes.agility;
        damage += damage * totalAttributes.accuracy / 100; // Увеличение урона в зависимости от меткости
        damage += damage * totalAttributes.skill / 100; // Увеличение урона от мастерства

        // Шанс критического удара от везения
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        if (dis(gen) <= totalAttributes.luck) {
            damage *= 2; // Критический удар удваивает урон
        }

        // Выводим расчетный урон
        std::cout << this->name << " (" << this->characterClass << ") deals " << damage << " damage." << std::endl;
    }
};

std::shared_ptr<Equipment> generateRandomEquipment() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10);
    std::uniform_int_distribution<> distSecondary(1, 5); // Для вторичных атрибутов делаем диапазон поменьше

    Attributes attributes;
    attributes.health = dist(gen);
    attributes.armor = dist(gen);
    attributes.intelligence = dist(gen);
    attributes.strength = dist(gen);
    attributes.agility = dist(gen);
    attributes.accuracy = distSecondary(gen);
    attributes.luck = distSecondary(gen);
    attributes.skill = distSecondary(gen);

    std::string equipmentNames[3] = {"Random Helmet", "Mysterious Armor", "Ancient Boots"};
    std::uniform_int_distribution<> nameDist(0, 2);  // Индекс для выбора имени из списка
    std::string chosenName = equipmentNames[nameDist(gen)];

    return std::make_shared<Equipment>(chosenName, attributes);
}

int main() {
    // Создаем персонажей разных классов
    Defender defender("Arthur");
    MeleeFighter meleeFighter("Lancelot");
    RangedFighter rangedFighter("Robin");

    // Генерируем случайную экипировку
    auto equipment1 = generateRandomEquipment();
    auto equipment2 = generateRandomEquipment();
    auto equipment3 = generateRandomEquipment();

    // Добавляем экипировку персонажам
    defender.addEquipment(equipment1);
    meleeFighter.addEquipment(equipment2);
    rangedFighter.addEquipment(equipment3);

    // Демонстрируем расчет урона
    std::cout << "Demonstration of Damage Calculation:" << std::endl;
    defender.calculateDamage();
    meleeFighter.calculateDamage();
    rangedFighter.calculateDamage();

    // Демонстрация общих атрибутов персонажей с учетом экипировки
    std::cout << "\nTotal Attributes after Equipping:" << std::endl;
    std::cout << '\n' << defender.name << " used " << equipment1->name << ":\n";
    defender.getTotalAttributes().printInfo();
    std::cout << '\n' << meleeFighter.name << " used " << equipment2->name << ":\n";
    meleeFighter.getTotalAttributes().printInfo();
    std::cout << '\n' << rangedFighter.name << " used " << equipment3->name << ":\n";
    rangedFighter.getTotalAttributes().printInfo();

    return 0;
}


