#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "fuzzy_matching.hpp"

void TestAll();

int main(int argc, char* argv[]) {
  if (argc > 1 && strcmp(argv[1], "--test") == 0) {
    TestAll();
    return 0;
  }
  const char kWildcard = '?';
  const std::string kPatternWithWildcards = ReadString(std::cin);
  const std::string kText = ReadString(std::cin);
  Print(FindFuzzyMatches(kPatternWithWildcards, kText, kWildcard));
  return 0;
}

// ===== TESTING ZONE =====

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
  std::copy(vector.begin(), vector.end(), std::ostream_iterator<T>(os, " "));
  return os;
}

class TestNotPassedException : public std::runtime_error {
 public:
  explicit TestNotPassedException(const char* what)
      : std::runtime_error(what) {}

  explicit TestNotPassedException(const std::string& what)
      : std::runtime_error(what.c_str()) {}
};

#define REQUIRE_EQUAL(first, second)                                         \
  do {                                                                       \
    auto firstValue = (first);                                               \
    auto secondValue = (second);                                             \
    if (!(firstValue == secondValue)) {                                      \
      std::ostringstream oss;                                                \
      oss << "Require equal failed: " << #first << " != " << #second << " (" \
          << firstValue << " != " << secondValue << ")\n";                   \
      throw TestNotPassedException(oss.str());                               \
    }                                                                        \
  } while (false)

void TestMatches();
void TestMatchesWithEmptyString();
void TestMatchesWithWildcards();
void TestBfsHalt();
void TestBfsOrder();
void TestSplit();

void TestAll() {
  TestMatches();
  TestMatchesWithEmptyString();
  TestMatchesWithWildcards();
  TestSplit();
  TestBfsHalt();
  TestBfsOrder();
  std::cerr << "Tests are passed!\n";
}

void TestMatches() {
  NAhoCorasick::AutomatonBuilder builder;
  builder.Add("suffix", 1);
  builder.Add("ffix", 2);
  builder.Add("ix", 3);
  builder.Add("abba", 4);

  std::unique_ptr<NAhoCorasick::Automaton> automaton = builder.Build();

  const std::string kText = "let us find some suffix";
  NAhoCorasick::NodeReference node = automaton->Root();
  for (char ch : kText) {
    node = node.Next(ch);
  }
  std::vector<size_t> string_ids;

  node.GenerateMatches(
      [&string_ids](size_t string_id) { string_ids.push_back(string_id); });
  std::sort(string_ids.begin(), string_ids.end());

  REQUIRE_EQUAL(string_ids, std::vector<size_t>({1, 2, 3}));
}

void TestMatchesWithEmptyString() {
  NAhoCorasick::AutomatonBuilder builder;
  builder.Add("", 1);
  builder.Add("", 2);
  builder.Add("t", 3);
  builder.Add("", 4);

  auto automaton = builder.Build();
  const std::string kText = "test";
  NAhoCorasick::NodeReference node = automaton->Root();

  std::vector<size_t> sizes{4, 3, 3, 4};
  for (size_t i = 0; i < kText.size(); ++i) {
    node = node.Next(kText[i]);
    std::set<int> matches;
    node.GenerateMatches([&matches](size_t id) { matches.insert(id); });
    REQUIRE_EQUAL(sizes[i], matches.size());
  }
}

void TestMatchesWithWildcards() {
  WildcardMatcher matcher = WildcardMatcher::BuildFor("a?c?", '?');

  {
    std::vector<size_t> occurrences;
    //                              012345678901234
    const std::string kFirstText = "abcaaccxaxcxacc";
    for (size_t i = 0; i < kFirstText.size(); ++i) {
      matcher.Scan(kFirstText[i],
                   [&occurrences, i]() { occurrences.push_back(i); });
    }

    REQUIRE_EQUAL(occurrences, std::vector<size_t>({3, 6, 7, 11}));
  }
  {
    matcher.Reset();
    std::vector<size_t> occurrences;
    const std::string kSecondText = "xyzadcc";
    for (size_t i = 0; i < kSecondText.size(); ++i) {
      matcher.Scan(kSecondText[i],
                   [&occurrences, i]() { occurrences.push_back(i); });
    }

    REQUIRE_EQUAL(occurrences, std::vector<size_t>({6}));
  }
}

void TestSplit() {
  {
    auto is_dash = [](char ch) { return ch == '-'; };
    {
      REQUIRE_EQUAL(Split("a--b-cd-e", is_dash),
                    std::vector<std::string>({"a", "", "b", "cd", "e"}));
    }
    {
      REQUIRE_EQUAL(Split("-", is_dash), std::vector<std::string>({"", ""}));
    }
    {
      REQUIRE_EQUAL(Split("--abc--", is_dash),
                    std::vector<std::string>({"", "", "abc", "", ""}));
    }
    {
      REQUIRE_EQUAL(Split("ab", is_dash), std::vector<std::string>({"ab"}));
    }
    {
      REQUIRE_EQUAL(Split("", is_dash), std::vector<std::string>({""}));
    }
  }
  {
    auto any_char = [](char /*ch*/) { return true; };
    {
      std::string str = "2f1tgyhnjd";
      // empty string before each character and one after the string
      REQUIRE_EQUAL(Split(str, any_char).size(), str.size() + 1);
    }
  }
}

namespace NTestBfs {

typedef std::vector<int> VerticesList;

struct Graph {
  std::vector<VerticesList> adjacent_vertices;
};

int GetTarget(const Graph& /*graph*/, int edge) { return edge; }

enum class BfsEvent { KDiscoverVertex, KExamineEdge, KExamineVertex };

std::ostream& operator<<(std::ostream& out, const BfsEvent& event) {
  switch (event) {
    case BfsEvent::KDiscoverVertex:
      out << "discover";
      break;
    case BfsEvent::KExamineEdge:
      out << "examine edge";
      break;
    case BfsEvent::KExamineVertex:
      out << "examine vertex";
      break;
  }
  return out;
}

std::ostream& operator<<(std::ostream& os,
                         const std::pair<BfsEvent, int>& item) {
  os << "(" << item.first << ", " << item.second << ")";
  return os;
}

class TestBfsVisitor : public NTraverses::BfsVisitor<int, int> {
 public:
  explicit TestBfsVisitor(std::vector<std::pair<BfsEvent, int>>* events) {
    events_ = events;
  }

  virtual void DiscoverVertex(int vertex) override {
    events_->emplace_back(BfsEvent::KDiscoverVertex, vertex);
  }
  virtual void ExamineEdge(const int& edge) override {
    events_->emplace_back(BfsEvent::KExamineEdge, edge);
  }
  virtual void ExamineVertex(int vertex) override {
    events_->emplace_back(BfsEvent::KExamineVertex, vertex);
  }

 private:
  std::vector<std::pair<BfsEvent, int>>* events_;
};
}  // namespace NTestBfs

void TestBfsHalt() {
  NTestBfs::Graph graph;
  graph.adjacent_vertices.resize(4);
  const int kA = 0;
  const int kB = 1;
  const int kC = 2;
  const int kD = 3;
  graph.adjacent_vertices[kA].push_back(kB);
  graph.adjacent_vertices[kB].push_back(kC);
  graph.adjacent_vertices[kC].push_back(kD);
  graph.adjacent_vertices[kD].push_back(kB);
  NTraverses::BreadthFirstSearch(kA, graph, NTraverses::BfsVisitor<int, int>());
  // BreadthFirstSearch should not hang on a graph with a cycle
}

void TestBfsOrder() {
  using namespace NTestBfs;
  Graph graph;
  graph.adjacent_vertices.resize(4);
  const int kA = 0;
  const int kB = 1;
  const int kC = 2;
  const int kD = 3;
  graph.adjacent_vertices[kA].push_back(kB);
  graph.adjacent_vertices[kA].push_back(kC);
  graph.adjacent_vertices[kB].push_back(kD);
  graph.adjacent_vertices[kC].push_back(kD);
  graph.adjacent_vertices[kD].push_back(kA);

  std::vector<std::pair<BfsEvent, int>> events;
  TestBfsVisitor visitor(&events);
  NTraverses::BreadthFirstSearch(kA, graph, visitor);

  std::vector<std::pair<BfsEvent, int>> expected{
      {BfsEvent::KDiscoverVertex, kA}, {BfsEvent::KExamineVertex, kA},
      {BfsEvent::KExamineEdge, kB},    {BfsEvent::KDiscoverVertex, kB},
      {BfsEvent::KExamineEdge, kC},    {BfsEvent::KDiscoverVertex, kC},
      {BfsEvent::KExamineVertex, kB},  {BfsEvent::KExamineEdge, kD},
      {BfsEvent::KDiscoverVertex, kD}, {BfsEvent::KExamineVertex, kC},
      {BfsEvent::KExamineEdge, kD},    {BfsEvent::KExamineVertex, kD},
      {BfsEvent::KExamineEdge, kA}};

  REQUIRE_EQUAL(events, expected);
}
