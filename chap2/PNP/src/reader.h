#pragma once

#ifndef READER_H
#  define READER_H

#  include "data_entry.h"

#  include <vector>

std::vector<Entry> read_data(int argc, char **argv);

#endif // !READER_H
