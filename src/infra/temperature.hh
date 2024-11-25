#ifndef TEMPERATURE_HH

class Temperature {
  enum scale {Kelvin, Celsius, Fahrenheit};
  public:
    Temperature();
    Temperature(double x, scale);
    ~Temperature();
  public:
    inline double celsius() const { return _tempInC; }
  public:
    double fahrenheit() const;
    double kelvin() const;
  private:
    static double celsius(double x, scale s);
  private:
    double _tempInC; // temperature in celsius
};
#endif
