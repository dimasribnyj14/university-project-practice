#ifndef SHAPES_H // Перевірка якщо константа SHAPES_H ще не визначена
#define SHAPES_H // Визначаємо цю константу, щоб наступні включення файлу ігнорувались

#include <SFML/Graphics.hpp> // Головна бібліотека для 2D графіки
#include <string> // Використовується для тексту (хоча є String у SFML)
#include <optional> // Для події SFML 3
#include <iostream> // Для виводу у консоль (у нашому випадку, лише виводу помилок)
#include <memory> // Для управління об'єктів з пам'яті

// Скорочення без цих префіксів (sf з sfml)
using namespace sf;
using namespace std;

// Для отримання позиції об'єктів
struct Position {
    double x;
    double y;
};

// Головний клас фігур
class ClassShape{
    protected: // Обрано замість private для наслідування класів
        // RGB Колір для заповнення
        int fill_r, fill_g, fill_b;
        // RGB Колір для обведення
        int stroke_r, stroke_g, stroke_b;
        // Розмір об'єкта
        double size_x, size_y;
        // Позиція об'єкта (при створенні вказуємо початковий)
        double pos_x, pos_y;
    public:
        // Конструктор з параметрами
        ClassShape(int fr, int fg, int fb, int sr, int sg, int sb, double sx, double sy)
        : fill_r(fr), fill_g(fg), fill_b(fb), stroke_r(sr), stroke_g(sg), stroke_b(sb), size_x(sx), size_y(sy) {}

        // Функція відображення об'экту на екран (самий функціонал є у наслідування класів)
        // До функції додаємо вікно, щоб змогли відобразити самий об'єкт
        virtual void draw(RenderWindow& window) = 0;
        // Функція отримання позицій для перевірки кордону
        virtual Position getPos() const = 0;

        // Функція зміни позиції об'єкта (пересування)
        void move(double dx, double dy) {
            pos_x += dx;
            pos_y += dy;
        }
        
        virtual ~ClassShape() = default;
};

// Клас круги, 
class Circle : public ClassShape {
    public:
        // Для створення використовуємо конструктор з батьківського класа
        using ClassShape::ClassShape;

        // Сама функція додавання об'єкту до екрану
        void draw(RenderWindow& window) override {
            // Колір обведення
            Color strokeColor(stroke_r, stroke_g, stroke_b);
            // Колір заповнення
            Color fillColor(fill_r, fill_g, fill_b);
            // Створення круга
            CircleShape circle;
            // Радіус круга (використовуємо для цього size_x та ділимо на 2. size_y не використовується)
            circle.setRadius(size_x / 2);
            // Додаємо колір обведення до круга
            circle.setOutlineColor(strokeColor);
            // Ширина обведення (обираємо стандартну, бо користувач не може обирати це значення)
            // Щодо самого значення -5. 
            // Пам'ятаємо з CSS або хоча б Figma, є зовнішнє обведення, якого більше нуля.
            // Однак є обведення всередині об'єкта, якого менше нуля. Ми обрали обведення всередині
            // Бо таким чином буде добре працювати вигляд перевірки кордону
            circle.setOutlineThickness(-5);
            // Додаємо колір заповнення самого круга
            circle.setFillColor(fillColor);
            // Обираємо позицію для створення круга (стандартно - 0)
            circle.setPosition({pos_x, pos_y});
            // Малюємо самий об'єкт до екрану
            window.draw(circle);
        }
        // Функція отримання позиції. Потрібно для перевірки кордону.
        Position getPos() const override {
            return {pos_x, pos_y};
        };
// Далі повторювати однакові функції, як вважаю, не потрібно (окрім деяких деталей).
};

class Triangle : public ClassShape{
    public:
        using ClassShape::ClassShape;

        void draw(RenderWindow& window) override {
            Color strokeColor(stroke_r, stroke_g, stroke_b);
            Color fillColor(fill_r, fill_g, fill_b);
            // Для створення трикутника додаємо 3 точки за допомогою setPointCount
            ConvexShape triangle;
            triangle.setPointCount(3);
            // Vector2f використовуємо для визначення точних координат
            triangle.setPoint(0, Vector2f(size_x / 2, 0));
            triangle.setPoint(1, Vector2f(0, size_y));
            triangle.setPoint(2, Vector2f(size_x, size_y));
            triangle.setFillColor(fillColor);
            triangle.setOutlineColor(strokeColor);
            triangle.setOutlineThickness(-5);
            triangle.setPosition({pos_x, pos_y});
            
            window.draw(triangle);
        }

        Position getPos() const override {
            return {pos_x, pos_y};
        };
};

class Square : public ClassShape{
    public:
        using ClassShape::ClassShape;

        void draw(RenderWindow& window) override {
            Color strokeColor(stroke_r, stroke_g, stroke_b);
            Color fillColor(fill_r, fill_g, fill_b);

            RectangleShape square(Vector2f(size_x, size_y));
            square.setFillColor(fillColor);
            square.setOutlineColor(strokeColor);
            square.setOutlineThickness(-5);
            square.setPosition({pos_x, pos_y});

            window.draw(square);
        }

        Position getPos() const override {
            return {pos_x, pos_y};
        };
};

class Oval : public ClassShape{
    public:
        using ClassShape::ClassShape;

        void draw(RenderWindow& window) override {
            Color strokeColor(stroke_r, stroke_g, stroke_b);
            Color fillColor(fill_r, fill_g, fill_b);
            
            // Для створення овалу використовуємо функцію створення кругів, змінюючи радіус та розміри
            CircleShape oval(1);
            oval.setScale(Vector2f(size_x / 2, size_y / 2));
            oval.setFillColor(fillColor);
            oval.setOutlineColor(strokeColor);
            oval.setOutlineThickness(-5);
            oval.setPosition({pos_x, pos_y});

            window.draw(oval);
        }

        Position getPos() const override {
            return {pos_x, pos_y};
        };
};

class Diamond : public ClassShape{
    public:
        using ClassShape::ClassShape;

        void draw(RenderWindow& window) override {
            Color strokeColor(stroke_r, stroke_g, stroke_b);
            Color fillColor(fill_r, fill_g, fill_b);

            ConvexShape diamond;
            diamond.setPointCount(4);
            diamond.setPoint(0, Vector2f(size_x / 2, 0));
            diamond.setPoint(1, Vector2f(size_x, size_y / 2));
            diamond.setPoint(2, Vector2f(size_x / 2, size_y));
            diamond.setPoint(3, Vector2f(0, size_y / 2));
            diamond.setFillColor(fillColor);
            diamond.setOutlineColor(strokeColor);
            diamond.setOutlineThickness(-5);
            diamond.setPosition({pos_x, pos_y});

            window.draw(diamond);
        }
        Position getPos() const override {
            return {pos_x, pos_y};
        };
};

// Запуск програми налаштувань для main.cpp
void configurationBeforeLaunch();
// Запуск самої програми після налаштування, потрібно для sfmlUtilies.cpp
// Використовується назва фігури, rgb колір заповнення, rgb колір обведення, розмір фігури, розмір вікна
void programRunning(string shape, int fr, int fg, int fb, int sr, int sg, int sb, double sx, double sy, int swx, int swy);

// Клас кнопки. Потрібні для натискання.
// Використовуються для запуску та обирання фігурів.
struct Button {
    // Для цього використовуємо текст та прямокутник (прямокутник - відображення самої кнопки, текст - відображення того, що кнопка робить)
    RectangleShape shape;
    Text text;

    // Конструктор кнопки з такими параметрами:
    // Розмір, позиція, колір, шрифт, заголовок
    Button(Vector2f size, Vector2f position, Color color, const Font& font, const String& label) : text(font) {
        // Налаштування для прямокутника
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(color);
        
        // Налаштування для тексту
        text.setString(label);
        // Розмір шрифта ставимо 20
        text.setCharacterSize(20);
        // Колір білий обираємо
        text.setFillColor(Color::White);
        // Для центрування тексту використовуємо власні координати тексту та 
        // центруємо за допомогою ділень на 2 (половини від розміру та позиції). Далі йде функція локального центру переміщень
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2, textRect.position.y + textRect.size.y / 2});
        text.setPosition({position.x + size.x / 2, position.y + size.y / 2});
    }

    // Перевіряємо якщо користувач навів мишкою на кнопку за допомогою перевірки координат миші та переведень координат з піксельних на світових.
    bool isHovered(const RenderWindow& window) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        // Перевірка результату того, що навівся мишка на кнопку або ні. 
        // Це робиться за допомогою глобального координати (тобто, координати у вікні)
        return shape.getGlobalBounds().contains(mousePos);
    }

    // Малюємо самі прямокутники та текст у вікно
    void draw(RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};

// Клас текстового поля. Потрібні для написання значень.
// Використовуються для RGB кольору та розмірів.
struct TextBox {
    // Використовуються прямокутники, текст та текстові рядки. Також перевірка на то, чи натискав користувач.
    RectangleShape box;
    Text text;
    String inputString;
    bool isSelected = false;

    // Конструктор створення текстового поля.
    // Використовується шрифт, розміри (ширина, висота), розмір шрифта
    TextBox(const Font& font, int size_x, int size_y, int charSize) : text(font) {
        // Налаштування прямокутника (спочатку розміри)
        box.setSize({size_x, size_y});
        // Білий колір для заповнення
        box.setFillColor(Color::White);
        // Чорний колір для обведення
        box.setOutlineColor(Color::Black);
        // Ширина обведення
        box.setOutlineThickness(2);
        // Налаштування тексту (спочатку розмір шрифта)
        text.setCharacterSize(charSize);
        text.setFillColor(Color::Black);
        // Позиція тексту ставимо там те звичайно знаходятся у текстових полях
        text.setPosition({box.getPosition().x + 5, box.getPosition().y + 5});
        // Спочатку пишемо ..., щоб показати, що поля порожні
        text.setString("...");
    }

    // Ставимо позицію текстового поля (також з додаванням 5 у тексту для точності)
    void setPosition(Vector2f pos) {
        box.setPosition(pos);
        text.setPosition({pos.x + 5, pos.y + 5});
    }

    // Перевірка, щоб користувач навів мишку на текстове поле
    bool contains(Vector2f mousePos) const {
        return box.getGlobalBounds().contains(mousePos);
    }

    // Якщо користувач обрав, натиснувши лівою кнопкою миші, на текстове поле
    // АБО якщо користувач прибрав з поля (обиравши іншу)
    void setSelected(bool sel) {
        // Ставимо значення обраного поля
        isSelected = sel;
        if (isSelected) {
            // Якщо користувач обрав, то показуємо це користувачу візуально
            box.setOutlineColor(Color::Red);
            box.setOutlineThickness(4);

            text.setString(inputString + "|"); 
        } else {
            // Якщо користувач прибрав, повертаємо визуально, як було
            box.setOutlineColor(Color::Black);
            box.setOutlineThickness(2);
            // ОКРІМ цього випадку. Якщо текст не порожній, залишаємо як є
            if (inputString.isEmpty()) {
                text.setString("..."); 
            } else {
                text.setString(inputString);
            }
        }
    }

    // Користувач натискає щось клавіатурою (виникає подія)
    void processEvent(const Event& event) {
        // Якщо користував не обирав дане поле, виходимо
        if (!isSelected) return;
        // Перевіряємо якщо користувач спробував написати текст
        if (const auto* textEntered = event.getIf<Event::TextEntered>()) {
            // Перевіряємо, щоб користувач написав лише цифрою
            // Бо у нас текстові поля використовується лише для числових значень
            if (isdigit(textEntered->unicode)) {
                // Додаємо усе написане користувача до поля, конвертуючи з unicode на число
                inputString += static_cast<char32_t>(textEntered->unicode);
                
                // Показуємо це візуально користувачу та змінюємо значення.
                String displayString = inputString;
                displayString += "|"; 
                text.setString(displayString);
            }
        }
        // Якщо користував спробував не писати текст
        else if (const auto* keyPressed = event.getIf<Event::KeyPressed>()) {
            // Якщо користувач обрав Backspace та текст не порожній
            if (keyPressed->code == Keyboard::Key::Backspace && !inputString.isEmpty()) {
                // Видаляємо останнє число
                inputString.erase(inputString.getSize() - 1);
                
                String displayString = inputString;
                displayString += "|";
                text.setString(displayString);
            }
        }
    }

    // Додаємо до вікна текстове поле
    void draw(RenderWindow& window) {
        window.draw(box);
        window.draw(text);
    }
};


#endif