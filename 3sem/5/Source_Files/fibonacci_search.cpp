#include "../Head_Files/fibonacci_search.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void createIndexTable(const char *filename, std::vector<IndexRecord> &table) {
  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile) {
    std::cerr << "Ошибка: не удалось открыть файл для создания таблицы!"
              << std::endl;
    return;
  }

  CityRecord temp_record;
  long current_offset = 0;

  // сохраняем в память только ключ и текущее смещение из файла
  while (inFile.read((char *)&temp_record, sizeof(CityRecord))) {
    IndexRecord index_rec;
    index_rec.key = temp_record.city_code;
    index_rec.offset = current_offset;

    table.push_back(index_rec);

    current_offset += sizeof(CityRecord);
  }
  inFile.close();
}

void sortIndexTable(std::vector<IndexRecord> &table) {
  // Сортируем таблицу по возрастанию ключей
  std::sort(
      table.begin(), table.end(),
      [](const IndexRecord &a, const IndexRecord &b) { return a.key < b.key; });
}

long fibonacciSearch(const std::vector<IndexRecord> &table, int target_code,
                     int &comparisons) {
  int n = table.size();
  if (n == 0)
    return -1;

  // Инициализация чисел Фибоначчи
  int fibMMm2 = 0;
  int fibMMm1 = 1;
  int fibM = fibMMm2 + fibMMm1;

  while (fibM < n) {
    fibMMm2 = fibMMm1;
    fibMMm1 = fibM;
    fibM = fibMMm2 + fibMMm1;
  }

  int offset = -1;
  comparisons = 0;

  while (fibM > 1) {
    int i = std::min(offset + fibMMm2, n - 1);

    comparisons++;
    if (table[i].key < target_code) {
      fibM = fibMMm1;
      fibMMm1 = fibMMm2;
      fibMMm2 = fibM - fibMMm1;
      offset = i;
    } else {
      comparisons++;
      if (table[i].key > target_code) {
        fibM = fibMMm2;
        fibMMm1 = fibMMm1 - fibMMm2;
        fibMMm2 = fibM - fibMMm1;
      } else {
        return table[i].offset; // Ключ найден
      }
    }
  }

  comparisons++;
  if (fibMMm1 && offset + 1 < n && table[offset + 1].key == target_code) {
    return table[offset + 1].offset;
  }

  return -1; // Ключ не найден
}

bool readRecordByOffset(const char *filename, long offset, CityRecord &record) {
  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile)
    return false;

  // Моментально прыгаем на нужный байт в файле (Прямой доступ)
  inFile.seekg(offset, std::ios::beg);

  // Считываем ровно одну запись
  bool success = (bool)inFile.read((char *)&record, sizeof(CityRecord));
  inFile.close();

  return success;
}
