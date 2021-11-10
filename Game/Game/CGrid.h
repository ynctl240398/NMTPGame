//#pragma once
//
//#include "CEntity.h"
//#include "CUtil.h"
//
//struct SCell {
//	const static unsigned int CELL_WIDTH = static_cast<unsigned int>(SCREEN_WIDTH / 2);
//	const static unsigned int CELL_HEIGHT = static_cast<unsigned int>(SCREEN_HEIGHT / 2);
//
//	unsigned int indexX, indexY;
//	vector<CEntity*> entities;
//};
//
//class CGrid
//{
//private:
//	friend class CEntity;
//
//	enum class _GridFileSection {
//		GRIDFILE_SECTION_UNKNOWN,
//		GRIDFILE_SECTION_GRIDCELLS,
//		GRIDFILE_SECTION_POSITIONS
//	};
//
//	unsigned int _xCells, _yCells;
//
//	vector<vector<SCell>> _cells;
//
//	void _ParseGridCells(std::string);
//	void _ParsePositions(std::string, std::vector<CEntity*>&);
//
//public:
//	SCell* GetCell(unsigned int, unsigned int);
//	SCell* GetCell(D3DXVECTOR2);
//
//	void AddEntity(CEntity*);
//	void AddEntity(CEntity*, SCell*);
//
//	void RemoveEntity(CEntity*);
//
//	void ParseData(std::string, std::vector<CEntity*>&);
//
//	void Release();
//};
//
