// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <set>
#include <map>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Avg = constant
    // As per "https://en.cppreference.com/w/cpp/container/unordered_map/size"
    unsigned int town_count();

    // avg = n, where n is city count in our unordered map
    // https://en.cppreference.com/w/cpp/container/unordered_map/clear
    void clear_all();

    // avg = constant
    // https://www.cplusplus.com/reference/unordered_map/unordered_map/find/
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // avg = constant
    // referencing unordered map on average is constant, according to cppreference
    // https://www.cplusplus.com/reference/unordered_map/unordered_map/operator[]/
    Name get_town_name(TownID id);

    // avg = constant
    // referencing unordered map on average is constant, according to cppreference
    // https://www.cplusplus.com/reference/unordered_map/unordered_map/operator[]/
    Coord get_town_coordinates(TownID id);

    // avg = constant
    // referencing unordered map on average is constant, according to cppreference
    // https://www.cplusplus.com/reference/unordered_map/unordered_map/operator[]/
    int get_town_tax(TownID id);

    // average = n, where n stands for elements in town_db aka number of towns
    // Has one for-loop, hence the average of n.
    std::vector<TownID> all_towns();


    // average = n, where n stands for elements in town_db aka number of towns
    // Has for-loop (n) and push_back (constant). (n) being slower results in
    // whole function averaging to (n).
    std::vector<TownID> find_towns(Name const& name);

    // average = constant
    // nothing but constant operations within this function,
    // so average for function remains constant
    bool change_town_name(TownID id, Name const& newname);

    // average = n*logn, n being amount of towns
    // sort() is the slowest operation inside this function
    // hence the nlogn
    std::vector<TownID> towns_alphabetically();

    // average = n*logn, n being amount of towns
    // sort() is the slowest operation inside this function
    // hence the nlogn
    std::vector<TownID> towns_distance_increasing();

    // exactly linear n
    // has to access every single n-amount of keys inside unordered map once
    TownID min_distance();

    // exactly linear n
    // has to access every single n-amount of keys inside unordered map once
    TownID max_distance();

    // avg = constant
    // find() and simply referencing the vector both have average constant
    bool add_vassalship(TownID vassalid, TownID masterid);

    // avg = linear (n)
    // because of the for-loop iterating through n elements in vassal vector
    std::vector<TownID> get_town_vassals(TownID id);

    // Linear n, with n being the amount of cities in the
    // taxer path
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // has for-loops (non-nested) iterating
    // over vassal vectors...so linear n
    // with n being the amount of vassals?
    bool remove_town(TownID id);

    // because of sort() this one is nlogn yet again
    // n being the amount of towns in total
    std::vector<TownID> towns_nearest(Coord coord);

    // linear n
    // has while loop, std::reverse, calls for furthest_town(...) which all have n
    // n being the amount of cities in chain
    std::vector<TownID> longest_vassal_path(TownID id);

    // not implemented...
    int total_net_tax(TownID id);

    // linear because of the for-loop iterating over vassals for each city
    // Using the to find the furthest town so we can calculate longest_vassal_path()
    TownID furthest_town(TownID id, int temp = 0, int distance = 1, TownID id_return = NO_TOWNID);

    // Phase 2 operations

    // O(n), where n is the amount of cities. Every city gets their "roads_to" structure cleared
    // within 1 for loop.

    // ...second thought, it has a for-loop with a .clear()-command for umap inside the loop
    // .clear() is linear in size, so...is it actually n²?
    void clear_roads();

    // O(n²), because of 2 nested for-loops. N being the city count.
    std::vector<std::pair<TownID, TownID>> all_roads();

    // O(log n), because of the .insert() to multimap
    // Everything else is mostly referencing data strutcture, so I think .insert()
    // takes the cake here.
    // https://www.cplusplus.com/reference/map/multimap/insert/
    bool add_road(TownID town1, TownID town2);

    // O(n), because of one for loop. N being the adjacent cities
    // with a connected road
    std::vector<TownID> get_roads_from(TownID id);

    // O(n), because of recursive funtion WITHOUT a for-loop.
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // O(n²), because of recursive function WITH a for-loop
    std::vector<TownID> depth_first_search(TownID fromid, TownID toid, std::vector<TownID> route);

    // Non-compulsory phase 2 operations

    // O(n) because of having to find the pair as value from multimap
    // within a for-loop
    bool remove_road(TownID town1, TownID town2);

    // O(n²) due to 2 nested loops within "breath_first_search", which is called inside this function
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // O(n²) due to 2 nested loops
    bool breath_first_search(TownID fromid, TownID toid);

    // Calls for depth_first_cycle, which has n², so O(n²)
    std::vector<TownID> road_cycle_route(TownID startid);

    // 2 nested loops, so O(n²)
    bool depth_first_cycle(TownID startid);

    // Calls for dijkstra_enhanced (aka. A*) which has O(n²)
    // due to nested loops
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // O(n²) due to nested loops
    bool dijkstra_enhanced(TownID start, TownID finish);

    // O(n²), two nested for-loops
    Distance trim_road_network();



private:    
    struct Town {
        TownID town_id = NO_TOWNID;
        Name town_name = NO_NAME;
        Coord coords;
        int tax = NO_VALUE;

        Town *master = nullptr;
        std::vector<Town*> vassals;

        std::unordered_map<Town*, int> roads_to;

        std::string color = "white";
        int source_dist = -1;
        TownID arrived_from = NO_TOWNID;
        Town* went_to = nullptr;
        bool loop_cause = false;

        // We want to use negative values
        // so that priority queue gives the cheapest
        // Dijkstra...or more precisely A*
        uint dijkstra_d = 1;
        uint dijkstra_de = 1;
        bool re_relaxed = false;

    };

    std::unordered_map<TownID,Town> town_db;
    std::multimap<int, std::pair<Town*, Town*>> road_db;

    // O(1), only referencing and addition
    void relax_A(Town &u, Town &v, Town &goal);

    // O(n), for-loop iterating over cities
    void reset_town_flags();


};







#endif // DATASTRUCTURES_HH
