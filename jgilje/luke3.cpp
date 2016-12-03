#include <iostream>
#include <fstream>

#include <unordered_map>
#include <unordered_set>

#include <algorithm>

struct Person {
    std::string name;
    std::unordered_set<std::string> friends;
    std::unordered_set<std::string> hates;
    int chameleon = 0;
};

static std::unordered_map<std::string, Person> persons;

static void ensure_person(const std::string a) {
    if (persons.count(a) == 0) {
        persons[a] = Person{a, {}, {}};
    }
}

static void ensure_persons(const std::string a, const std::string b) {
    ensure_person(a);
    ensure_person(b);
}

static void read(std::string file) {
    std::ifstream f(file);

    std::string a, b, c;

    while (f >> a >> b >> c) {
        if (a == "friends") {
            ensure_persons(b, c);
            persons[b].friends.emplace(c);
            persons[c].friends.emplace(b);
        } else if (b == "hates") {
            ensure_persons(a, c);
            persons[a].hates.emplace(c);
        } else {
            throw "Unknown input line";
        }
    }
}

static bool is_mutual_friends(const std::string& a, const std::string& b) {
    return persons[b].friends.count(a) > 0;
}

static bool is_chameleon(const std::string& a, const std::string& b) {
    return (persons[a].hates.count(b) > 0) && (persons[b].hates.count(a) == 0);
}

static void calc() {
    std::for_each(persons.begin(), persons.end(), [](auto& it) {
        auto& person = it.second;
        std::for_each(person.friends.begin(), person.friends.end(), [&](auto& other) {
            if (is_mutual_friends(person.name, other) && is_chameleon(person.name, other)) {
                person.chameleon++;
            }
        });
    });
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Missing input file" << std::endl;
        return 1;
    }

    read(argv[1]);
    calc();

    auto max_element = std::max_element(persons.begin(), persons.end(), [](auto& a, auto& b) {
        return a.second.chameleon < b.second.chameleon;
    });
    std::cout << max_element->first << std::endl;

    return 0;
}
