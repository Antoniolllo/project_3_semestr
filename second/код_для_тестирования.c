#include <Arduino.h>

// Конфигурация светодиода
#define EXTERNAL_LED_PIN 2  // Внешний светодиод на пине D2
#define BUILTIN_LED_PIN 13  // Встроенный светодиод (для индикации)

// Тайминги для азбуки Морзе (в миллисекундах)
#define DOT_DURATION 200
#define DASH_DURATION 600
#define SYMBOL_GAP 200
#define LETTER_GAP 600
#define WORD_GAP 1400

// Структура для символа Морзе
typedef struct {
    char character;
    const char *morse_code;
} morse_char_t;

// Таблица преобразования символов в азбуку Морзе
static const morse_char_t morse_table[] = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},  {'D', "-.."},
    {'E', "."},     {'F', "..-."},  {'G', "--."},   {'H', "...."},
    {'I', ".."},    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},   {'P', ".--."},
    {'Q', "--.-"},  {'R', ".-."},   {'S', "..."},   {'T', "-"},
    {'U', "..-"},   {'V', "...-"},  {'W', ".--"},   {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},
    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
    {'8', "---.."}, {'9', "----."},
    {' ', " "},     // Пробел для разделения слов
};

#define MORSE_TABLE_SIZE (sizeof(morse_table) / sizeof(morse_char_t))

const char* char_to_morse(char c) {
    c = toupper(c);
    
    for (int i = 0; i < MORSE_TABLE_SIZE; i++) {
        if (morse_table[i].character == c) {
            return morse_table[i].morse_code;
        }
    }
    
    return NULL;
}

void blink_dot() {
    digitalWrite(EXTERNAL_LED_PIN, HIGH);
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay(DOT_DURATION);
    digitalWrite(EXTERNAL_LED_PIN, LOW);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay(SYMBOL_GAP);
}

void blink_dash() {
    digitalWrite(EXTERNAL_LED_PIN, HIGH);
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay(DASH_DURATION);
    digitalWrite(EXTERNAL_LED_PIN, LOW);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay(SYMBOL_GAP);
}

void blink_morse_sequence(const char *sequence) {
    if (sequence == NULL) return;
    
    for (int i = 0; sequence[i] != '\0'; i++) {
        switch (sequence[i]) {
            case '.':
                blink_dot();
                break;
            case '-':
                blink_dash();
                break;
            case ' ':
                delay(WORD_GAP - LETTER_GAP);
                break;
        }
    }
    delay(LETTER_GAP - SYMBOL_GAP);
}

void process_character(char c) {
    Serial.print("Принят символ: ");
    Serial.println(c);
    
    const char *morse_sequence = char_to_morse(c);
    
    if (morse_sequence != NULL) {
        Serial.print("Код Морзе: ");
        Serial.println(morse_sequence);
        blink_morse_sequence(morse_sequence);
    } else {
        Serial.println("Символ не поддерживается");
    }
}

void setup() {
    // ИСПРАВЛЕННАЯ СКОРОСТЬ
    Serial.begin(9600);
    
    pinMode(EXTERNAL_LED_PIN, OUTPUT);
    pinMode(BUILTIN_LED_PIN, OUTPUT);
    digitalWrite(EXTERNAL_LED_PIN, LOW);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    
    // Тестовое мигание
    for (int i = 0; i < 3; i++) {
        digitalWrite(EXTERNAL_LED_PIN, HIGH);
        digitalWrite(BUILTIN_LED_PIN, HIGH);
        delay(100);
        digitalWrite(EXTERNAL_LED_PIN, LOW);
        digitalWrite(BUILTIN_LED_PIN, LOW);
        delay(100);
    }
    
    Serial.println("Система Морзе готова! Вводите текст:");
}

void loop() {
    if (Serial.available() > 0) {
        char received_char = Serial.read();
        process_character(received_char);
    }
}
