#include "../include/graph.hpp"

Vertex::Vertex(int _id, int _index, int _x, int _y)
    : id(_id), index(_index), x(_x), y(_y), neighbor()
{
}

Graph::Graph() : V(Vertices()), width(0), height(0) {}

Graph::~Graph()
{
  for (auto &v : V)
    if (v != nullptr) delete v;
  V.clear();
}

// to load graph
static const std::regex r_height = std::regex(R"(height\s(\d+))");
static const std::regex r_width = std::regex(R"(width\s(\d+))");
static const std::regex r_map = std::regex(R"(map)");

Graph::Graph(const std::vector<std::vector<int>> &map)
    : V(Vertices()), width(0), height(0)
{
  width = map[0].size();
  height = map.size();
  U = Vertices(width * height, nullptr);

  // create vertices
  // int index = 0;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (map[x][y] != 0) continue;  // obstacle
      auto index = width * y + x;
      auto v = new Vertex(V.size(), index, x, y);
      V.push_back(v);
      U[index] = v;
      index++;
    }
  }

  // create edges
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto v = U[width * y + x];
      if (v == nullptr) continue;
      // left
      if (x > 0) {
        auto u = U[width * y + (x - 1)];
        if (u != nullptr) v->neighbor.push_back(u);
      }
      // right
      if (x < width - 1) {
        auto u = U[width * y + (x + 1)];
        if (u != nullptr) v->neighbor.push_back(u);
      }
      // up
      if (y < height - 1) {
        auto u = U[width * (y + 1) + x];
        if (u != nullptr) v->neighbor.push_back(u);
      }
      // down
      if (y > 0) {
        auto u = U[width * (y - 1) + x];
        if (u != nullptr) v->neighbor.push_back(u);
      }
    }
  }
}

int Graph::size() const { return V.size(); }

bool is_same_config(const Config &C1, const Config &C2)
{
  const auto N = C1.size();
  for (size_t i = 0; i < N; ++i) {
    if (C1[i]->id != C2[i]->id) return false;
  }
  return true;
}

uint ConfigHasher::operator()(const Config &C) const
{
  uint hash = C.size();
  for (auto &v : C) {
    hash ^= v->id + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  }
  return hash;
}

std::ostream &operator<<(std::ostream &os, const Vertex *v)
{
  os << v->index;
  return os;
}

std::ostream &operator<<(std::ostream &os, const Config &Q)
{
  for (auto v : Q) os << v << ",";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Paths &paths)
{
  for (auto i = 0; i < paths.size(); ++i) {
    os << i << ":";
    for (auto &v : paths[i]) {
      os << std::setw(4) << v << "->";
    }
    std::cout << std::endl;
  }
  return os;
}
