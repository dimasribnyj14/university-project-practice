#ifndef GAMEUTILS_H // Перевірка якщо константа GAMEUTILS_H ще не визначена
#define GAMEUTILS_H // Визначаємо цю константу, щоб наступні включення файлу ігнорувались

#include <iostream> 
#include <limits>
#include <memory>
#include <vector>
#include <string> // Робота з текстовими рядками
#include <cstdlib>
#include <unordered_map> // Робота з словарями
#include <chrono>
#include <fstream>

using namespace std; // Використовуємо станд. бібліотеки без префікса std

// Клас локації (або водойми), у якому захищено назва (окрім від класів-наслідування)
class Location {
    protected:
        string name;
    public:
        // Конструктор з параметром назва
        Location(string name) : name(name) {}
        // Отримуємо назву локації
        string getName() const { return name; }
        virtual void getInfo() const {
            cerr << "Локація: " << name << endl;
        }
};

// Клас озера, яка наслідує клас локації
class Lake : public Location {
    public:
        // Отримуємо назву локації
        Lake(string name) : Location(name) {}
        void getInfo() const override {
            cerr << "Локація: " << name << " (Озеро)" << endl;
        }
};
// Клас річки, яка наслідує клас локації
class River : public Location {
    public:
        // Отримуємо назву локації
        River(string name) : Location(name) {}
        string getName() const { return name; }
        void getInfo() const override {
            cerr << "Локація: " << name << " (Річка)" << endl;
        }
};

// Клас приманки раніше додаємо, щоб клас Fish спрацювала до створення самого класа приманки
class Bait;
// Класс риби, яка буде наслідуватись
class Fish {
    protected:
        string name; // Назва риби
        int minWeight; // Мінімальна вага
        int maxWeight; // Максимальна вага
        float chanceToCatch; // Шанс спіймати (серед інших риб)
        float escapeChance; // Шанс зірватись (після спіймання)
    public:
        // Конструктор риби з параметрами
        Fish(string name, int minWeight, int maxWeight, float chanceToCatch, float escapeChance);
        virtual ~Fish() = default;
        // Отримати значення параметр
        string getName() const;
        int getMinWeight() const;
        int getMaxWeight() const;
        float getChanceToCatch() const;
        float getEscapeChance() const;
        // Додаємо рибу до списку конкретного приманку
        void addFishToBait(class Bait& bait);
        // Додаємо рибу до списку конкретного водойму
        void addFishToLocation(class Location& location);
        virtual void getInfo() const = 0;
};

// Клас мирної риби, яка наслідує класу Fish
class PeacefulFish : public Fish {
    public:
        // Конструктор з параметрами від класу Fish
        PeacefulFish(string name, int minWeight, int maxWeight, float chanceToCatch, float escapeChance)
        : Fish(name, minWeight, maxWeight, chanceToCatch, escapeChance) {}
        // Функція отримання інформації щодо риби
        void getInfo() const override {
            cerr << "Риба: " << name << " (Мирна)"
            << "Вага: від " << minWeight
            << " до " << maxWeight 
            << " кг, Шанс зловити: " << chanceToCatch * 100 << "%"
            << " Шанс зірватися: " << escapeChance * 100 << "%" << endl;
        }
};

// Клас хижої риби, яка наслідує класу Fish
class HunterFish : public Fish {
    public:
        // Конструктор з параметрами від класу Fish
        HunterFish(string name, int minWeight, int maxWeight, float chanceToCatch, float escapeChance)
        : Fish(name, minWeight, maxWeight, chanceToCatch, escapeChance) {}
        // Функція отримання інформації щодо риби
        void getInfo() const override {
            cerr << "Риба: " << name << " (Хижак)"
            << "Вага: від " << minWeight
            << " до " << maxWeight 
            << " кг, Шанс зловити: " << chanceToCatch * 100 << "%"
            << " Шанс зірватися: " << escapeChance * 100 << "%" << endl;
        }
};

// Клас приманки, яка може приймати у собі список риби
class Bait {
    public:
        string name;
        vector<shared_ptr<Fish>> fishes;
        vector<shared_ptr<Fish>> getFishes() const;
};

// Клас гравця
class Player {
    private:
        string name; // Ім'я гравця
        shared_ptr<Location> currentLocation; // Локація, у якому знаходиться гравець
        string currentBait; // Приманка, який обрав гравець
        int coins; // Кількість монет у гравця (для режиму гри на час)
        bool isFishing; // Чи закинув гравець вудку?
        vector<string> inventory; // Інвентар гравця, у якому будуть лежати риба
    public:
        // Конструктор без параметрів
        Player();
        // Конструктор з параметрами
        Player(const string name, shared_ptr<Location> currentLocation, string currentBait, int coins);
        // Отримуємо інформацію про гравця
        string getName() const;
        shared_ptr<Location> getCurrentLocation() const;
        string getCurrentBait() const;
        int getCoins() const;
        bool getFishing() const;
        // Змінюємо значення у гравця (окрім ім'я гравця)
        void setCoins(int newCoins);
        void setFishing(bool isFishing);
        void setLocation(shared_ptr<Location> newLocation);
        void setBait(string newBait);
        // Додаємо рибу до інвентаря гравця
        void addToInventory(string item);
        // Отримуємо інформацію про інвентар гравця.
        void getInventory() const {
            cerr << "Інвентар гравця " << name << ":" << endl;
            if (inventory.empty()) {
                cerr << "Ваш інвентар порожній." << endl;
            } else {
                // Якщо є об'єкти з однаковими іменами, то у кінці пишемо кількість x
                unordered_map<string, int> itemCounts;
                for (string item : inventory) {
                    itemCounts[item]++;
                }
                for (auto pair : itemCounts) {
                    cerr << "- " << pair.second << "x " << pair.first << endl;
                }
            }
        }
};

// Словник для зберігання цих зв'язків: Локація, Приманка, Риба
using FishBaitLocationMap = unordered_map<string, unordered_map<string, Bait>>;

// Додаємо самі функції до main.cpp
void BeginGame(Player& player, FishBaitLocationMap& locationMap);
void ChooseGamemode(bool &gamemodeTime, int &countFishes);
void GameRunning(Player& player, FishBaitLocationMap& locationMap, bool gamemodeTime, int countFishes);

#endif // Перевірка з константою закінчується