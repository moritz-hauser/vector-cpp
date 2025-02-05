#include "vector.h"
#include <iostream>
#include <string>

// Eigene Datenstruktur:
struct Person {
  std::string name;
  int age;

  Person() : name(""), age(0) {}

  Person(const std::string &n, int a) : name(n), age(a) {}
};

// Damit Person in std::cout ausgegeben werden kann:
std::ostream &operator<<(std::ostream &os, const Person &p) {
  os << "Person(" << p.name << ", " << p.age << ")";
  return os;
}

int main() {
  // 1) Erstellen eines Vektors für 'Person'
  Vector<Person> people;
  std::cout << "1) Erstelle leeren Vector<Person>\n";
  std::cout << "   size: " << people.size()
            << ", capacity: " << people.capacity() << "\n\n";

  // 2) push_back - füge mehrere Personen hinzu
  people.push_back(Person("Alice", 30));
  people.push_back(Person("Bob", 25));
  people.push_back(Person("Charlie", 35));
  std::cout << "2) Nach push_back von Alice, Bob, Charlie:\n";
  std::cout << "   people = " << people << "\n";
  std::cout << "   size: " << people.size()
            << ", capacity: " << people.capacity() << "\n\n";

  // 3) Iteration per Iterator (read-only Ausgabe)
  std::cout << "3) Iteration per Iterator:\n";
  for (auto it = people.begin(); it != people.end(); ++it) {
    std::cout << *it << "\n";
  }
  std::cout << "\n";

  // 4) insert() - füge neue Person an Position 1 ein
  {
    auto pos = people.begin();
    ++pos; // nun auf das 2. Element (Bob) zeigen
    people.insert(pos, Person("Diana", 28));
    std::cout << "4) Nach insert(Diana) an der 2. Stelle:\n"
              << "   people = " << people << "\n\n";
  }

  // 5) erase() - entferne das 3. Element
  {
    auto pos = people.begin();
    // Wir gehen zwei Schritte weiter: begin() -> 2. Person (Diana) -> 3. Person
    // (Bob)
    ++pos;
    ++pos;
    std::cout << "   Erase an Position, die aktuell " << *pos << " ist...\n";
    people.erase(pos);
    std::cout << "   people = " << people << "\n\n";
  }

  // 6) pop_back() - entferne letztes Element
  std::cout << "6) Pop Back...\n";
  people.pop_back();
  std::cout << "   people = " << people << "\n";
  std::cout << "   size: " << people.size()
            << ", capacity: " << people.capacity() << "\n\n";

  // 7) reserve() - erhöhe Kapazität auf 20
  std::cout << "7) reserve(20) ...\n";
  people.reserve(20);
  std::cout << "   size: " << people.size()
            << ", capacity: " << people.capacity() << "\n\n";

  // 8) shrink_to_fit() - reduziere Kapazität auf aktuelle size() oder MIN_SIZE
  std::cout << "8) shrink_to_fit() ...\n";
  people.shrink_to_fit();
  std::cout << "   size: " << people.size()
            << ", capacity: " << people.capacity() << "\n\n";

  // 9) Zugriff per operator[]
  std::cout << "9) Zugriff via operator[]\n";
  for (std::size_t i = 0; i < people.size(); ++i) {
    std::cout << "   people[" << i << "] = " << people[i] << "\n";
  }
  std::cout << "\n";

  // 10) clear() und Leer-Check
  std::cout << "10) clear() ...\n";
  people.clear();
  std::cout << "   people = " << people << "\n"
            << "   size: " << people.size()
            << ", capacity: " << people.capacity() << "\n";
  std::cout << "   empty? " << (people.empty() ? "yes" : "no") << "\n\n";

  return 0;
}