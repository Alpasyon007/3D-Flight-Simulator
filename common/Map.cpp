#include <Map.h>

Map::Map() {
	std::cout << "Decleration of Map" << std::endl;
}

void Map::generateMap(std::string filename) {
    mapFile.open(filename);
    if(!mapFile) {
	    std::cerr << "Map not found" << std::endl;
	}  else {
		ringNum = 0; 
        col = 0; 
        row = 0;
		std::string temp;
		while(getline(mapFile,temp)) { 
			++row;
		};

		int i;
		for(i = 0; temp[i] != '\0'; i++) {
			if(temp[i] == ' ' && temp[i-1] != ' ') {
				col++;
			}
		}
		if(temp[i-1] != ' ') ++col;


        matrix.resize(row);
        for(int i = 0; i < row; i++) {
            matrix[i].resize(col);
        }

		mapFile.clear();
		mapFile.seekg(0);

		for(int i = 0; i < row; i++) {
			for(int j = 0; j < col; j++) {
				mapFile >> matrix[i][j];
				if(matrix[i][j] > 0) {
					ringNum++;

					hightList.push_back(matrix[i][j]);
                    rowList.push_back(i);
                    colList.push_back(j);
				} 
			}
		}

		for(int i = 0; i < row; i++) {
			for(int j = 0; j < col; j++) {
				std::cout << matrix[i][j] << " ";
			}
		}
    }	
	mapFile.close();
}

std::vector<std::vector<int>> Map::getMatrix() {
    return matrix;
}

std::vector<int> Map::getHightList() {
	return hightList;
};

std::vector<int> Map::getRowList() {
	return rowList;
};

std::vector<int> Map::getColList() {
	return colList;
};

int Map::getRingNum() {
    return ringNum;
}

int Map::getRow() {
    return row;
}

int Map::getCol() {
    return col;
}