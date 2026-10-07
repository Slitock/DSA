#include "../Head_Files/fibonacci_search.h"
#include "../Head_Files/generator.h"
#include "../Head_Files/linear_search.h"
#include <chrono>
#include <fstream>
#include <iomanip> // форматирование таблицы
#include <iostream>
#include <string>
#include <vector>

// Получение худшего ключа
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

void runTest(int n) {
  std::string filename_str = "../bin/cities_" + std::to_string(n) + ".bin";
  const char *filename = filename_str.c_str();

  generateBinaryFile(filename, n);

  // Имитируем худший случай: берем ключ последней записи в файле
  int target_key = getLastRecordKey(filename);

  // ================== ЗАДАНИЕ 1: Линейный поиск ==================
  CityRecord result_linear;
  int comp_linear = 0;

  auto start_lin = std::chrono::high_resolution_clock::now();
  linearSearch(filename, target_key, result_linear, comp_linear);
  auto end_lin = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time_lin = end_lin - start_lin;

  // ================== ЗАДАНИЕ 2: Поиск Фибоначчи ==================
  CityRecord result_fib;
  int comp_fib = 0;

  auto start_fib = std::chrono::high_resolution_clock::now();

  std::vector<IndexRecord> indexTable;
  createIndexTable(filename, indexTable); // Создание таблицы в памяти
  sortIndexTable(indexTable);             // Сортировка таблицы

  long offset = fibonacciSearch(indexTable, target_key, comp_fib);

  if (offset != -1) {
    readRecordByOffset(filename, offset, result_fib);
  }

  auto end_fib = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time_fib = end_fib - start_fib;

  // ================== ВЫВОД В ТАБЛИЦУ ==================
  // Выводим строку с результатами для текущего 'n'
  std::cout << std::left << std::setw(10) << n << std::setw(15)
            << time_lin.count() << std::setw(15) << comp_linear << std::setw(15)
            << time_fib.count() << std::setw(15) << comp_fib << "\n";
}

int main() {
  std::cout << "Генерация файлов и выполнение поиска...\n\n";

  // Вывод шапки
  std::cout << std::left << std::setw(10) << "n" << std::setw(15)
            << "T(n) Лин, мс" << std::setw(15) << "Тф Лин" << std::setw(15)
            << "T(n) Фиб, мс" << std::setw(15) << "Тф Фиб" << "\n";
  std::cout << "---------------------------------------------------------------"
               "-------\n";

  // Выполнение тестирования на файлах объема 100, 1000, 10000
  runTest(100);
  runTest(1000);
  runTest(10000);

  return 0;
}
