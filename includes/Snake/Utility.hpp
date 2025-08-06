#pragma once

#include <utility>
#include <iostream>
#include <vector>

typedef std::pair<int, int> Dir;
typedef Dir Pos;
typedef std::pair<std::vector<char>, std::vector<char>> Vision;

constexpr Dir UP    { -1,  0 };
constexpr Dir DOWN  {  1,  0 };
constexpr Dir RIGHT {  0,  1 };
constexpr Dir LEFT  {  0, -1 };

inline Dir inv(Dir dir)
{
	return {dir.first * -1, dir.second * -1};
}

inline int dir_to_idx(const Dir& d)
{
	if (d == UP) return 0;
	if (d == DOWN) return 1;
	if (d == LEFT) return 2;
	else return 3;
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

typedef enum {
	NOTHING = 0,
	EAT_RED = -1,
	DIED = -5,
	EAT_GREEN = +5,
}	Reward;
