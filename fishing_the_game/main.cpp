#include <windows.h> // Бібліотека для нормального відображення кирилиці
#include "gameUtils.h" // ООП та абстракції разом із функціями від ядра гри.

using namespace std; // Використовуємо станд. бібліотеки без префікса std

// Головна функція гри
int main(){
    // Кодування UTF-8 для нормального відображення кирилиці
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    // Конструктор гравця, риби, локації та приманки без параметрів для подальшого створення 
    Player player;
    FishBaitLocationMap locationMap;
    // Починаємо гру зі створення гравці
    BeginGame(player, locationMap);
    // Змінні для вибору режимів гри
    bool gamemodeTime = false;
    int countFishes = 0;
    // Гравець обирає режим гри, а саме:
    // 1. Гра до певної кількості спійманої риби
    // 2. Гра на час (хвилина коштує 10 монет, заплачено 30 монет, через 3 хвилини гра завершиться з можливістю продовжити або вийти)
    ChooseGamemode(gamemodeTime, countFishes);
    // Гра починається у тому режимі, який обрав гравець.
    GameRunning(player, locationMap, gamemodeTime, countFishes); 
}