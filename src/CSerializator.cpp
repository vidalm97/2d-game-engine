#include "CSerializator.h"

#include "CApplication.h"
#include "CGameObject.h"

#include <fstream>

CSerializator::CSerializator()
{
}

void CSerializator::SerializeTo( const std::string& aFilename, const std::vector<CGameObject>& aGameObjects ) const
{
	json finalJ;
	for( const auto& GO : aGameObjects )
	{
		json j;
		GO.Serialize(j);
		finalJ["Entities"].push_back(j);
	}

	std::ofstream file(aFilename);
	file << std::setw(4) << finalJ << std::endl;
}

void CSerializator::DeserializeFrom( const std::string& aFilePath, std::vector<CGameObject>& aGameObjects ) const
{
	std::ifstream i(aFilePath);
	json j;
	i >> j;

	for( json& GO : j["Entities"] )
	{
		aGameObjects.push_back( CGameObject( 0, "" ) );
		aGameObjects.back().Deserialize( GO );
	}
}
