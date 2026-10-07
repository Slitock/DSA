#include "../Head_Files/generator.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

// созднаие бинарника
void generateBinaryFile(const char *filename, int n) {
  std::ofstream outFile(filename, std::ios::binary);

  srand(time(0));

  int *used_codes = new int[n];

  for (int i = 0; i < n; i++) {
    CityRecord record;

    bool is_unique;
    do {
      is_unique = true;
      record.city_code = rand() % 90000 + 10000;

      for (int j = 0; j < i; j++) {
        if (used_codes[j] == record.city_code) {
          is_unique = false;
          break;
        }
      }
    } while (!is_unique);

    used_codes[i] = record.city_code;

    snprintf(record.city_name, sizeof(record.city_name), "City_%d",
             record.city_code);

    outFile.write((char *)&record, sizeof(CityRecord));
  }

  delete[] used_codes;
  outFile.close();
  std::cout << "Файл " << filename << " успешно создан. Записей: " << n
            << std::endl;
}
