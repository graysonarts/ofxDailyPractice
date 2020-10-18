#pragma once

#include "ofMain.h"
#include "Cell.h"
#include <math.h>

class Grid {
public:
	Grid(int width, int height) : m_width(width), m_height(height), m_grid(nullptr) {
		m_grid = new int* [m_height];
		for (int i = 0; i < m_height; i++) {
			m_grid[i] = new int[m_width];
			for (int j = 0; j < m_width; j++) {
				m_grid[i][j] = -1;
			}
		}
	}

	~Grid() {
		if (m_grid != nullptr) {
			for (int i = 0; i < m_height; i++) {
				if (m_grid[i] != nullptr) {
					delete m_grid[i];
				}
			}
		}

		delete m_grid;
	}

	Cell* at(int x, int y) {
		int index = m_grid[y][x];
		if (index < m_cells.size())
			return &m_cells.at(index);

		return nullptr;
	}

	void for_each(std::function<void(int, int, int)>&& callback) {
		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {
				callback(x, y, m_grid[y][x]);
			}
		}

	}

	Edges get_neighbor_constraints(int x, int y) {
		Edges results = (Edges)0;

	}

private:
	int m_width, m_height;
	int** m_grid;
	std::vector<Cell> m_cells;
};