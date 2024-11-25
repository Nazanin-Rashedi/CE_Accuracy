#include "like.hh"
#include <cstdlib>
#include <iostream>

// #define DEBUG_LIKE

#ifdef DEBUG_LIKE
  using namespace std;
#endif

bool 
like(const char *s, const char *p)
{
    const char *ss;
    const char *pp;

#ifdef DEBUG_LIKE
    cout << endl << "trace: like(0): " << s << " " << p << endl;
#endif

    while (*s && *p && *s == *p && *p != '%') {
	s++;
	p++;
    }

#ifdef DEBUG_LIKE
         cout << "  trace(1): s = " << s << ", p = " << p << endl;
#endif

    if (*s == 0) {
	while (*p == '%') {
	    p++;
	}
	if (*p == 0) {
	    return true;
	} else {
#ifdef DEBUG_LIKE
            cout << "  trace(2): s = " << s << ", p = " << p << endl;
#endif
	    return false;
	}
    } else if (*p == '%') {

#ifdef DEBUG_LIKE
         cout << "  trace(3): s = " << s << ", p = " << p << endl;
#endif
	if (*(p + 1) == 0) {
	    return true;
	} else {
	    ss = s;
	    pp = ++p;
	    while (*ss != 0) {
		s = ss;
		p = pp;

#ifdef DEBUG_LIKE
		cout << "  while: " << s << " " << p << endl;
#endif

		while (*s && *p && *s == *p && *p != '%') {
		    s++;
		    p++;
		}

#ifdef DEBUG_LIKE
		cout << "     lx: " << s << " " << p << endl;
#endif

		if (*s == 0) {
		    while (*p == '%') {
			p++;
		    }
		    if (*p == 0) {
			return true;	// break;
		    } else {
			return false;	// break;
		    }
		} else if (*p == '%') {
		    if (*(p + 1) == 0) {
			return true;	// break;
		    } else if (like(s, p)) {
			return true;	// break;
		    }
		}
		ss++;
	    }
#ifdef DEBUG_LIKE
            cout << "  trace(5): s = " << s << ", p = " << p << endl;
#endif
	    return false;
	}
    } else {
#ifdef DEBUG_LIKE
      cout << "  trace(6): s = " << s << ", p = " << p << endl;
#endif
	return false;
    }
#ifdef DEBUG_LIKE
    cout << "  trace(7): s = " << s << ", p = " << p << endl;
#endif
    return false;
}

