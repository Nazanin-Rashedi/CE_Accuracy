#include "Hilbert.hh"


Hilbert::Hilbert(const int aN) : _n(aN) {}



//convert (x,y) to d
int 
Hilbert::xy2d (const int n, int x, int y) {
    int rx = 0; 
    int ry = 0;
    int d = 0;
    for (int s = n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, rx, ry, x, y);
    }
    return d;
}
 
//convert d to (x,y)
void 
Hilbert::d2xy(const int n, const int d, int& x, int& y) {
    int rx, ry, t=d;
    x = 0;
    y = 0;
    for (int s = 1; s < n; s *= 2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, rx, ry, x, y);
        x += s * rx;
        y += s * ry;
        t /= 4;
    }
}


