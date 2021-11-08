#pragma once
#include <fstream>
#include <iostream>
#include <vector>

// Class used to import and store the game map layout (Ring positions)
class Map {
    private:
        std::fstream mapFile;

        int row;
        int col;
        
        std::vector<std::vector<int>> matrix;
        std::vector<int> hightList;
        std::vector<int> rowList;
        std::vector<int> colList;

        int ringNum;
    public:
        Map();
        Map(std::string filename);

        void generateMap(std::string filename);

        std::vector<std::vector<int>> getMatrix();

        std::vector<int> getHightList();
        std::vector<int> getRowList();
        std::vector<int> getColList();

        int getRingNum();
        int getRow();
        int getCol();
};