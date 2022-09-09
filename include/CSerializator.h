#ifndef CSERIALIZATOR__H
#define CSERIALIZATOR__H

#include "json.hpp"

class CGameObject;

class CSerializator
{
public:
	using json = nlohmann::json;

	void SerializeTo( const std::string& aFilePath, const std::vector<CGameObject>& aGameObjects ) const;
	void DeserializeFrom( const std::string& aFilePath, std::vector<CGameObject>& aGameObjects ) const;

	CSerializator();
	~CSerializator() = default;
};

#endif
