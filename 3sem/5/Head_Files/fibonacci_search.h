#ifndef FIBONACCI_SEARCH_H
#define FIBONACCI_SEARCH_H

#include "generator.h"
#include <vector>

struct IndexRecord {
  int key;
  long offset;
};

// Функция заполнения таблицы из бинарного файла
void createIndexTable(const char *filename, std::vector<IndexRecord> &table);

// Функция сортировки таблицы
void sortIndexTable(std::vector<IndexRecord> &table);

// Алгоритм поиска Фибоначчи в таблице. Возвращает смещение или -1.
long fibonacciSearch(const std::vector<IndexRecord> &table, int target_code,
                     int &comparisons);

// Функция прямого доступа к файлу по смещению
bool readRecordByOffset(const char *filename, long offset, CityRecord &record);

#endif
