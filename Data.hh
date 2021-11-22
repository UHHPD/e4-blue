#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>

class Data {
 public:
  Data(const std::string& filename);
  int checkCompatibility(const Data& in, int n);
  Data operator+(const Data& in);
  double chi_square();

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return (m_bins[i]+m_bins[i+1])/2; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }
  void set_data(double d, int i) { m_data[i] = d; }
  void set_bin(double b, int i) { m_bins[i] = b; }
  void set_error(double e, int i) { m_errors[i] = e; }

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;
};

#endif
