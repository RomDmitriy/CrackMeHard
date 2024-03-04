#include <iostream>
#include <string>
#include <map>
#include <cmath>

#define random rand() % 16

class Int16Parser {
private:
    std::map<int16_t, std::string> _intToStr;
    std::string _symbols = "0123456789ABCDEF";

    auto numberToChar(int16_t number) {
        return number >= 10 ? _intToStr[number] : std::to_string(number);
    }

    auto charToNumber(char* symbol) {
        const unsigned short position = _symbols.find(*symbol);
        return position <= 16 ? position : throw std::exception("Wrong key format");
    }

public:
    Int16Parser() {
        _intToStr[10] = "A";
        _intToStr[11] = "B";
        _intToStr[12] = "C";
        _intToStr[13] = "D";
        _intToStr[14] = "E";
        _intToStr[15] = "F";
    }

    std::string arrayToString(int16_t* key) {
        std::string keyStr;
        for (unsigned short iter = 0, counter = 0; iter < 20; iter++) {
            if (counter % 5 == 0 && counter != 0) {
                keyStr += "-";
                counter = 0;
                iter--;
                continue;
            }
            else counter++;

            keyStr += this->numberToChar(key[iter]);
        }

        return keyStr;
    }

    void stringToArray(std::string keyStr, int16_t* key) {
        if (keyStr.length() != 23) throw std::exception("Wrong key format");

        for (unsigned short i = 0, counter = 0; i < 23; i++) {
            if (keyStr[i] != '-') {
                key[counter] = charToNumber(&keyStr[i]);
                counter++;
            }
        }
    }
};

static std::string generateCode() {
    Int16Parser parser;
    int16_t key[20]{};

    // 1) Генерируем коэффициенты
    int16_t a = 0, b = 0, c = 0;
    do {
        a = random;
        b = random;
        c = random;
    } while (pow(-a, 3) + (b * 2) + c != 0);

    // 2) Генерируем случайные значения
    key[0] = random;
    key[1] = random;
    key[2] = random;
    key[3] = random;
    key[4] = random;

    key[5] = random;
    key[9] = random;

    key[10] = random;
    key[14] = random;

    key[15] = random;
    key[19] = random;

    // 3) Считаем проверочные ячейки
    key[6] = key[0] ^ key[5];
    key[8] = key[1] ^ key[9];

    key[11] = key[2] ^ key[10];
    key[13] = key[3] ^ key[14];

    key[16] = key[4] ^ key[15];
    key[18] = (key[0] ^ key[19]) ^ key[19];

    // 4) Считаем смысловые ячейки
    key[7]  = a ^ (key[(key[5] + key[6] + key[8] + key[9]) % 5]);
    key[12] = b ^ (key[(key[10] + key[11] + key[13] + key[14]) % 5]);
    key[17] = c ^ (key[(key[15] + key[16] + key[18] + key[19]) % 5]);

    // 5) Переводим массив в ключ
    return parser.arrayToString(key);
}

static bool validateCode(std::string code) {
    Int16Parser parser;
    // 1) Парсим ключ в массив чисел
    int16_t key[20]{ 0 };
    parser.stringToArray(code, key);

    // 2) Получаем коэффициенты
    const int16_t a = key[7] ^ key[(key[5] + key[6] + key[8] + key[9]) % 5];
    const int16_t b = key[12] ^ key[(key[10] + key[11] + key[13] + key[14]) % 5];
    const int16_t c = key[17] ^ key[(key[15] + key[16] + key[18] + key[19]) % 5];

    // 3) Проверяем равенство
    if (pow(-a, 3) + (b * 2) + c != 0) {
        return false;
    }

    // 4) Проверяем проверочные коэффициенты
    if ((key[0] ^ key[5]) != key[6]) return false;
    if ((key[1] ^ key[9]) != key[8]) return false;

    if ((key[2] ^ key[10]) != key[11]) return false;
    if ((key[3] ^ key[14]) != key[13]) return false;

    if ((key[4] ^ key[15]) != key[16]) return false;
    if (((key[0] ^ key[19]) ^ key[19]) != key[18]) return false;

    return true;
}

int main()
{
    srand(time(NULL));
    std::string input = "";
    while (true) {
        system("cls");
        std::cout << "Good code: " << generateCode() << std::endl;
        std::cout << "Input activation code: ";
        std::cin >> input;
        try {
            if (!validateCode(input)) {
                std::cout << "Invalid key!" << std::endl;
                system("pause");
                system("read");
            }
            else {
                std::cout << "Key accepted!" << std::endl;
                system("pause");
                system("read");
                break;
            }
        }
        catch (std::exception err) {
            std::cout << err.what() << std::endl;
            system("pause");
            system("read");
        }
    }

    system("cls");
    if (!validateCode(input)) abort();
    std::cout << "Hello World!" << std::endl;
}
