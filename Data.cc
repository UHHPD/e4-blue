#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  
  for (int i = 0; i < size; ++i) {
   double error;
   file >> error;
   m_errors.push_back(error);
  }

  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& in, int n) {
  int counter = 0;
  int size = in.size();
  for (int i = 0; i < size; i++) {
    double delta_mu = in.measurement(i) - m_data[i];
    if(delta_mu < 0) {delta_mu *= -1;}
    double sigma = sqrt(pow(in.error(i),2) + pow(m_errors[i],2)) * n;
    if (delta_mu > sigma) {
      counter++;
    }
  }
  return counter;
}
