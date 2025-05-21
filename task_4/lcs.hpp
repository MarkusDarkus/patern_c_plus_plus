#ifndef LCS_H
#define LCS_H

template<class D>
struct less_than_comparable {

    friend bool operator> (const less_than_comparable& c1, const less_than_comparable& c2) {
        return static_cast<const D*>(&c2) < static_cast<const D*>(&c1);
    }

    friend bool operator!= (const less_than_comparable& c1, const less_than_comparable& c2) {
        return static_cast<const D*>(&c1) < static_cast<const D*>(&c2) ||
            static_cast<const D*>(&c1) > static_cast<const D*>(&c2);
    }

    friend bool operator== (const less_than_comparable& c1, const less_than_comparable& c2) {
        return !(static_cast<const D*>(&c1) != static_cast<const D*>(&c2));
    }

    friend bool operator<= (const D& c1, const D& c2) {
        return static_cast<const less_than_comparable*>(&c1) < static_cast<const less_than_comparable*>(&c2) ||
            static_cast<const D*>(&c1) == static_cast<const D*>(&c2);
    }
    friend bool operator>= (const less_than_comparable& c1, const less_than_comparable& c2) {
        return static_cast<const D*>(&c2) <= static_cast<const D*>(&c1);
    }
};

#endif //LCS_H
