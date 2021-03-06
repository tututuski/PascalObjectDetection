#ifndef FILE_IO_H
#define FILE_IO_H

#include "Common.h"
#include "SupportVectorMachine.h"
#include "Feature.h"
#include "Detection.h"

void saveToFile(const std::string &filename, const SupportVectorMachine &svm);
void loadFromFile(const std::string &filename, SupportVectorMachine &svm);

void saveToFile(const std::string &filename, const std::vector<Detection> &dets);

#endif // FILE_IO_H
