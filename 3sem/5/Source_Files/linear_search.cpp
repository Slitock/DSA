#include "../Head_Files/linear_search.h"
#include <fstream>
#include <iostream>

bool linearSearch(const char *filename, int target_code,
                  CityRecord &found_record, int &comparisons) {
  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile) {
    std::cerr << "Ошибка открытия файла для чтения!" << std::endl;
    return false;
  }

  comparisons = 0;
  CityRecord current_record;

  while (inFile.read((char *)&current_record, sizeof(CityRecord))) {
    comparisons++;

    if (current_record.city_code == target_code) {
      found_record = current_record;
      inFile.close();
      return true;
    }
  }

  inFile.close();
  return false;
}
