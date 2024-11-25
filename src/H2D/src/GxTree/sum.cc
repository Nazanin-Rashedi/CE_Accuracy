#include <iostream>
#include <iomanip>


/*
 *  simple procedure used by show_coverage
 *  sums up occurrences of different node types
 */

const int NO_G  =  8;
const int NO_L  = 11;
const int NO_M  =  8;
const int NO_S  =  4;
const int MAX_X = 12;

std::string G_Name[NO_G] = { "G.I.A",
                             "G.I.B",
                             "G.I.C",
                             "G.I.D",
                             "G.L.A",
                             "G.L.B",
                             "G.L.C",
                             "G.L.D" };


std::string L_Name[NO_L] = { "L.I.A1",
                             "L.I.A2",
                             "L.I.B1",
                             "L.I.B2",
                             "L.I.C1",
                             "L.I.C2",
                             "L.L.A1",
                             "L.L.B1",
                             "L.L.B2",
                             "L.L.C1",
                             "L.L.C2" };


std::string M_Name[NO_M] = { "M.I.A1",
                             "M.I.A2",
                             "M.I.B2",
                             "M.L.A1",
                             "M.L.A2",
                             "M.L.B1",
                             "M.L.B2",
                             "M.L.C2" };

std::string S_Name[NO_S] = { "S.I.A2",
                             "S.L.A1",
                             "S.L.A2",
                             "S.L.B2" };

 
struct sum_t {
  int no[4][MAX_X];

  sum_t() : no() {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < MAX_X; ++j) {
        no[i][j] = 0;
      }
    }
  }
  void print(std::ostream& os) const;
};

void
sum_t::print(std::ostream& os) const {
  for(int i = 0; i < NO_G; ++i) {
    os << std::setw(6) << G_Name[i] << ' ';
  }
  os << std::endl;
  for(int i = 0; i < NO_G; ++i) {
    std::cout << std::setw(6) << no[0][i] << ' ';
  }
  os << std::endl;

  for(int i = 0; i < NO_L; ++i) {
    os << std::setw(6) << L_Name[i] << ' ';
  }
  os << std::endl;
  for(int i = 0; i < NO_L; ++i) {
    std::cout << std::setw(6) << no[1][i] << ' ';
  }
  os << std::endl;

  for(int i = 0; i < NO_M; ++i) {
    os << std::setw(6) << M_Name[i] << ' ';
  }
  os << std::endl;
  for(int i = 0; i < NO_M; ++i) {
    std::cout << std::setw(6) << no[2][i] << ' ';
  }
  os << std::endl;

  for(int i = 0; i < NO_S; ++i) {
    os << std::setw(6) << S_Name[i] << ' ';
  }
  os << std::endl;
  for(int i = 0; i < NO_S; ++i) {
    std::cout << std::setw(6) << no[3][i] << ' ';
  }
  os << std::endl;

}

void
read(std::istream& aIs, sum_t& aSum) {
  char c = '0';
  int  n = 0;
  int  k = 0;
  int  j = 0;
  while(!aIs.eof() && aIs) {
    aIs >> c;
    // std::cout << "c = " << c << std::endl;
    switch(c) {
      case 'G': j = 0;
                n = NO_G;
                break;
      case 'L': j = 1;
                n = NO_L;
                break;
      case 'M': j = 2;
                n = NO_M;
                break;
      case 'S': j = 3;
                n = NO_S;
                break;
      default: std::cerr << "unknown node type: " << c << std::endl;
      return;
    }
    for(int i = 0; i < n; ++i) {
      aIs >> k;
      aSum.no[j][i] += k;
    }
  }
}





int
main() {
  sum_t lSum;
  read(std::cin, lSum);
  lSum.print(std::cout);
}


