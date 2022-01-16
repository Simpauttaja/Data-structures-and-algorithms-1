// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <iostream>
#include <set>
#include <queue>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    return town_db.size(); // Avg = constant
}

void Datastructures::clear_all()
{
    town_db.clear(); // avg = n, where n is city count in our unordered map
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    if (town_db.find(id) != town_db.end()) { // If id already exists in map
        return false;
    } else {
        Town temp;
        temp.town_id = id;
        temp.coords.x = coord.x;
        temp.coords.y = coord.y;
        temp.tax = tax;
        temp.town_name = name;
        town_db[id] = temp;
    }
    return true;
}

Name Datastructures::get_town_name(TownID id)
{
    if (town_db.find(id) == town_db.end()) { // avg = constant
        return NO_NAME;
    }
    return town_db[id].town_name; // avg = constant
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    if (town_db.find(id) == town_db.end()) { // avg = constant
        return NO_COORD;
    }
    return town_db[id].coords; // avg = constant
}

int Datastructures::get_town_tax(TownID id)
{
    if (town_db.find(id) == town_db.end()) { // avg = constant
        return NO_VALUE;
    }
    return town_db[id].tax; // avg = constant
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> temp(town_db.size()); //To avoid push_back...
    int i = 0;
    for (auto c:town_db) {
        temp[i] = c.first;
        i++;
    }
    return temp;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> temp;
    for (auto c:town_db) { // avg = n aka linear
        if (c.second.town_name == name) {
            temp.push_back(c.first); // avg == constant
        }
    }
    return temp;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if (town_db.find(id) == town_db.end()) { // avg = constant
        return false;
    }

    town_db[id].town_name = newname; // avg = constant
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    // I guess this is where choosing unordered map backfires.
    // First we iterate the town_db and push pair(TownID, Name)
    // to vector...
    std::vector<std::pair<TownID, Name>> temp(town_db.size());
    int i = 0;
    for (auto c:town_db) { // average = n aka linear
        temp[i] = std::make_pair(c.second.town_id, c.second.town_name);
        i++;
    }

    // ...and then we utilize lambda to sort the vector by Name.
    std::sort(temp.begin(), temp.end(), [](std::pair<TownID, Name> a, std::pair<TownID, Name> b)
    {return a.second < b.second;}); // avg = n*logn

    // After sorting it, we need to take out the TownIDs to separate vector to
    // satisfy function call's return value
    std::vector<TownID> vector_to_return(temp.size());
    i = 0;
    for (auto c:temp) { //avg = n aka linear
        vector_to_return[i] = c.first;
        i++;
    }
    return vector_to_return;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Again iterating over town_db, taking out TownID and coords
    // Calculating distance based on coords and making a pair(TownID, distance)
    // for the vector.
    std::vector<std::pair<TownID, int>> temp;
    for (const auto &c: town_db) { // linear n
        auto x_coord = c.second.coords.x;
        auto y_coord = c.second.coords.y;
        // Not taking the sqrt() here, because it makes no
        // difference in their relative order
        int dist = (x_coord * x_coord) + (y_coord * y_coord);
        temp.push_back(std::make_pair(c.first, dist)); // constant
    }

    //Sort by distance using lambda.
    std::sort(temp.begin(), temp.end(), [](const auto &a, const auto &b) {return a.second < b.second;}); // nlogn

    std::vector<TownID> vector_to_return;
    for (const auto &c: temp) { //linear
        vector_to_return.push_back(c.first);
    }
    return vector_to_return;
}

TownID Datastructures::min_distance()
{
    if (town_db.empty()) { // avg = constant
        return NO_TOWNID;
    }

    // Not taking the sqrt() here, because it makes no
    // difference in the comparison
    auto pred = [](const auto& a, const auto& b) {
        auto ax = a.second.coords.x;
        auto ay = a.second.coords.y;
        auto bx = b.second.coords.x;
        auto by = b.second.coords.y;
        return ((ax * ax) + (ay * ay)) < ((bx * bx) + (by * by));
    };

    // Find the min element and return its id
    auto it = std::min_element(town_db.begin(), town_db.end(), pred); // exactly linear n

    TownID id = it->first;
    return id;
}

TownID Datastructures::max_distance()
{
    if (town_db.empty()) { // avg = constant
        return NO_TOWNID;
    }

    // Not taking the sqrt() here, because it makes no
    // difference in the comparison
    auto pred = [](const auto& a, const auto& b) {
        auto ax = a.second.coords.x;
        auto ay = a.second.coords.y;
        auto bx = b.second.coords.x;
        auto by = b.second.coords.y;
        return ((ax * ax) + (ay * ay)) < ((bx * bx) + (by * by));
    };

    // Find the max element and return its id
    auto it = std::max_element(town_db.begin(), town_db.end(), pred); // exactly linear n

    TownID id = it->first;
    return id;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if (town_db.find(vassalid) == town_db.end() || town_db.find(masterid) == town_db.end()) { // avg = constant
        return false;
    }

    if (town_db[vassalid].master != nullptr) { //avg = constant
        return false;
    }

    town_db[vassalid].master = &town_db[masterid]; //avg = constant
    town_db[masterid].vassals.push_back(&town_db[vassalid]);
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> temp;

    if (town_db.find(id) == town_db.end()) { //constant
        temp.push_back(NO_TOWNID);
    } else {
        for (auto &c: town_db[id].vassals) { //linear
            temp.push_back(c->town_id);      //constant
        }
    }
    return temp;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> temp;

    if (town_db.find(id) == town_db.end()) { //if id is not found
        temp.push_back(NO_TOWNID); // constant
    } else {
        temp.push_back(town_db[id].town_id); // constant

        //keep "climbing up" the pointers of masters in each city
        //Add the first master, then replace original id with master's id
        //and check if it has a master etc. When we find master = nullptr
        //we are done.
        while (town_db[id].master != nullptr) { // linear
            temp.push_back(town_db[id].master->town_id); // constant
            id = town_db[id].master->town_id;
        }
    }
    return temp;
}

bool Datastructures::remove_town(TownID id)
{
    if (town_db.find(id) == town_db.end()) { //constant
        return false;
    }

    auto town = town_db[id];

    // No master BUT has vassals -> vassals' master
    // will be set to nullptr
    if (!town.vassals.empty()) {
        if (town.master == nullptr) {
            for (auto &c:town.vassals ) { //linear
                town_db[c->town_id].master = nullptr;
            }

    // Town has a master -> town's previous vassals
    // will obey town's previous master as their new master
        } else if (town.master != nullptr) {
            auto master = town.master;
            for (auto &c:town.vassals) { //linear
                c->master = nullptr;
                add_vassalship(c->town_id, master->town_id);
            }
            // finally iterate master's vassals to find and remove the TownID id
            // so no funky pointers are left behind
            for (auto &c : master->vassals) { //linear
                if (c->town_id == id) {
                    master->vassals.erase(std::find(master->vassals.begin(), master->vassals.end(), c)); //constant
                }
            }
        }
    }
    town_db.erase(id);

    return true;
}


std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    // Much like towns_distance_increasing, except
    // we "move the origo" by subtracting the provided coordinates
    std::vector<std::pair<TownID, uint>> temp;
    for (const auto &c: town_db) { //linear
        uint x_coord = c.second.coords.x - coord.x;
        uint y_coord = c.second.coords.y - coord.y;
        int dist = (x_coord * x_coord) + (y_coord * y_coord);
        temp.push_back(std::make_pair(c.first, dist));
    }

    std::sort(temp.begin(), temp.end(), [](const auto &a, const auto &b) {return a.second < b.second;}); //nlogn

    std::vector<TownID> vector_to_return;
    for (const auto &c: temp) { //linear
        vector_to_return.push_back(c.first);
    }
    return vector_to_return;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{

    std::vector<TownID> town_chain;

    if (town_db.find(id) == town_db.end()) {
        town_chain.push_back(NO_TOWNID);

    } else {
        //Find the furthest = furthest_town(...) and keep climbing "upwards" using the pointer
        // to master. Makes it more pleasant this way, because there can only be
        // one master max.
        TownID furthest = furthest_town(id);

        Town node = town_db[furthest];
        while (node.town_id != id) {
            town_chain.push_back(node.town_id);
            node = town_db[node.master->town_id];
        }
        town_chain.push_back(town_db[id].town_id);

        //Should have probably utilized deque here, but out of time now...
        std::reverse(town_chain.begin(), town_chain.end()); //exactly n/2...so n?

    }
    return town_chain;

}

int Datastructures::total_net_tax(TownID /*id*/)
{
    throw NotImplemented("total_net_tax()");
}


TownID Datastructures::furthest_town(TownID id, int temp, int distance, TownID id_return)
{
    Town temp_town = town_db[id];

    // Keep going deeper recursively. Keep track of the deepest node in the chain.
    if (temp > distance) {
        distance = temp;
        id_return = temp_town.town_id;
    }

    if (!temp_town.vassals.empty()) {
        for (const auto &vassal: temp_town.vassals) { //linear
            return furthest_town(vassal->town_id, temp +1, distance, id_return);
        }
    } else {
        // If there are no vassals, just return town's id
        return id;
    }
    return id_return;
}



//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    for (auto &c : town_db) {  //average n, where n is town count
        c.second.roads_to.clear();
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> roads;
    std::set<std::pair<TownID, TownID>> road_set;
    TownID id1 = NO_TOWNID;
    TownID id2 = NO_TOWNID;

    for (auto &c : town_db) { // 2 nested for-loops gg, game over. n²
        for (auto &target_city : c.second.roads_to) {
            int comp = target_city.first->town_id.compare(c.second.town_id);
            if (comp < 0) {
                id1 = target_city.first->town_id;
                id2 = c.second.town_id;
            } else {
                id1 = c.second.town_id;
                id2 = target_city.first->town_id;
            }
            std::pair road{id1, id2};
            roads.push_back(road);

        }
    }

    sort(roads.begin(), roads.end());
    auto last = std::unique(roads.begin(), roads.end());
    roads.erase(last, roads.end());
    return roads;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    if (town_db.find(town1) == town_db.end() ||
            town_db.find(town2) == town_db.end()) {
        return false;
    }

    Town *temp1 = &town_db[town1];
    Town *temp2 = &town_db[town2];
    if (temp1->roads_to.find(temp2) !=
            temp1->roads_to.end()) {
        return false;
    }


    int x_1 = temp1->coords.x;
    int y_1 = temp1->coords.y;

    int x_2 = temp2->coords.x;
    int y_2 = temp2->coords.y;

    int x = x_1 - x_2;
    int y = y_1 - y_2;

    int road_length = std::hypot(x, y); //not 100% sure, but probably
                                        // average n because of sqrt?


    temp1->roads_to[temp2] = road_length;
    temp2->roads_to[temp1] = road_length;


    // Sort and insert the road also to our ancillary data structure for
    // later purposes...
    Town* first = temp1;
    Town* second = temp2;
    if (temp1->town_id > temp2->town_id) {
        first = temp2;
        second = temp1;
    }

    auto pair = std::make_pair(first, second);
    road_db.insert(std::make_pair(road_length, pair)); //"...logarithmic in size in general"

    return true;

}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    Town* temp = &town_db[id];
    std::vector<TownID> roads_from(temp->roads_to.size());

    int i = 0;
    for (auto &c : temp->roads_to) { //average n
        roads_from[i] = c.first->town_id;
        i++;
    }

    return roads_from;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    // Use depth_first_search to form a route and return the route as a vector
    std::vector<TownID> route;

    if (town_db.find(fromid) == town_db.end() || town_db.find(toid) == town_db.end() ) {
        route.push_back(NO_TOWNID);
    } else {
        reset_town_flags(); //average n
        route = depth_first_search(fromid, toid, route); //recursive without a loop...so linear n?

        // if the last element is the desired city, we know the route was found...else return
        // empty vector.
        if (*route.rbegin() != toid) {
            route.clear();
            route.push_back(NO_TOWNID);
        }
    }

    return route;
}

std::vector<TownID> Datastructures::depth_first_search(TownID fromid, TownID toid, std::vector<TownID> route)
{

    // Recursive DFS to find a route between 2 points. Any route w
    Town* from_town = &town_db[fromid];

    from_town->color = "gray";
    route.push_back(fromid);

    for (auto &c : from_town->roads_to) {

        Town* to_town = &town_db[c.first->town_id];
        if (to_town->color == "white") {
            auto node_list = depth_first_search(to_town->town_id, toid, route);

            if (*node_list.rbegin() == toid) { // function with a for-loop recursively calling itself is probably n²?
                return node_list;
            };
        }

        else if (to_town->color == "gray") {
            continue;
        }

    }

    from_town->color = "black";
    return route;

}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    if (town_db.find(town1) == town_db.end() || town_db.find(town2) == town_db.end() ) {
        return false;
    }

    Town* city1 = &town_db[town1];
    Town* city2 = &town_db[town2];

    if (city1->roads_to.find(city2) == city1->roads_to.end()) {
        return false;
    }

    // If we made it here without running to return false, then we know
    // the road actually exists and we need to erase it from both towns'
    // roads_to-vector

    city1->roads_to.erase(city2);
    city2->roads_to.erase(city1);


    // want to arrange them always the same way to pairs and into multimap
    Town* first = city1;
    Town* second = city2;

    if (city1->town_id > city2->town_id) {
        first = city2;
        second = city1;
    }

    // make sure to remove from the ancillary data structure as well
    auto pair = std::make_pair(first, second);
    for (auto &c : road_db) {  // average linear
        if (c.second == pair) {
            road_db.erase(c.first);
            break;
        }
    }

    return true;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    std::vector<TownID> route;

    if (town_db.find(fromid) == town_db.end() || town_db.find(toid) == town_db.end() ) {
        route.push_back(NO_TOWNID);
        return route;
    }

    reset_town_flags();

    // If a route was found, start from "toid" and backtrack the route
    // using "arrived_from" and push it to vector "route". If no
    // route is found, return empty vector.
    if (breath_first_search(fromid, toid)) { // 2 nested loops...n² it is
        Town* town_node = &town_db[toid];
        while (town_node->arrived_from != NO_TOWNID) {            
            route.push_back(town_node->town_id);
            town_node = &town_db[town_node->arrived_from];
        }
        route.push_back(town_node->town_id);
    }
    std::reverse(route.begin(), route.end());
    return route;
}

bool Datastructures::breath_first_search(TownID fromid, TownID toid)
{
    // Following the logic of BFS, we keep increasing each city's
    // distance to source. If we find "toid", we know we arrived using
    // the (or "a", could be several of similar length) shortest route.
    // If found, return true, else return false. Using this method
    // should also take care of the "what if no route is found?" -problem.

    Town* from_town = &town_db[fromid];
    Town* target_town = nullptr;

    from_town->color = "gray";
    from_town->source_dist = 0;

    std::queue<Town*> Q;
    Q.push(from_town);

    while(!Q.empty()) {
        auto u = Q.front();
        Q.pop();

        for (auto &c : u->roads_to) {  // 2 nested loops...n² it is
            target_town = &town_db[c.first->town_id];
            if (target_town->color == "white") {
                target_town->color = "gray";
                target_town->source_dist = u->source_dist + 1;
                target_town->arrived_from = u->town_id;
                Q.push(target_town);

                if (target_town->town_id == toid) {
                    return true;
                }
            }
        }
        u->color = "black";
    }
    return false;
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    reset_town_flags();

    std::vector<TownID> towns;
    if (depth_first_cycle(startid)) { // this is n²
        auto city = town_db[startid];
        towns.push_back(city.town_id);

        while (!city.loop_cause) {
            towns.push_back(city.went_to->town_id);
            city = *city.went_to;
        }
        towns.push_back(city.went_to->town_id);
        return towns;
    }
    return towns;
}

bool Datastructures::depth_first_cycle(TownID startid)
{
    // Using depth first, start from startid and try to find a cycle.
    // If cycle is found, return true and make note of which city was the
    // one to form a cycle by connecting to an already visited city.

    // Keep track of the path by updating the "went_to" pointer for each city.

    // Using the recursive approach
    Town* town = &town_db[startid];
    town->color = "gray";

    for (auto &c : town->roads_to) {
        Town* target =  c.first;
        if (target->color == "white") {
            town->went_to = c.first;
            if (depth_first_cycle(c.first->town_id)) { //recursive with for-loop smells like n²
                return true;
            }
        }
        else if (target->went_to->color == "gray") {

            if (target->went_to != &town_db[startid]) {
                town->loop_cause = true;
                town->went_to = c.first;
                return true;
            }
            else {
                continue;
            }
        }
    }
    town->color = "black";
    return false;
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{

    std::vector<TownID> route;

    if (dijkstra_enhanced(fromid, toid)) { // 2 nested loops -> n²
        Town* temp = &town_db[toid];
        Town* start = &town_db[fromid];
        start->arrived_from = NO_TOWNID;
        while (temp->arrived_from != NO_TOWNID) {
            route.push_back(temp->town_id);
            temp = &town_db[temp->arrived_from];
        }
        route.push_back(fromid);
    }

    std::reverse(route.begin(), route.end());
    return route;
}

bool Datastructures::dijkstra_enhanced(TownID startid, TownID goalid)
{

    // This function is supposed to be A*...
    // Utilize priority queue, count priorities for white and
    // gray nodes, break and return if we arrive at our goalid

    reset_town_flags();

    Town* start_town = &town_db[startid];
    Town* goal = &town_db[goalid];

    std::priority_queue<std::pair<uint, Town*>> Q;
    start_town->color = "gray";
    start_town->dijkstra_d = 0;

    // First use d (or rather -d) as priority. Later we use de
    auto pair = std::make_pair(start_town->dijkstra_d, start_town);
    Q.push(pair);

    Town* u = nullptr;

        while (!Q.empty()) {
        u = Q.top().second;
        Q.pop();

        // Go through adjacent cities connected by a road...
        for (auto &v : u->roads_to) { // 2 nested loops -> n²
            if (v.first == goal) {
                goal->arrived_from = u->town_id;
                return true;
            }
            relax_A(*u, *v.first, *goal);

            // If the node has not been visited...
            if (v.first->color == "white") {                
                v.first->color == "gray";
                auto pair = std::make_pair(v.first->dijkstra_de, v.first);
                v.first->arrived_from = u->town_id;
                Q.push(pair);                
            }

            // If the node was already visited BUT the priority changed....
            else if ( v.first->color == "gray" && v.first->re_relaxed ) {
                v.first->arrived_from = u->town_id;
                auto pair = std::make_pair(v.first->dijkstra_de, v.first);
                Q.push(pair);
                v.first->re_relaxed = false;
            }

            else {
                continue;
            }
        }
        u->color = "black";
    }
    return false;
}

void Datastructures::relax_A(Datastructures::Town &u, Datastructures::Town &v, Datastructures::Town &goal)
{
    uint cost_u_d = u.roads_to[&v];
    if (v.dijkstra_d < (u.dijkstra_d - cost_u_d)) {
        // if we found a cheaper path, update the cost and estimated cost de for priority purposes
        v.arrived_from = u.town_id;
        v.dijkstra_d = u.dijkstra_d - cost_u_d;


        int x_1 = v.coords.x;
        int y_1 = v.coords.y;

        int x_2 = goal.coords.x;
        int y_2 = goal.coords.y;

        int x = x_1 - x_2;
        int y = y_1 - y_2;

        int minimum_est = std::hypot(x, y);

        v.dijkstra_de = v.dijkstra_d - minimum_est;
        //u.went_to = &v;

        if (v.color == "gray") {
            // If priority is changed for a gray node
            v.re_relaxed = true;
        }

    }
    return;
}

void Datastructures::reset_town_flags()
{
    for (auto &c : town_db) {
        c.second.color = "white";
        c.second.arrived_from = NO_TOWNID;
        c.second.source_dist = -1;
        c.second.went_to = nullptr;
        c.second.loop_cause = false;

        // We want to use negative values with Dijkstra/A*
        // as taught on the lecture video. Dirty but neat trick.
        // Hence we initiate them to positive 1 to say
        // "never been here".

        c.second.dijkstra_d = 1;
        c.second.dijkstra_de = 1;
        c.second.re_relaxed = false;
    }
}

Distance Datastructures::trim_road_network()
{
    // This is trying to be Kruskal's algorithm
    // Basically, we have a data structure road_db
    // containing information of every road (stored only once)
    // and its length. This structure is ordered by the road length automatically.

    // We clear all the roads from map first. Then we start pulling
    // one road at a time from road_db, going into each town's struct data and connecting
    // it to the appropiate city. After each addition, check if we formed a loop.
    // If the latest addition formed a loop, cancel the addition and move forward to adding
    // next road instead.

    // Doing this should give the shortest possible total road length while maintaining
    // a direct or an indirect connection between each city without any loops.
    clear_roads();
    Town* town1 = nullptr;
    Town* town2 = nullptr;

    for (auto &road : road_db) {
        reset_town_flags(); // for-loop inside this function, so 2 nested for-loops
        town1 = road.second.first;
        town2 = road.second.second;
        auto length = road.first;

        town1->roads_to[town2] = length; //connect the cities
        town2->roads_to[town1] = length;

        if (depth_first_cycle(town1->town_id)) { //check for any cycles after each addition
            town1->roads_to.erase(town2);
            town2->roads_to.erase(town1);
        }
    }

    int total_length = 0;
    Town* temp1 = nullptr;
    Town* temp2 = nullptr;

    // Count total length to be returned.
    // Only use the roads that actually remained connected, which means
    // we have to check every road from road_db whether they remained
    // connected or not.
    for (auto &c : road_db) {
        temp1 = c.second.first;
        temp2 = c.second.second;
        if (temp1->roads_to.count(temp2) == 1) {
            total_length += c.first;
        }
    }

    return total_length;

}



