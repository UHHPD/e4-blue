#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

bool testCheckCompatibility() {
  std::cout << "testCheckCompatibility...";
  Data a("testA");
  Data b("testB");
  return testEqual("compatibility",1,a.checkCompatibility(b,1)) && testEqual("compatibility",0,a.checkCompatibility(b,2));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor, testCheckCompatibility});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of all experiments
  std::vector<Data> data_sets;
  Data datA("exp_A");
  data_sets.push_back(datA);
  Data datB("exp_B");
  data_sets.push_back(datB);
  Data datC("exp_C");
  data_sets.push_back(datC);
  Data datD("exp_D");
  data_sets.push_back(datD);

  // measured cross section in bin 27
  for(int i = 0; i < 4; i++) {
    cout << "bin 27: from " << data_sets[i].binLow(27) << " to " << data_sets[i].binHigh(27)
       << endl;
    cout << "measurement of experiment " << i <<" in bin 27: " << data_sets[i].measurement(27)
       << endl;
  }
  
  // check compatibility of bin 27 
  int n = 1;
  double delta_mu = data_sets[0].measurement(27) - data_sets[1].measurement(27);
  if(delta_mu < 0) {delta_mu *= -1;}
  double sigma = sqrt(pow(data_sets[0].error(27),2) + pow(data_sets[1].error(27),2)) * n;
  if (delta_mu < sigma) {
   cout << "data sets A and B compatible in bin 27" << endl;
  } else {
    cout << "data sets A and B not compatible in bin 27" << endl;
  }
  
  // statistical analysis
  // a) check compatibility
  n = 2;
  cout << datA.checkCompatibility(datB, n) << " data points from set A and B differ by more than " << n << " s.d." << endl;
  cout << datA.checkCompatibility(datC, n) << " data points from set A and C differ by more than " << n << " s.d." << endl;
  cout << datA.checkCompatibility(datD, n) << " data points from set A and D differ by more than " << n << " s.d." << endl;
  n = 3;
  cout << datA.checkCompatibility(datB, n) << " data points from set A and B differ by more than " << n << " s.d." << endl;
  cout << datA.checkCompatibility(datC, n) << " data points from set A and C differ by more than " << n << " s.d." << endl;
  cout << datA.checkCompatibility(datD, n) << " data points from set A and D differ by more than " << n << " s.d." << endl;

  
  
  // b) values of chi_square for individual model
  for(int i = 0; i < 4; i++) {
    cout << "chi square value for data set " << i << ": " << data_sets[i].chi_square() << endl;
  }
  
  // c) value of chi_square for combined model
  Data sum = datA + datB + datC + datD;
  cout << "chi square value for combined data set: " << sum.chi_square() << endl;
  return 0;
}
