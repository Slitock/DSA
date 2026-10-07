#ifndef GENERATOR_H
#define GENERATOR_H

struct CityRecord {
  int city_code;
  char city_name[50];
};

void generateBinaryFile(const char *filename, int n);

#endif
