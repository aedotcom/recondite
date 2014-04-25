#ifndef R_ASSETMANIFESTDATAFILE_HPP
#define R_ASSETMANIFESTDATAFILE_HPP

#include <fstream>

#include "rBuild.hpp"
#include "rString.hpp"

#include "xml/rXMLDocument.hpp"
#include "xml/rXMLElement.hpp"

#include "data/rAssetManifestData.hpp"

class RECONDITE_API rAssetManifestDataReader{
public:

	rContentError ReadFromFile(const rString& path, rAssetManifestData& assetManifest);
	rContentError ReadFromStream(std::istream& stream, rAssetManifestData& assetManifest);

	rContentError ReadAssets(std::istream& stream, rAssetManifestData& assetManifest);
private:

};

class RECONDITE_API rAssetManifestDataWriter{
public:
	rContentError WriteToFile(const rString& path, const rAssetManifestData& assetManifest);
	rContentError WriteToStream(std::ostream& stream, const rAssetManifestData& assetManifest);

private:
	rContentError WriteAssets(std::ostream& stream, const rAssetManifestData& assetManifest);
};

#endif