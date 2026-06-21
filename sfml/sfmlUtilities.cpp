#include "shapesUtilities.h" // ООП, Класси фігур та кнопки разом із текстовими полями

// Універсальна функція для створення фігур, у якому використовується
// Назва фігури, RGB колір заповнення, RGB колір обведення, розмір об'єкту
unique_ptr<ClassShape> shapeFill(string shapeName, int fr, int fg, int fb, int sr, int sg, int sb, double sx, double sy){
    unique_ptr<ClassShape> shape;
    if (shapeName == "Circle"){ // Круг
        shape = make_unique<Circle>(fr, fg, fb, sr, sg, sb, sx, sy);
    }else if (shapeName == "Square"){ // Квадрат (може бути прямокутником, якщо ширина та висота не рівні)
        shape = make_unique<Square>(fr, fg, fb, sr, sg, sb, sx, sy);
    }else if (shapeName == "Diamond"){ // Ромб
        shape = make_unique<Diamond>(fr, fg, fb, sr, sg, sb, sx, sy);
    }else if (shapeName == "Oval"){ // Овал
        shape = make_unique<Oval>(fr, fg, fb, sr, sg, sb, sx, sy);
    }else if (shapeName == "Triangle"){ // Трикутник
        shape = make_unique<Triangle>(fr, fg, fb, sr, sg, sb, sx, sy);
    };
    // Повертаємо створену фігуру
    return shape;
};

// Функція налаштування перед запуском
// Використовується для обирання фігури, її конфігурації та розміри вікна
void configurationBeforeLaunch(){
    // Спочатку створюємо вікно налаштування
    RenderWindow window(VideoMode({750, 600}), "ObjeRot Configuation"); 
    
    // Додаємо та перевіряємо шрифт
    Font font;
    if (!font.openFromFile("pixel_font.ttf")) {
        return;
    }

    // Додаємо текст, яке буде позначати назву фігури
    Text shapeLabelTitle(font);
    shapeLabelTitle.setString(L"Фігура:");
    shapeLabelTitle.setCharacterSize(16);
    shapeLabelTitle.setFillColor(Color::Black);
    shapeLabelTitle.setPosition({20, 25});

    // Декілька кнопок з обиранням фігур
    vector<Button> shapeButtons;
    vector<string> shapeNames = {"Square", "Circle", "Oval", "Diamond", "Triangle"};
    // Якщо звертати увагу на String та L. String брали з SFML, бо той краще розуміє інші декодери.
    // Також L перед текстовими рядками ставимо, щоб кирилиця відображалася добре.
    vector<String> shapeDisplayNames = {L"Квадрат", L"Круг", L"Овал", L"Ромб", L"Трикутник"};
    // Стандартно обираємо круг
    string selectedShape = "Circle";
    // Цикл робимо для виставлення кольорів кнопок, щоб знати, який користувач обрав
    for (size_t i = 0; i < shapeNames.size(); ++i) {
        Color btnColor = (shapeNames[i] == selectedShape) ? Color::Blue : Color(150, 150, 150);
        shapeButtons.emplace_back(Vector2f(100, 30), Vector2f(100 + i * 110, 20), btnColor, font, shapeDisplayNames[i]);
    }

    // Створюємо текст (для відображення того, що воно обозначає) та текстові поля
    vector<Text> labels;
    vector<TextBox> inputs;
    
    // Список з текстами (з використанням функції SFML для UTF-8)
    vector<String> labelNames = {
        L"Fill R (0-255):", L"Fill G (0-255):", L"Fill B (0-255):",
        L"Stroke R (0-255):", L"Stroke G (0-255):", L"Stroke B (0-255):",
        L"Розмір X:", L"Розмір Y:", 
        L"Ширина вікна:", L"Висота вікна:"
    };

    // Цикл створення тексту
    for (int i = 0; i < 10; ++i) {
        Text label(font);
        label.setString(labelNames[i]);
        label.setCharacterSize(16);
        label.setFillColor(Color::Black);
        label.setPosition({20, 75 + i * 40});
        labels.push_back(label);
        inputs.emplace_back(font, 250, 30, 20);
        inputs.back().setPosition({280, 70 + i * 40});
    }

    // Кнопка запуску
    Button startButton({150, 50}, {300, 480}, Color::Green, font, L"Запуск");

    while (window.isOpen()){
        while (const optional event = window.pollEvent()) {
            // Для закриття програми
            if (event->is<Event::Closed>()) {
                window.close();
                return; 
            }

            // Якщо користувач натискав на мишкою
            if (const auto* mouseClick = event->getIf<Event::MouseButtonPressed>()) {
                // Якщо користував обрав ліву кнопку миші
                if (mouseClick->button == Mouse::Button::Left) {
                    // Переклад з піксельних на світових координат миші
                    Vector2f mousePos = window.mapPixelToCoords(mouseClick->position);
                    
                    // Перевірка того, чи натискав користувач на текстове поле
                    for (auto& box : inputs) { box.setSelected(box.contains(mousePos)); }

                    // Перевірка того, чи натискав користувач на кнопки
                    for (size_t i = 0; i < shapeButtons.size(); ++i) {
                        // Перевірка, чи стикається глобальні координат кнопок з мишкою
                        if (shapeButtons[i].shape.getGlobalBounds().contains(mousePos)) {
                            selectedShape = shapeNames[i];
                            // У випадок з обиранням фігур змінюємо колір кнопки
                            for (size_t j = 0; j < shapeButtons.size(); ++j) {
                                shapeButtons[j].shape.setFillColor((shapeNames[j] == selectedShape) ? Color::Blue : Color(150, 150, 150));
                            }
                        }
                    }
                    // Перевірка, чи стикається глобальні координат кнопки запуску з мишкою
                    if (startButton.shape.getGlobalBounds().contains(mousePos)) {
                        try {
                            // Перевірка того, що користувач написав у текстових полях
                            // Якщо користувач нічого не написав, обираються стандартні варіанти
                            string shape = selectedShape;
                            
                            // RGB Колір заповнення
                            int fr = inputs[0].inputString.isEmpty() ? 255 : stoi(inputs[0].inputString.toAnsiString());
                            int fg = inputs[1].inputString.isEmpty() ? 0   : stoi(inputs[1].inputString.toAnsiString());
                            int fb = inputs[2].inputString.isEmpty() ? 0   : stoi(inputs[2].inputString.toAnsiString());

                            // RGB Колір обведення
                            int sr = inputs[3].inputString.isEmpty() ? 255 : stoi(inputs[3].inputString.toAnsiString());
                            int sg = inputs[4].inputString.isEmpty() ? 255 : stoi(inputs[4].inputString.toAnsiString());
                            int sb = inputs[5].inputString.isEmpty() ? 255 : stoi(inputs[5].inputString.toAnsiString());

                            // Розмір об'єкту
                            double sx = inputs[6].inputString.isEmpty() ? 100.0 : stod(inputs[6].inputString.toAnsiString());
                            double sy = inputs[7].inputString.isEmpty() ? 100.0 : stod(inputs[7].inputString.toAnsiString());

                            // Розмір вікна
                            double swx = inputs[8].inputString.isEmpty() ? 1280.0 : stod(inputs[8].inputString.toAnsiString());
                            double swy = inputs[9].inputString.isEmpty() ? 720.0 : stod(inputs[9].inputString.toAnsiString());

                            // Закриваємо це вікно та запускаємо саму робочу програму
                            window.close(); 
                            programRunning(shape, fr, fg, fb, sr, sg, sb, sx, sy, swx, swy);
                            return; 
                            
                        } catch (const out_of_range&){ // Якщо розмір тексту перевищено допустимий індекс
                            cerr << "Помилка: багато чисел!" << endl;
                        }
                    }
                }
            }

            // Функція написання текстових полей
            for (auto& box : inputs) {
                box.processEvent(*event);
            }
        }

        // Очищаємо вікно та заповнюємо фон білим кольорем
        window.clear(Color::White);
        
        // Додаємо усі наши елементи (текст, кнопки, текстові поля)
        window.draw(shapeLabelTitle);
        for (auto& btn : shapeButtons) { btn.draw(window); }
        for (auto& label : labels) { window.draw(label); }
        for (auto& box : inputs) { box.draw(window); }
        startButton.draw(window);
        
        // Показуємо вікно
        window.display();
    };
};

// Головна функція після налаштувань
void programRunning(string shapeName, int fr, int fg, int fb, int sr, int sg, int sb, double sx, double sy, int swx, int swy){
    // Створюємо вікно с цьома розмірами, які обрали у налаштуваннях
    RenderWindow window(VideoMode({swx, swy}), "ObjeRot 1.0");
    // Ставимо ліміт 60 ФПС, щоб об'єкти рівномірно пересувалися
    window.setFramerateLimit(60);

    // Створюємо саму фігуру. Для цього беремо дані з аргументу, які у свою чергу беруться з налаштування
    unique_ptr<ClassShape> shape = shapeFill(shapeName, fr, fg, fb, sr, sg, sb, sx, sy);

    // Коли вікноо працює
    while (window.isOpen()) {    
        // Перевірка закриття програми
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }
        // Якщо є фігура
        if (shape) {
            // Швидкість фігури
            double speed = 5;
            // Отримуємо позицію фігури для перевірки кордон
            Position position = shape->getPos();
            // Користувач зажав стрілку зліва
            if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
                if (position.x > 0){
                    shape->move(-speed, 0);
                }
            }
            // Користувач зажав стрілку справа
            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
                if (position.x + sx < swx){
                    shape->move(speed, 0);
                }
            }
            // Користувач зажав стрілку верх
            if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
                if (position.y > 0){
                    shape->move(0, -speed);
                }
            }
            // Користувач зажав стрілку знизу
            if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
                if (position.y + sy < swy){
                    shape->move(0, speed);
                }
            }
        }
        // Фон чорного кольору
        window.clear(Color::Black);

        // Якщо є фігура, додаємо до вікна
        if (shape) {
            shape->draw(window);
        }
        
        // Показуємо вікно
        window.display();
    }
}