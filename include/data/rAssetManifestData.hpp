#ifndef R_ASSET_MANIFEST_DATA_HPP
#define R_ASSET_MANIFEST_DATA_HPP

#include <vector>

#include "rBuild.hpp"
#include "rDefs.hpp"
#include "rString.hpp"

struct RECONDITE_API rAssetManifestEntry {
	rAssetType type;
	rString name;
	rString path;
	
	rAssetManifestEntry(rAssetType t, const rString& n, const rString& p) : type(t), name(n), path(p) {}
};

typedef std::vector<rAssetManifestEntry*> rAssetManifestList;

class RECONDITE_API rAssetManifestData {
public:
	~rAssetManifestData();

	void AddManifestEntry(rAssetType type, const rString& name, const rString& path);
	rAssetManifestEntry* GetManifestEntry(size_t index) const;

	rString GetPath() const;
	void SetPath(const rString& path);

	size_t AssetCount() const;
	
	void Clear();
	
private:
	
	rString m_path;
	rAssetManifestList m_assets;
};

#endif
