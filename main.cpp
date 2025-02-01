#include "vector.h" // Pfad anpassen, falls nötig
#include <iostream>

int main() {
  std::cout << "===== Test 1: push_back, size, capacity =====\n";
  Vector<int> v;
  std::cout << "Initial size: " << v.size() << ", capacity: " << v.capacity()
            << "\n";

  for (int i = 0; i < 12; ++i) {
    v.push_back(i);
    std::cout << "Pushed back " << i << " -> size: " << v.size()
              << ", capacity: " << v.capacity() << "\n";
  }
  std::cout << "v = " << v << "\n\n";

  std::cout << "===== Test 2: operator[] und out_of_range überprüfen =====\n";
  std::cout << "v[0] = " << v[0] << "\n";
  std::cout << "v[5] = " << v[5] << "\n";
  try {
    std::cout << "v[999] = " << v[999] << "\n"; // Sollte Exception werfen
  } catch (const std::runtime_error &e) {
    std::cerr << "Caught exception (as expected): " << e.what() << "\n";
  }
  std::cout << "\n";

  std::cout << "===== Test 3: pop_back =====\n";
  std::cout << "Current vector: " << v << "\n";
  v.pop_back();
  v.pop_back();
  std::cout << "After 2x pop_back: " << v << "\n";
  std::cout << "size: " << v.size() << ", capacity: " << v.capacity() << "\n";
  // Test pop_back bei leerem Vektor
  {
    Vector<int> emptyVec;
    try {
      emptyVec.pop_back();
    } catch (const std::runtime_error &e) {
      std::cerr << "Caught exception (as expected): " << e.what() << "\n";
    }
  }
  std::cout << "\n";

  std::cout << "===== Test 4: insert und erase =====\n";
  Vector<int>::iterator it = v.begin();
  // Insert am Anfang
  it = v.insert(it, 999);
  std::cout << "After insert(999) at begin: " << v << "\n";

  // Insert in der Mitte
  it = v.begin();
  std::advance(it, 5); // gehe 5 Elemente weiter
  it = v.insert(it, 777);
  std::cout << "After insert(777) at position 5: " << v << "\n";

  // Erase an der gleichen Stelle
  it = v.erase(it);
  std::cout << "After erase(pos) (sollte 777 entfernt haben): " << v << "\n\n";

  std::cout << "===== Test 5: shrink_to_fit und reserve =====\n";
  std::cout << "Current size: " << v.size() << ", capacity: " << v.capacity()
            << "\n";
  std::cout << "Shrink to fit...\n";
  v.shrink_to_fit();
  std::cout << "size: " << v.size() << ", capacity: " << v.capacity() << "\n";
  std::cout << "Reserve(30)...\n";
  v.reserve(30);
  std::cout << "size: " << v.size() << ", capacity: " << v.capacity() << "\n\n";

  std::cout << "===== Test 6: clear =====\n";
  v.clear();
  std::cout << "After clear: " << v << "\n";
  std::cout << "size: " << v.size() << ", capacity: " << v.capacity() << "\n\n";

  std::cout << "===== Test 7: Iterator loop (manuell mit push_back) =====\n";
  for (int i = 0; i < 5; ++i) {
    v.push_back(i * 2);
  }
  std::cout << "v = " << v << "\n";
  std::cout << "Iteriere per iterator:\n";
  for (auto it2 = v.begin(); it2 != v.end(); ++it2) {
    std::cout << *it2 << " ";
  }
  std::cout << "\n\n";

  std::cout << "Alle informellen Tests abgeschlossen.\n";
  return 0;
}