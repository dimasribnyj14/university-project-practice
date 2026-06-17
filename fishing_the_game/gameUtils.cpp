#include "gameUtils.h" // ООП та абстракції
ifstream LocBaitFishTXT("./fish_data.txt"); // Шукаємо файл, у якому написано:
// Назва водойми : Назва приманки : Назва риби ; Мін. вага ; Макс. вага ; Шанс спіймати ; Шанс зірвати

// Отримати список риб за приманкою
vector<shared_ptr<Fish>> Bait::getFishes() const {
    return fishes;
}

// Конструктор створення риби з параметрами
Fish::Fish(string name, int minWeight, int maxWeight, float chanceToCatch, float escapeChance)
: name(name), minWeight(minWeight), maxWeight(maxWeight), chanceToCatch(chanceToCatch), escapeChance(escapeChance) {}

// Функції для отримання інформації риб
string Fish::getName() const { return name; }
int Fish::getMinWeight() const { return minWeight; }
int Fish::getMaxWeight() const { return maxWeight; }
float Fish::getChanceToCatch() const { return chanceToCatch; }
float Fish::getEscapeChance() const { return escapeChance; }

// Конструктор гравця зі стандартними параметрами
Player::Player() : coins(0), isFishing(false) {}
 
// Конструктор гравця з параметрами
Player::Player(string name, shared_ptr<Location> currentLocation, string currentBait, int coins)
: name(name), currentLocation(currentLocation), currentBait(currentBait), coins(coins), isFishing(false) {}

// Функції для отримання інформації гравця
string Player::getName() const { return name; }
shared_ptr<Location> Player::getCurrentLocation() const { return currentLocation; }
string Player::getCurrentBait() const { return currentBait; }
int Player::getCoins() const { return coins; }
bool Player::getFishing() const { return isFishing; }

// Функції для зміни даних гравця (окрім ім'я гравця)
void Player::setFishing(bool f) { isFishing = f; }
void Player::setLocation(shared_ptr<Location> l) { currentLocation = l; }
void Player::setBait(string b) { currentBait = b; }
void Player::setCoins(int c) { coins = c;}
// Додаємо рибу до інвентаря гравці
void Player::addToInventory(string i) { inventory.push_back(i); }

// Функція використання файлу fish_data.txt для створення локації, приманки та риб
FishBaitLocationMap createFishBaitLocationMap() {
    FishBaitLocationMap location;
    if (!LocBaitFishTXT.is_open()) {
        cerr << "Не вдалося відкрити файл з даними про рибу та приманку" << endl;
        exit(EXIT_FAILURE);
        return location;
    } else {
        // Рядок з файлу
        string line;
        while (getline(LocBaitFishTXT, line)) {
            // Хоча у файлі немає пустих рядків, все одно перевіряємо
            if (line.empty()) continue;
            // Шукаємо по перше локацію, а далі - приманку
            size_t locationEnd = line.find(':');
            size_t baitEnd = line.find(':', locationEnd + 1);
            
            if (locationEnd == string::npos || baitEnd == string::npos) {
                cerr << "Невірний формат рядка: " << line << endl;
                continue;
            }
            // Якщо формати рядка правильні, беремо назву локації, приманки та інформації про рибу
            string locationName = line.substr(0, locationEnd);
            string baitName = line.substr(locationEnd + 1, baitEnd - locationEnd - 1);
            string fishInfo = line.substr(baitEnd + 1);
            // Інформація про рибу шукаємо назви приманки
            size_t nameEnd = fishInfo.find(';');
            size_t minWeightEnd = fishInfo.find(';', nameEnd + 1);
            size_t maxWeightEnd = fishInfo.find(';', minWeightEnd + 1);
            size_t catchChanceEnd = fishInfo.find(';', maxWeightEnd + 1);

            if (nameEnd == string::npos || minWeightEnd == string::npos || maxWeightEnd == string::npos || catchChanceEnd == string::npos) {
                cerr << "Невірний формат даних про рибу: " << fishInfo << endl;
                continue;
            }

            // Якщо формати даних правильні, то створюємо нові змінні для конструкції риби з параметрами
            string name = fishInfo.substr(0, nameEnd);
            string minWeight = fishInfo.substr(nameEnd + 1, minWeightEnd - nameEnd - 1);
            string maxWeight = fishInfo.substr(minWeightEnd + 1, maxWeightEnd - minWeightEnd - 1);
            string chanceToCatch = fishInfo.substr(maxWeightEnd + 1, catchChanceEnd - maxWeightEnd - 1);
            string escapeChance = fishInfo.substr(catchChanceEnd + 1);

            shared_ptr<Fish> fish;

            // Таким чином перевіряємо чи мирна або хижна риба.
            if (name == "Щука" || name == "Окунь" || name == "Сом" || name == "Судак") {
                fish = make_shared<HunterFish>(name, stoi(minWeight), stoi(maxWeight), stof(chanceToCatch), stof(escapeChance));
            } else {
                fish = make_shared<PeacefulFish>(name, stoi(minWeight), stoi(maxWeight), stof(chanceToCatch), stof(escapeChance));
            }

            // Додаємо до списку приманок
            location[locationName][baitName].name = baitName;
            location[locationName][baitName].fishes.push_back(fish);
        }
    }
    
    // Після завершення операції, закриваємо файл та повертаємо інформацію про локацію, приманок та риби
    LocBaitFishTXT.close();
    return location;
}

// Функція витагування вудки з рибою (3)
bool Fishing(Player& player, FishBaitLocationMap& locationMap) {
    // Отримуємо інформацію з гравця та мапи, який гравець обрав приманку та які риби використовуються
    const Bait& bait = locationMap[player.getCurrentLocation()->getName()][player.getCurrentBait()];
    const vector<shared_ptr<Fish>>& fishes = bait.getFishes();

    // Цикл риб, у якому використовуємо auto для автоматичного виведення типу
    for (auto fish : fishes) {
        // Інформація про шанс спіймати та зірвати
        float catchChance = fish->getChanceToCatch();
        float escapeChance = fish->getEscapeChance();

        // Випадкова подія спіймання риби
        if (static_cast<float>(rand()) / RAND_MAX < catchChance) {
            // Випадкова вага риби за допомогою мінімальної та максимальної ваги
            int actualWeight = fish->getMinWeight() + rand() % (fish->getMaxWeight() - fish->getMinWeight() + 1);
            cerr << "Ви зловили " << fish->getName() << " вагою " << actualWeight << " кг!"
                    << " Ви отримали 1 монету!" << endl;
            // Додаємо монетку гравцю (особливо важливо для режиму гри на час)
            // Додаємо рибу до інвентаря гравця та повертаємо true
            player.setCoins(player.getCoins() + 1);
            player.addToInventory(fish->getName());
            return true;
        // Випадкова подія зірвання риби
        } else if (static_cast<float>(rand()) / RAND_MAX < escapeChance) {
            // Риба зірвалася, тому повертаємо false
            cerr << "Риба зірвалася!" << endl;
            return false;
        }
    }
    return false;
}

// Гравець закидує вудку (1)
void StartFishing(Player& player, FishBaitLocationMap& locationMap) {
    // Якщо закинуто, повертаємось.
    if (player.getFishing()) {
        cerr << "Ви вже закинули вудку. Зачекайте, поки риба клюне." << endl;
        return;
    }
    // Гравець закинув вудку, тому змінюємо на true
    player.setFishing(true);
    cerr << "Ви закинули вудку з приманкою " << player.getCurrentBait() << " у локації " << player.getCurrentLocation()->getName() << "." << endl;
}

// Отримуємо інформацію про обрану локацію, у якому є риби та приманку, риби якого люблять (6)
void getInfo(string locationName, const FishBaitLocationMap locationMap) {
    // Хоча таких випадків не мають бути, але перевірка є.
    if (locationMap.find(locationName) == locationMap.end()) {
        cerr << "Локацію " << locationName << " не знайдено." << endl;
        return;
    }
    // Сама інформація про локацію, а саме назва локації
    cerr << "Інформація про локацію: " << locationName << ":" << endl;
    // Цикл приманок з автоматичним типом, через якого можемо отримати список риб
    for (auto baitPair : locationMap.at(locationName)) {
        string baitName = baitPair.first;
        const Bait& baitData = baitPair.second;

        cerr << " На приманку " << baitName << " можна зловити:" << endl;
        
        // Цикл риб з приманки з автоматичним типом, а саме string, float, int
        for (auto fish : baitData.getFishes()) {
            fish->getInfo();
        }
    }
}

// Початок гри, у якому створюємо гравця
void BeginGame(Player& player, FishBaitLocationMap& locationMap) {
    cerr << "Вітаємо у гру риболовля!" << endl;
    cerr << "Правило гри - виберіть приманку та локацію для риболовлі. Успіх залежить від вашого вибору та удачі!" << endl;

    // Просимо гравцю назвати ім'я
    cerr << "Введіть ваше ім'я: " << endl;
    string playerName;
    cin >> playerName;

    // Створюємо саме локації, приманки та риби з файлу fish_data.txt
    locationMap = createFishBaitLocationMap();
    // Створюємо змінні списки з приманкою та локацією
    vector<string> baits;
    vector<string> locations;

    // Створюємо список локації щоб гравець обрав
    for (auto locationPair : locationMap) { 
        locations.push_back(locationPair.first);
    }
    cerr << "Виберіть локацію: " << endl;
    for (int i = 0; i < locations.size(); i++){
        cerr << i + 1 << ". " << locations[i] << endl;
    }
    int locationChoice;
    cin >> locationChoice;

    // Створюємо список приманок щоб гравець обрав
    for (auto baitPair : locationMap[locations[locationChoice - 1]]) {
        baits.push_back(baitPair.first);
    }
    cerr << "Виберіть приманку: " << endl;
    for (int i = 0; i < baits.size(); i++){
        cerr << i + 1 << ". " << baits[i] << endl;
    }
    int baitChoice;
    cin >> baitChoice;

    // Таким чином знаходимо саме ту локацію, який обрав гравець
    string locationName = locations[locationChoice - 1];
    shared_ptr<Location> locationPtr = make_shared<Location>(locationName);
    // Перевіряємо також формат рядків
    if (locationName.find("Озеро") != string::npos) {
        locationPtr = make_shared<Lake>(locationName);
    } else if (locationName.find("Річка") != string::npos) {
        locationPtr = make_shared<River>(locationName);
    }

    // Створюємо гравця, на початку вже є 30 монет для другого режиму
    player = Player(playerName, locationPtr, baits[baitChoice - 1], 30);

    cerr << "Гравець створений! Інформація про гравця " << playerName << ":" << endl;
    cerr << "Локація: " << player.getCurrentLocation()->getName() << endl;
    cerr << "Приманка: " << player.getCurrentBait() << endl;
    cerr << "Монети: " << player.getCoins() << endl;
}

// Функція, де обираємо режим гри після створення гравця
void ChooseGamemode(bool &gamemodeTime, int &countFishes){
    bool inConfigurations = true;

    cerr << "Оберіть режим гри:" << endl;
    cerr << "1. Гра до певної кількості спійманої риби" << endl;
    cerr << "2. Гра на час (хвилина коштує 10 монет, заплачено 30 монет, через 3 хвилини гра завершиться з можливістю продовжити або вийти)" << endl;
    int action;
    cin >> action;
    while (inConfigurations){
        switch (action){
            case 1:
                // Гра до певної кількості спійманої риби
                cerr << "Оберіть до якої кількості спіймати рибу:" << endl;
                cin >> countFishes;
                inConfigurations = false;
                break;
            case 2:
                // Гра на час (хвилина коштує 10 монет, заплачено 30 монет, через 3 хвилини гра завершиться з можливістю продовжити або вийти)
                gamemodeTime = true;
                inConfigurations = false;
                break;
            default:
                cerr << "Невірний вибір. Спробуйте ще раз." << endl;
                break;
        }
    }
}

// Головна функція для роботи гри
void GameRunning(Player& player, FishBaitLocationMap& locationMap, bool gamemodeTime, int countFishes) {
    // Гра працює до тих пір, поки працює змінна isRunning
    bool isRunning = true;
    // Кількість спійманих риб (для першого режиму)
    int caughtFishes = 0;
    // Секундомер та оплачуємо хвилини за монети (для другого режиму)
    auto startTime = chrono::steady_clock::now();
    int paidMinutes = 3;
    // Конвертуємо з хвилин на секунд
    int totalAllowedSeconds = paidMinutes * 60; 

    // Перевірка який гравець обрав режим.
    if (gamemodeTime && countFishes <= 0){
        cerr << "Обрано режим: Гра на час" << endl;
        // Знімаємо монети, щоб гравець грав 3 хвилини у режимі гра на час
        player.setCoins(player.getCoins() - 30);
    }else if(!gamemodeTime && countFishes > 0){
        cerr << "Обрано режим: Гра до певної кількості спійманої риби" << endl;
    }

    // Цикл while, щоб гра працювала
    while (isRunning) {
        // Якщо обрано режим гра на час
        if (gamemodeTime) {
            // Гра починається зі секундомера
            auto currentTime = chrono::steady_clock::now();
            auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
            // Якщо поточний час перевищує дозволений, пропонуємо докупити час
            if (elapsedSeconds >= totalAllowedSeconds) {
                cerr << "ЧАС ВИЙШОВ Оплачені 3 хвилини закінчилися!" << endl;
                cerr << "Бажаєте продовжити гру? (1 хвилина = 10 монет)" << endl;
                cerr << "1. Продовжити (Докупити час)" << endl;
                cerr << "2. Вийти з гри" << endl;
                
                int continueChoice;
                cin >> continueChoice;

                // Якщо гравець обирає докупити час
                if (continueChoice == 1) {
                    cerr << "Скільки хвилин бажаєте докупити? (1 хв = 10 монет): ";
                    int extraMinutes;
                    cin >> extraMinutes;

                    // Якщо у гравця є як мінімум 10 монет, час подовжено
                    if (player.getCoins() >= extraMinutes * 10){
                        totalAllowedSeconds = extraMinutes * 60;
                        startTime = chrono::steady_clock::now();
                        // Гравець витрачає монети, щоб подовжити час
                        player.setCoins(player.getCoins() - extraMinutes * 10);
                        cerr << "Час продовжено на " << extraMinutes << " хв. Продовжуємо риболовлю!" << endl;
                    } else {
                        // Якщо у гравця немає ту кількість монет, яку потрібно, закриваємо гру,
                        // але перед цим, даємо гравцю натиснути будь-яку клавішу, щоб вийти
                        cerr << "Некоректна кількість хвилин. Натисніть будь-яку клавішу, щоб вийти..." << endl;
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin.get();
                        isRunning = false;
                        return;
                    }
                } else {
                    // Якщо у гравця не бажає подовжити час монетами, закриваємо гру,
                    // але перед цим, даємо гравцю натиснути будь-яку клавішу, щоб вийти
                    cerr << "Вихід з гри! Натисніть будь-яку клавішу, щоб вийти..." << endl;
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    isRunning = false;
                    return;
                }
            } else {
                // Пишемо скільки залишилося часуу
                cerr << "Залишилося часу для ловлі: " << (totalAllowedSeconds - elapsedSeconds) << " сек." << endl;
            }
        }

        // У випадок коли гравець спіймав стільки риб, скільки хотів (для першого режиму), закриваємо гру
        if (!gamemodeTime && countFishes > 0 && caughtFishes >= countFishes) {
            cerr << "Ви успішно спіймали цільову кількість риби: " << countFishes << " шт.!" << endl;
            cerr << "Вихід з гри! Натисніть будь-яку клавішу, щоб вийти..." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            isRunning = false;
            return;
        }

        cerr << "У вас " << player.getCoins() << " монет." << endl;
        cerr << "Оберіть дію, " << player.getName() << ":" << endl;
        
        cerr << "1.	Закинути вудку." << endl;
        cerr << "2.	Змінити приманку." << endl;
        cerr << "3.	Виловити рибу." << endl;
        cerr << "4.	Залишити місце ловлі." << endl;
        cerr << "5.	Змінити локацію." << endl;
        cerr << "6.	Інформація про поточний улов." << endl;
        cerr << "7.	Інвентар." << endl;
        cerr << "0.	Вийти з гри." << endl;

        int action;
        cin >> action;
        
        // Гравець обирає дію для подальшої гри
        switch (action){
            case 1:
                // Гравець закидує вудку
                StartFishing(player, locationMap);
                break;
            case 2: {
                // Гравець змінює приманку, але перед цим перевіряємо, якщо гравець вже закинув вудку.
                if (player.getFishing()) {
                    cerr << "Ви вже закинули вудку. Зачекайте, поки риба клюне." << endl;
                    break;
                }
                vector<string> currentBaits;

                // Створюємо список приманок щоб гравець обрав
                for (auto baitPair : locationMap[player.getCurrentLocation()->getName()]) { 
                    currentBaits.push_back(baitPair.first);
                }
                cerr << "Виберіть приманку: " << endl;
                for (int i = 0; i < currentBaits.size(); i++){
                    cerr << i + 1 << ". " << currentBaits[i] << endl;
                }
                int baitChoice;
                cin >> baitChoice;
                string newBait = currentBaits[baitChoice - 1];
                player.setBait(newBait);
                break;
            }
            case 3:
                // Гравець виловлює рибу, якщо до цього закидував вудкуу
                if (player.getFishing()) {
                    player.setFishing(false);
                    cerr << "Ви виловили рибу!" << endl;
                    // Додаємо гравцю до списку спойманих риб для першого режиму.
                    // Однак якщо Fishing повертає false, не додаємо
                    if (Fishing(player, locationMap)) { caughtFishes++; }
                } else {
                    cerr << "Ви не закинули вудку. Спочатку закиньте вудку." << endl;
                }
                break;
            case 4:
                // Залишаємо місце ловлі, не виловивши рибу, якщо до цього закидував вудкуу
                if (player.getFishing()) {
                    player.setFishing(false);
                    cerr << "Ви залишили місце ловлі." << endl;
                } else {
                    cerr << "Ви не закинули вудку. Спочатку закиньте вудку." << endl;
                }
                break;
            case 5:{
                // Гравець змінює локацію, але перед цим перевіряємо, якщо гравець вже закинув вудку.
                if (player.getFishing()) {
                    cerr << "Ви вже закинули вудку. Зачекайте, поки риба клюне." << endl;
                    break;
                }
                vector<string> currentLocations;

                // Створюємо список локації щоб гравець обрав
                for (auto locationPair : locationMap) {
                    currentLocations.push_back(locationPair.first);
                }
                cerr << "Виберіть локацію: " << endl;
                for (int i = 0; i < currentLocations.size(); i++){
                    cerr << i + 1 << ". " << currentLocations[i] << endl;
                }
                int locationChoice;
                cin >> locationChoice;

                // Таким чином знаходимо саме ту локацію, який обрав гравець
                string locationName = currentLocations[locationChoice - 1];
                shared_ptr<Location> newLocation = make_shared<Location>(currentLocations[locationChoice - 1]);
                // Перевіряємо також формат рядків
                if (locationName.find("Озеро") != string::npos) {
                    newLocation = make_shared<Lake>(locationName);
                } else if (locationName.find("Річка") != string::npos) {
                    newLocation = make_shared<River>(locationName);
                }
                // Змінюємо значення локації гравця
                player.setLocation(newLocation);
                break;
            };
            case 6:
                // Отримуємо інформацію про обрану локацію, у якому є риби та приманку, риби якого люблять
                getInfo(player.getCurrentLocation()->getName(), locationMap);
                break;
            case 7:
                // Отримуємо інформацію про інвентар гравця, у якого є риби, окрім випадків, коли інвентар порожній
                player.getInventory();
                break;
            case 0:
                // Виходимо з гри, також додаємо гравцю можливість натиснути будь-яку клавішу, щоб вийти
                cerr << "Вихід з гри! Натисніть будь-яку клавішу, щоб вийти..." << endl;
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                isRunning = false;
                return;
            default:
                cerr << "Невірний вибір. Спробуйте ще раз." << endl;
                break;
            }
    }
}