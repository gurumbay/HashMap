#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "Dictionary.h"
#include "LinkedList.h"

void loadDictionaryFromFile(Dictionary& dict);
void showDictionaryStats(const Dictionary& dict);
void addTranslation(Dictionary& dict);
void searchTranslation(Dictionary& dict);
void removeTranslation(Dictionary& dict);

void printList(const SortedUniqueList<std::string>& lst);
void printDictionary(const Dictionary& dict, std::size_t entriesPerPage = 5);

void runTests();
void testDictionary();
void testSortedUniqueList();

int main()
{
  std::system("chcp 1251 > nul");  // Поддержка кириллицы в Windows
  Dictionary dict;
  bool keepRunning = true;

  runTests();

  while (keepRunning)
  {
    std::cout << "\n=== DICTIONARY MENU ===\n"
      << "1. Load dictionary from file\n"
      << "2. Show dictionary statistics\n"
      << "3. Add translation\n"
      << "4. Search translation\n"
      << "5. Remove translation\n"
      << "6. Print entire dictionary\n"
      << "7. Exit\n"
      << "Choose an option: ";

    int choice;
    if (!(std::cin >> choice))
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid input. Please enter a number.\n";
      continue;
    }
    std::cin.ignore();

    switch (choice)
    {
    case 1: loadDictionaryFromFile(dict); break;
    case 2: showDictionaryStats(dict); break;
    case 3: addTranslation(dict); break;
    case 4: searchTranslation(dict); break;
    case 5: removeTranslation(dict); break;
    case 6: printDictionary(dict); break;
    case 7:
      std::cout << "Exiting program.\n";
      keepRunning = false;
      break;
    default:
      std::cerr << "Invalid option. Please try again.\n";
    }
  }
  return EXIT_SUCCESS;
}

void loadDictionaryFromFile(Dictionary& dict)
{
  std::string filename;
  std::cout << "Enter filename: ";
  std::getline(std::cin, filename);

  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open file '" << filename << "'.\n";
    return;
  }

  std::string line, englishWord, russianWord;
  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    if (std::getline(iss, englishWord, '-') && std::getline(iss, russianWord))
    {
      englishWord.erase(englishWord.find_last_not_of(" ") + 1);
      russianWord.erase(0, russianWord.find_first_not_of(" "));
      dict.insert(englishWord, russianWord);
    }
    else
    {
      std::cerr << "Error: Invalid format in line: " << line << "\n";
    }
  }

  std::cout << "Dictionary successfully loaded from '" << filename << "'.\n";
}

void showDictionaryStats(const Dictionary& dict)
{
  std::cout << "\n--- DICTIONARY STATISTICS ---\n";
  std::cout << "Total Entries: " << dict.size() << "\n";
  std::cout << "Load Factor: " << dict.loadFactor() << "%\n";
}

void addTranslation(Dictionary& dict)
{
  std::string englishWord, russianWord;
  std::cout << "Enter English word: ";
  std::getline(std::cin, englishWord);
  std::cout << "Enter Russian translation: ";
  std::getline(std::cin, russianWord);

  dict.insert(englishWord, russianWord);
  std::cout << "Translation added: " << englishWord << " -> " << russianWord << "\n";
}

void searchTranslation(Dictionary& dict)
{
  std::string englishWord;
  std::cout << "Enter English word to search: ";
  std::getline(std::cin, englishWord);

  const auto search = dict.find(englishWord);
  if (search != dict.end())
  {
    std::cout << englishWord << ": ";
    printList(search->second);
  }
  else
  {
    std::cout << "No translations found for '" << englishWord << "'.\n";
  }
}

void removeTranslation(Dictionary& dict)
{
  std::string englishWord, russianWord;
  std::cout << "Enter English word: ";
  std::getline(std::cin, englishWord);
  std::cout << "Enter Russian translation to remove: ";
  std::getline(std::cin, russianWord);

  dict.remove(englishWord, russianWord);
  std::cout << "Translation removed (if it existed).\n";
}

void runTests()
{
  std::cout << "\nRunning basic tests...\n";
  testDictionary();
  testSortedUniqueList();
  std::cout << "Tests completed.\n";
}

void printDictionary(const Dictionary& dict, std::size_t entriesPerPage)
{
  if (dict.empty())
  {
    std::cout << "Dictionary is empty.\n";
    return;
  }

  std::size_t totalEntries = dict.size();
  std::size_t totalPages = (totalEntries + entriesPerPage - 1) / entriesPerPage;
  std::size_t currentPage = 0;
  std::size_t count = 0;
  std::string choice;

  std::cout << "--- DICTIONARY CONTENT ---\n";

  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    if (count % entriesPerPage == 0)
    {
      currentPage++;
      std::cout << "Page " << currentPage << " of " << totalPages << ":\n";
    }

    std::cout << it->first << ": ";
    printList(it->second);

    if (++count % entriesPerPage == 0 && count != totalEntries)
    {
      std::cout << "--- Press Enter to continue or 'Q' to quit ---\n";
      std::getline(std::cin, choice);

      if (choice == "Q" || choice == "q") break;
    }
  }

  if (count % entriesPerPage != 0)
  {
    std::cout << "\nEnd of dictionary. Page " << currentPage << " of " << totalPages << ".\n";
  }
}

void printList(const SortedUniqueList<std::string>& lst)
{
  for (auto it = lst.cbegin(); it != lst.cend(); ++it)
  {
    std::cout << *it << " ";
  }
  std::cout << "\n";
}

void testDictionary()
{
  Dictionary dict;

  // Test 1: Insert elements
  dict.insert("hello", "привет");
  dict.insert("world", "мир");
  dict.insert("world", "земля");

  assert(dict.size() == 2);
  assert(dict.find("hello") != dict.end());
  assert(dict.find("world")->second.size() == 2);

  // Test 2: Remove specific translation
  dict.remove("world", "земля");
  auto translations = dict.find("world");
  assert(translations->second.size() == 1);

  // Test 3: Remove key entirely
  dict.remove("hello", "привет");
  assert(dict.find("hello") == dict.end());

  // Test 4: Handle non-existing keys
  dict.remove("nonexistent", "value");
  assert(dict.size() == 1);

  // Test 5: Load factor
  dict.insert("key1", "value1");
  dict.insert("key2", "value2");
  assert(dict.loadFactor() > 0 && dict.loadFactor() <= 1);

  // Test 6: Clear dictionary
  dict.clear();
  assert(dict.empty());

  // Test 7: Re-insertion after clearing
  dict.insert("test", "тест");
  assert(dict.size() == 1);

  // Test 8: Handling duplicate insertions
  dict.insert("test", "тест");
  dict.insert("test", "пример");
  translations = dict.find("test");
  assert(translations->second.size() == 2);

  // Test 9: Edge cases for empty dictionary
  dict.clear();
  assert(dict.empty());
  assert(dict.find("nonexistent") == dict.end());

  std::cout << "All Dictionary tests completed successfully.\n";
}

void testSortedUniqueList()
{
  SortedUniqueList<int> list;

  // Test 1: Insert elements
  assert(list.insert(5));   // OK
  assert(list.insert(3));   // OK
  assert(list.insert(8));   // OK
  assert(!list.insert(5));  // duplicate!

  auto it = list.cbegin();
  assert(*it++ == 3);
  assert(*it++ == 5);
  assert(*it++ == 8);
  assert(it == list.cend());

  // Test 2: Remove an element
  assert(list.remove(5));
  assert(!list.remove(5));

  it = list.cbegin();
  assert(*it++ == 3);
  assert(*it++ == 8);
  assert(it == list.cend());

  // Test 3: Check empty and size
  assert(!list.empty());
  assert(list.size() == 2);

  // Test 4: Clear the list
  list.clear();
  assert(list.empty());
  assert(list.size() == 0);

  // Test 5: Insert after clearing
  assert(list.insert(10));
  assert(list.insert(1));
  assert(list.insert(5));

  it = list.cbegin();
  assert(*it++ == 1);
  assert(*it++ == 5);
  assert(*it++ == 10);
  assert(it == list.cend());

  // Test 6: Handling negative and duplicate values
  assert(list.insert(-1));
  assert(!list.insert(-1));

  it = list.cbegin();
  assert(*it++ == -1);
  assert(*it++ == 1);
  assert(*it++ == 5);
  assert(*it++ == 10);
  assert(it == list.cend());

  std::cout << "All SortedUniqueList tests passed successfully.\n";
}
