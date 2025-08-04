#pragma once

#include <utility>
#include <iostream>
#include <vector>

typedef std::pair<int, int> Dir;
typedef Dir Pos;
typedef std::pair<std::vector<char>, std::vector<char>> Vision;
typedef std::vector<float> State;

constexpr Dir UP    { -1,  0 };
constexpr Dir DOWN  {  1,  0 };
constexpr Dir RIGHT {  0,  1 };
constexpr Dir LEFT  {  0, -1 };

inline Dir inv(Dir dir)
{
	return {dir.first * -1, dir.second * -1};
}

constexpr Pos
operator+(const Pos& a, const Pos& b)
{
    return { a.first  + b.first,
             a.second + b.second };
}

inline std::ostream& operator<<(std::ostream& os, const Pos& pos)
{
    os << "{" << pos.first << ", " << pos.second << "}";
    return os;
}

#define WIDTH 1920
#define HWIDTH 960
#define HEIGHT 950
#define HHEIGHT 475
