#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Вариант 21
struct CityRecord {
  int cityCode;      // Ключ поиска
  char cityName[60]; // Фиксированная строка
};

// Результат работы функции поиска
struct SearchResult {
  bool found;
  CityRecord record;
  long long comparisons; // ТФ
  double timeMs;         // T(n)
};

// генератор текстового бинарника
int generateBinaryFile(const std::string &filename, int count) {
  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile.is_open()) {
    std::cerr << "Не удалось открыть файл на запись\n";
    return -1;
  }

  // создание списока ключей
  std::vector<int> keys(count);
  for (int i = 0; i < count; ++i) {
    keys[i] = 1000 + i;
  }

  // 2. Просто перемешиваем их в случайном порядке
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  for (int i = count - 1; i > 0; --i) {
    int j = std::rand() % (i + 1);
    std::swap(keys[i], keys[j]);
  }

  // запись в файл
  CityRecord record;
  for (int i = 0; i < count; ++i) {
    record.cityCode = keys[i];

    std::string name = "City_" + std::to_string(keys[i]);
    std::strncpy(record.cityName, name.c_str(), sizeof(record.cityName) - 1);
    record.cityName[sizeof(record.cityName) - 1] = '\0';

    outFile.write((char *)&record, sizeof(CityRecord));
  }

  outFile.close();

  return keys.back();
}

// лин поиск в бинаре
SearchResult linearSearchInFile(const std::string &filename, int targetKey) {
  SearchResult result = {false, {}, 0, 0.0};

  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Ошибка открытия файла: " << filename << '\n';
    return result;
  }

  CityRecord currentRecord;

  // запуск замера
  auto start = std::chrono::high_resolution_clock::now();

  while (inFile.read((char *)&currentRecord, sizeof(CityRecord))) {
    result.comparisons++;
    if (currentRecord.cityCode == targetKey) {
      result.found = true;
      result.record = currentRecord;
      break;
    }
  }

  // подсчет Т(n)
  auto end = std::chrono::high_resolution_clock::now();
  result.timeMs =
      std::chrono::duration<double, std::milli>(end - start).count();

  inFile.close();
  return result;
}

int main() {
  std::vector<size_t> sizes = {100, 1000, 10000};
  const std::string filename = "cities.dat";

  std::cout
      << "--- Тестирование Задания 1 (Худший случай: последний элемент) ---\n";
  std::cout << "n\tT(n), мс\tТФ (сравнений)\tНайден\n";

  for (size_t n : sizes) {
    int targetKey = generateBinaryFile(filename, n);

    SearchResult res = linearSearchInFile(filename, targetKey);

    std::cout << n << "\t" << res.timeMs << "\t" << res.comparisons << "\t\t"
              << (res.found ? "Да" : "Нет") << '\n';
  }

  return 0;
}
