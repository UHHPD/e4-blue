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

Data Data::operator+(const Data& in) {
  Data avg(in);
  int n = 3;
  int c = this->checkCompatibility(in, n);
  if(c > 0) {
    cout << "data sets not compatible for n = " << n << " for " << c << "entries" << endl;
  }
  double w1, w2, y, sigma_y;
  for(int i = 0; i < in.size(); i++) {
    w1 = 1/pow(this->m_errors[i], 2);
    w2 = 1/pow(in.error(i), 2);
    y = (w1*this->m_data[i] + w2*in.measurement(i))/(w1 + w2);
    sigma_y = sqrt(1/(w1 + w2));
    avg.set_data(y, i);
    avg.set_error(sigma_y, i);
  }
  return avg;
}

double background(double x) {
  double alpha = 0.005, beta = -0.00001, gamma = 0.08, delta = 0.015;
  return alpha + beta * x + gamma * exp(-delta * x);
}

double Data::chi_square() {
  double chi = 0;
  for (int i = 0; i < this->size(); i++) {
    chi += pow(this->measurement(i) - background(this->binCenter(i)), 2)/pow(this->error(i), 2);
  }
  return chi/52;
}
