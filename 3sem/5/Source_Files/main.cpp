#include "../Head_Files/generator.h"
#include "../Head_Files/linear_search.h"
#include <chrono>
#include <fstream>
#include <iostream>

// узнаем ключ послежней записи
int getLastRecordKey(const char *filename) {
  std::ifstream inFile(filename, std::ios::binary | std::ios::ate);
  if (!inFile)
    return -1;

  std::streampos size = inFile.tellg();
  if (size == 0)
    return -1;

  inFile.seekg(size - static_cast<std::streampos>(sizeof(CityRecord)));

  CityRecord last_record;
  inFile.read((char *)&last_record, sizeof(CityRecord));
  inFile.close();

  return last_record.city_code;
}

int main() {
  const char *filename = "../bin/cities_100.bin";
  int n = 100;

  generateBinaryFile(filename, n);

  // Имитируем худший случай: ищем ключ последней записи в файле
  int target_key = getLastRecordKey(filename);
  std::cout << "Искомый ключ (последний в файле): " << target_key << "\n";

  CityRecord result;
  int comparisons = 0;

  // секундомер
  auto start_time = std::chrono::high_resolution_clock::now();

  bool found = linearSearch(filename, target_key, result, comparisons);

  auto end_time = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time_span = end_time - start_time;

  if (found) {
    std::cout << "\nРЕЗУЛЬТАТ ПОИСКА:\n";
    std::cout << "Запись найдена: Код - " << result.city_code << ", Город - "
              << result.city_name << "\n";
  } else {
    std::cout << "Запись не найдена.\n";
  }

  std::cout << "\nДАННЫЕ ДЛЯ ТАБЛИЦЫ 9:\n";
  std::cout << "Фактическая вычислительная сложность (Тф): " << comparisons
            << " сравнений\n";
  std::cout << "Время выполнения T(n): " << time_span.count() << " мс\n";

  return 0;
}
