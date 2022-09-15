#ifndef CSERIALIZATOR__H
#define CSERIALIZATOR__H

#include "json.hpp"

class CGameObject;

class CSerializator
{
public:
	using json = nlohmann::json;

	CSerializator();
	~CSerializator() = default;

	void SerializeTo( const std::string& aFilePath, const std::vector<CGameObject>& aGameObjects ) const;
	void DeserializeFrom( const std::string& aFilePath, std::vector<CGameObject>& aGameObjects ) const;
};

#endif
