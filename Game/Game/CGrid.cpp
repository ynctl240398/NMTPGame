//#include "CGrid.h"
//#include "CUtil.h"
//
//void CGrid::_ParseGridCells(string line) {
//	std::vector<std::string> tokens = split(line, "\t");
//
//	if (tokens.size() < 2) {
//		return;
//	}
//
//	_xCells = std::stoul(tokens.at(0));
//	_yCells = std::stoul(tokens.at(1));
//
//	_cells.resize(_xCells, std::vector<SCell>(_yCells));
//
//	const unsigned int MAX_ENTITIES_PER_CELL = 20;
//	for (unsigned int x = 0; x < _xCells; ++x) {
//		for (unsigned int y = 0; y < _yCells; ++y) {
//			_cells.at(x).at(y).indexX = x;
//			_cells.at(x).at(y).indexY = y;
//			_cells.at(x).at(y).entities.reserve(MAX_ENTITIES_PER_CELL);
//		}
//	}
//}
//
//void CGrid::_ParsePositions(string line, vector<CEntity*>& entities) {
//	std::vector<std::string> tokens = split(line, "\t");
//
//	if (tokens.size() < 3) {
//		return;
//	}
//
//	CGameObject::GameObjectType objectType = static_cast<CGameObject::GameObjectType>(std::stoul(tokens.at(0)));
//
//	unsigned int cellX = std::stoul(tokens.at(1));
//	unsigned int cellY = std::stoul(tokens.at(2));
//
//	for (auto& entity : entities) {
//		if (entity->ownerCell == nullptr && entity->GetObjectType() == objectType) {
//			SCell* newCell = GetCell(cellX, cellY);
//			AddEntity(entity, newCell);
//			return;
//		}
//	}
//}
//
//SCell* CGrid::GetCell(unsigned int x, unsigned int y) {
//	if (x < 0) {
//		x = 0;
//	}
//	else if (x >= _xCells) {
//		x = _xCells - 1;
//	}
//
//	if (y < 0) {
//		y = 0;
//	}
//	else if (y >= _yCells) {
//		y = _yCells - 1;
//	}
//
//	return &_cells.at(x).at(y);
//}
//
//SCell* CGrid::GetCell(D3DXVECTOR2 entityPosition) {
//	unsigned int x = static_cast<unsigned int>(entityPosition.x / SCell::CELL_WIDTH);
//	unsigned int y = static_cast<unsigned int>(entityPosition.y / SCell::CELL_HEIGHT);
//	return GetCell(x, y);
//}
//
//void CGrid::AddEntity(CEntity* entity) {
//	SCell* newCell = GetCell(entity->GetPosition());
//	newCell->entities.emplace_back(entity);
//	entity->ownerCell = newCell;
//}
//
//void CGrid::AddEntity(CEntity* entity, SCell* newCell) {
//	newCell->entities.emplace_back(entity);
//	entity->ownerCell = newCell;
//}
//
//void CGrid::RemoveEntity(CEntity* entity) {
//	std::vector<CEntity*>& entities = entity->ownerCell->entities;
//	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
//	entity->ownerCell = nullptr;
//}
//
//void CGrid::ParseData(std::string filePath, std::vector<CEntity*>& entities) {
//	std::ifstream readFile;
//	readFile.open(filePath, std::ios::in);
//
//	if (!readFile.is_open()) {
//		OutputDebugStringA("[GRID] Failed to read data\n");
//		return;
//	}
//
//	_GridFileSection gridFileSection = _GridFileSection::GRIDFILE_SECTION_UNKNOWN;
//
//	char str[MAX_SCENE_LINE];
//	while (readFile.getline(str, MAX_SCENE_LINE)) {
//		std::string line(str);
//
//		if (line.empty() || line.front() == '#') {
//			continue;
//		}
//
//		if (line == "[/]") {
//			gridFileSection = _GridFileSection::GRIDFILE_SECTION_UNKNOWN;
//			continue;
//		}
//
//		if (line == "[GRIDCELLS]") {
//			gridFileSection = _GridFileSection::GRIDFILE_SECTION_GRIDCELLS;
//			continue;
//		}
//
//		if (line == "[POSITIONS]") {
//			gridFileSection = _GridFileSection::GRIDFILE_SECTION_POSITIONS;
//			continue;
//		}
//
//		switch (gridFileSection) {
//		case _GridFileSection::GRIDFILE_SECTION_GRIDCELLS:
//			_ParseGridCells(line);
//			break;
//		case _GridFileSection::GRIDFILE_SECTION_POSITIONS:
//			_ParsePositions(line, entities);
//			break;
//		}
//	}
//
//	readFile.close();
//}
//
//void CGrid::Release() {
//	for (unsigned int i = 0; i < _xCells; ++i) {
//		for (unsigned int j = 0; j < _yCells; ++j) {
//			_cells.at(i).at(j).entities.clear();
//		}
//	}
//	_cells.clear();
//}