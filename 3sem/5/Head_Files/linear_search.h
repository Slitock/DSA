#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include "generator.h"

bool linearSearch(const char *filename, int target_code,
                  CityRecord &found_record, int &comparisons);

#endif
