#include "./STime.h"

void STime::getSystemTime() {
    time_t t;
    struct tm *tm_p;
    t = time(NULL);
    tm_p = localtime(&t);
    h  = tm_p->tm_hour;
    m = tm_p->tm_min;
    s = tm_p->tm_sec;
    delete []tm_p;
}

std::ostream &operator<< (std::ostream& os, const STime& st) {
    (st.h <= 9) ? os << "0" << st.h << ":" : os << st.h << ":";
    (st.m <= 9) ? os << "0" << st.m << ":" : os << st.m << ":";
    (st.s <= 9) ? os << "0" << st.s : os << st.s;
    return os;
}


