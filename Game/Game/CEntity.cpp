//#include "CEntity.h"
//#include "CGrid.h"
//#include "CUtil.h"
//
//void CEntity::_ParseHitboxes(std::string line) {
//	std::vector<std::string> tokens = split(line, "\t");
//
//	if (tokens.size() < 4) {
//		return;
//	}
//
//	CRect* bdBox;
//	float left = std::stof(tokens.at(0));
//	float top = std::stof(tokens.at(1));
//	float right = std::stof(tokens.at(2));
//	float bottom = std::stof(tokens.at(3));
//	bdBox = new CRect(left, top, right, bottom);
//	_boundingBox.AddHitbox(bdBox);
//}
//
//bool CEntity::CompareRenderPriority(CEntity*& a, CEntity*& b) {
//	return a->_renderPriority > b->_renderPriority;
//}
//
//CEntity::CEntity() {
//	_health = 1;
//	//_renderPriority = std::numeric_limits<unsigned int>::max();
//	_renderPriority = 0;
//	_removeTime = 800;
//
//	ownerCell = nullptr;
//}
//
//CEntity::~CEntity() {}
//
//bool CEntity::IsRemoved() const {
//	return _removeStart != 0;
//}
//
//void CEntity::StartRemoveTimer() {
//	_removeStart = static_cast<DWORD>(GetTickCount64());
//}
//
//std::vector<std::string> CEntity::GetExtraData() const {
//	return _extraData;
//}
//
//void CEntity::SetGravity(float gravity) {
//	_gravity = gravity;
//}
//
//float CEntity::GetGravity() const {
//	return _gravity;
//}
//
//void CEntity::SetHealth(int health) {
//	_health = health;
//}
//
//int CEntity::GetHealth() const {
//	return _health;
//}
//
//void CEntity::ParseData(
//	std::string dataPath,
//	LPTEXTURE& texture,
//	std::vector<std::string> extraData)
//{
//	std::ifstream readFile;
//	readFile.open(dataPath, std::ios::in);
//
//	if (!readFile.is_open()) {
//		char debug[100];
//		sprintf_s(debug, "[ENTITY] Failed to read data for object with ID: %d\n", _objectType);
//		OutputDebugStringA(debug);
//		return;
//	}
//
//	if (!extraData.empty()) {
//		_extraData = extraData;
//	}
//
//	_DataFileSection dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
//	char str[MAX_SCENE_LINE];
//	while (readFile.getline(str, MAX_SCENE_LINE)) {
//		std::string line(str);
//
//		if (line.empty() || line.front() == '#') {
//			continue;
//		}
//
//		if (line == "[/]") {
//			dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
//			continue;
//		}
//
//		if (line == "[SPRITES]") {
//			dataFileSection = _DataFileSection::DATAFILE_SECTION_SPRITES;
//			continue;
//		}
//
//		if (line == "[HITBOXES]") {
//			dataFileSection = _DataFileSection::DATAFILE_SECTION_HITBOXES;
//			continue;
//		}
//
//		switch (dataFileSection) {
//		case _DataFileSection::DATAFILE_SECTION_SPRITES:
//			_ParseSprites(line);
//			break;
//		case _DataFileSection::DATAFILE_SECTION_HITBOXES:
//			_ParseHitboxes(line);
//			break;
//		}
//	}
//
//	readFile.close();
//}
//
//void CEntity::TakeDamage() {
//	if (_health > 0) {
//		--_health;
//		tookDamage = true;
//	}
//}
//
//void CEntity::Update(
//	DWORD deltaTime,
//	std::vector<CEntity*>* collidableEntities,
//	std::vector<CEntity*>* collidableTiles,
//	CGrid* grid)
//{
//	/*if (_position.y > SceneManager::GetInstance()->GetCurrentScene()->GetSceneHeight()) {
//		_health = 0;
//	}*/
//
//	if (IsRemoved() && GetTickCount64() - _removeStart > _removeTime) {
//		_health = -1;
//		_removeStart = 0;
//	}
//
//	if (!_isActive && _objectType >= GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
//		return;
//	}
//
//	CGameObject::Update(deltaTime);
//	_velocity.y += _gravity * _deltaTime;
//
//	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
//	if (_health > 0) {
//		//Edge case: what if the entity's bounding box is larger than the grid cell size? i.e: tile->_hitbox > grid->_cellsize
//		//Use another collection (collidableTiles) and calculate the collisions
//		//Since tiles themselves are static, as in, they don't update and render every tick, performance wise it's negligible
//		CalcPotentialCollision(collidableTiles, collisionEvents);
//
//		if (grid != nullptr) {
//			//Check collisions from the residing cell
//			CalcPotentialCollision(&ownerCell->entities, collisionEvents);
//
//			//Check collisions from neighboring cells
//			//Only need half of the neighboring cells to avoid double checking
//			//		cell
//			// cell	entity cell
//			//		cell
//			//Left
//			if (ownerCell->indexX > 0) {
//				CalcPotentialCollision(
//					&grid->GetCell(ownerCell->indexX - 1, ownerCell->indexY)->entities,
//					collisionEvents
//				);
//			}
//			//Right
//			if (ownerCell->indexX < grid->_xCells - 1) {
//				CalcPotentialCollision(
//					&grid->GetCell(ownerCell->indexX + 1, ownerCell->indexY)->entities,
//					collisionEvents
//				);
//			}
//			//Top
//			if (ownerCell->indexY > 0) {
//				CalcPotentialCollision(
//					&grid->GetCell(ownerCell->indexX, ownerCell->indexY - 1)->entities,
//					collisionEvents
//				);
//			}
//			//Bottom
//			if (ownerCell->indexY < grid->_yCells - 1) {
//				CalcPotentialCollision(
//					&grid->GetCell(ownerCell->indexX, ownerCell->indexY + 1)->entities,
//					collisionEvents
//				);
//			}
//		}
//		else {
//			CalcPotentialCollision(collidableEntities, collisionEvents);
//		}
//	}
//
//	if (collisionEvents.empty()) {
//		_position += _distance;
//	}
//	else {
//		D3DXVECTOR2 minTime;
//		D3DXVECTOR2 offset(0.4f, 0.4f);
//		D3DXVECTOR2 normal;
//		D3DXVECTOR2 relativeDistance;
//
//		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);
//
//		if (normal.x != 0.0f) {
//			_velocity.x = 0.0f;
//		}
//
//		if (normal.y != 0.0f) {
//			_velocity.y = 0.0f;
//		}
//
//		for (LPCOLLISIONEVENT result : eventResults) {
//			HandleCollisionResult(result, minTime, offset, normal, relativeDistance);
//		}
//
//		_position.x += _distance.x * minTime.x + normal.x * offset.x;
//		_position.y += _distance.y * minTime.y + normal.y * offset.y;
//	}
//
//	for (LPCOLLISIONEVENT event : collisionEvents) {
//		delete event;
//	}
//}
//
//LPCOLLISIONEVENT CEntity::SweptAABBEx(CEntity*& entity) {
//	CRect movingEntity;
//	CRect staticEntity;
//	D3DXVECTOR2 normal;
//	float time;
//
//	staticEntity = entity->GetBoundingBox();
//	D3DXVECTOR2 staticVeloctity = entity->GetVelocity();
//	D3DXVECTOR2 staticDistance = staticVeloctity * static_cast<float>(_deltaTime);
//	D3DXVECTOR2 relativeDistance = this->_distance - staticDistance;
//
//	movingEntity = this->GetBoundingBox();
//	SweptAABB(movingEntity, staticEntity, relativeDistance, normal, time);
//
//	if (entity->GetObjectType() == GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM) {
//		normal.x = 0.0f;
//	}
//
//	return new CCollisionEvent(entity, normal, relativeDistance, time);
//}
//
//void CEntity::CalcPotentialCollision(std::vector<CEntity*>* collidableEntities, std::vector<LPCOLLISIONEVENT>& collisionEvents) {
//	if (collidableEntities == nullptr) {
//		return;
//	}
//
//	for (unsigned int i = 0; i < collidableEntities->size(); ++i) {
//		LPCOLLISIONEVENT event = SweptAABBEx(collidableEntities->at(i));
//		if (event->time >= 0.0f && event->time <= 1.0f) {
//			collisionEvents.emplace_back(event);
//		}
//		else {
//			delete event;
//		}
//	}
//
//	std::sort(collisionEvents.begin(), collisionEvents.end(), CCollisionEvent::CompareCollisionEvent);
//}