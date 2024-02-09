#include "City.h"

std::ostream& operator<< (std::ostream& ostr, const City& city) {
    ostr << city.name << ", pop: " << city.population << ", area: " << city.area << " sqkm, since: " << city.since << ", state: " << city.state;
    return ostr;
}
