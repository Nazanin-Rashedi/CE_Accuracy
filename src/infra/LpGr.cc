
bool
run_ks_lp_3(const uint_vt& aKsPat, const double_vt& aKsVal) {
  assert(aKsPat.size() == aKsVal.size());
  std::cout << "run_ks_lp_3:" << std::endl
            << "   pat = " << std::setw(3) << aKsPat << std::endl
            << "   val = " << std::setw(3) << aKsVal << std::endl
            << std::endl;

  const uint n = 4;
  const uint m = aKsPat.size();
  const uint N = (1 << n);

  Matrix A(m,N); 

  // fill A(0..m,0..N) with design matrix
  for(uint i = 0; i < m; ++i) {
    const uint ii = aKsPat[i];
    for(uint j = 0; j < N; ++j) {
       A(i,  j) =  ((ii | j) == j);  // >= beta[i]
    }
  }

  // slackvariablen
  // for(uint j = 0; j < m; ++j) {
  //   A(j, N + j) = 1;
  // }


  std::cout << "A = " << std::endl;
  A.print(std::cout, 3) << std::endl;

  // uint_vt lColIdxBase(m);  // Spaltenindexvektor der Basis
  // uint_vt lColIdxOther(N); // Spaltenindexvektorn der restlichen Spalten
 
  // initiale Basis ist Spalte N,...N+m
  // for(uint j = 0; j < m; ++j) {
  //   lColIdxBase[j] = N+j;
  // }

  // for(uint j = 0; j < N; ++j) {
  //   lColIdxOther[j] = j;
  // } 

  // std::cout << "B = " << lColIdxBase << std::endl;
  // std::cout << "N = " << lColIdxOther << std::endl;

  // Basisloesung
  // x_N = 0, x_B = A_B^{-1} b
  double_vt b(m);
  for(uint j = 0; j < m; ++j) {
     b[j] = aKsVal[j];
  }

  std::cout << "b = " << b << std::endl;


  // zielfunktion
  double_vt c(N); // \ol{c}^t = c_N^t - c_B^t A_B^{-1} A_N
  uint_vt   l1A(N);  // remember the number of '1' in every column (= column sum = \vec{1} A
  // da im momeent c_B^t = 0 gilt \ol{c}^t = c_N^t
  for(uint j = 0; j < N; ++j) {
    uint lNoOne = 0;
    for(uint i = 0; i < m; ++i) {
      const uint ii = aKsPat[i];
      lNoOne += ((ii | j) == j);
    }
    c[j]   = lNoOne;
    l1A[j] = lNoOne;
  }
  std::cout << "c = " << c << std::endl;

  // zielfunktionswert
  double c0 = 0; 
  std::cout << "c0 = " << c0 << std::endl;
 
  // \ol{A} = A_B^{-1} A_N. here: A_B = A_B^{-1} = I

  // indexmengen
  uint_vt lBidx(m); // indexmenge der basisloesung
  uint_vt lNidx(N); // indexmenge der uebrigen spaltenvektoren
  for(uint i = 0; i < m; ++i) {
    lBidx[i] = N + i;
  }
  for(uint j = 0; j < N; ++j) {
    lNidx[j] = j;
  }
  std::cout << "  B = " << lBidx << std::endl;
  std::cout << "  N = " << lNidx << std::endl;

  double_vt v(N);  // Hilfsvektor
  uint lRound = 0;
  for(lRound = 0; lRound < 1000; ++lRound) {
    std::cout << "  Round " << lRound << std::endl;

    // optimal?
    const double lEps = 1E-10;
    bool lOptimal = true;
    for(uint j = 0; j < N; ++j) {
      if(lEps <= c[j]) {
        lOptimal = false;
        break;
      }
    }
    if(lOptimal) {
      break;
    }

    // suche austauschspalte
    uint s = 0;
    for(uint j = 0; j < N; ++j) {
      if(0 < c[j]) {
        s = j;
        break;
      }
    }
    std::cout << "    s = " << s << "  (pivot column)" << std::endl;

    // unbeschraenkt (brauchen wir eigentlich nicht)
    bool lBeschraenkt = false;
    for(uint i = 0; i < m; ++i) {
      if(0 < A(i, s)) {
        lBeschraenkt = true;
        break;
      }
    }
    if(!lBeschraenkt) {
      std::cout << "    unbeschraenkt" << std::endl;
    }

    double lLambda0 = std::numeric_limits<double>::max();
    uint r   = 0;
    for(uint i = 0; i < m; ++i) {
      if(0 < A(i,s)) {
        const double lQuot = b[i] / A(i,s);
        if(lQuot < lLambda0) { // < is alternative
          lLambda0 = lQuot;
          r = i;
        }
      }
    }
    std::cout << "    r = " << r 
              << " , lambda_0 = " << lLambda0 
              << std::endl;
  
    // NEUBERECHNUNGEN
    const double x = ((double) 1.0) / A(r,s);
    std::cout << "    x = " << x << std::endl;

    // berechne v
    for(uint j = 0; j < N; ++j) {
      v[j] = x * A(r,j);
    }

    std::cout << "    v = " << v << std::endl;

    // Neuberechnung c
    const double w = c[s];
    for(uint j = 0; j < N; ++j) {
      if(j == s) { continue; }
      c[j] = c[j] - v[j] * w;
    }
    c[s] = - x * w;
    c0 = c0 + x * b[r] * w;

    std::cout << "    c  = " << c << std::endl;
    std::cout << "    c0 = " << c0 << std::endl;

    // Neuberechnung b
    const double z = x * b[r];
    for(uint i = 0; i < m; ++i) {
      if(i == r) { continue; }
      b[i] = b[i] - z * A(i,s);
    }
    b[r] = z;
    std::cout << "    b = " << b << std::endl;

    // Neuberechnung A
    for(uint i = 0; i < m; ++i) {
      if(i == r) { continue; }
      const double y = A(i,s);
      if(0 == y) { continue; }
      for(uint j = 0; j < N; ++j) {
        if(j == s) { continue; }
        A(i,j) = A(i,j) - v[j] * y;
      }
    }
    for(uint j = 0; j < N; ++j) {
      if(j == s) { continue; }
      A(r,j) = v[j];
    }
    for(uint i = 0; i < m; ++i) {
      if(i == r) { continue; }
      A(i,s) = - x * A(i,s);
    }
    A(r,s) = x;

    // std::cout << "    A = " << std::endl;
    // A.print(std::cout, 3, 6, Matrix::kAscii);

    // Neuberechnung N, B
    std::swap(lBidx[r], lNidx[s]);
    std::cout << "    B = " << lBidx << std::endl;
    std::cout << "    N = " << lNidx << std::endl;
  }

  double lSumKs = 0; // vec{1} b  fuer stopkriterium/erfolgskriterium
  for(uint i = 0; i < m; ++i) {
    lSumKs += aKsVal[i];
  }

  std::cout << "run_ks_lp_3: fin: c0 = " << c0 << std::endl;
  std::cout << "   b = " << b << std::endl;
  std::cout << "  1A = " << l1A << std::endl;
  std::cout << "  1b = " << lSumKs << "  (here: original b = known sels)" << std::endl;

  double lSum2 = 0;
  for(uint j = 0; j < m; ++j) {
    if(lBidx[j] < N) {
      lSum2 += l1A[lBidx[j]] * b[j];
    } else {
      // lSum2 += b[j];
    }
  }

  std::cout << "  sum(ksval) = " << lSumKs
            << "  =?= " << lSum2 << " ; c0 = " << c0
            << "; diff = " << (lSumKs - c0)
            << "; #R = " << lRound
            << std::endl;

  bool lSolvable = (c0 == lSumKs);
  std::cout << "solvable: " << lSolvable << std::endl;

  if(!lSolvable) {
    return false;
  }

  // analyze case \vec{1} A x = \vec{1} b
  /// not necessary here

  return lSolvable;
}

