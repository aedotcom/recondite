#ifndef R_MATERIAL_DATA_HPP
#define R_MATERIAL_DATA_HPP

#include <map>
#include <fstream>

#include "rBuild.hpp"
#include "rTypes.hpp"
#include "rString.hpp"

#include "xml/rXMLDocument.hpp"
#include "stream/rIFileStream.hpp"

#include "rPropertyCollection.hpp"

struct RECONDITE_API rMaterialParameterData{
	rMaterialParameterData(){}
	rMaterialParameterData(rPropertyType t, const rString& n, const rString& v, const rString& p);
	
	rMaterialParameterData(const rMaterialParameterData& data);
	void Set(rPropertyType t, const rString& n, const rString& v, const rString& p);

	rPropertyType type;
	rString name;
	rString value;
	rString path;
};

typedef std::map<rString, rMaterialParameterData> rMaterialParameterDataMap;
typedef rMaterialParameterDataMap::const_iterator rMaterialParameterDataConstItr;

class RECONDITE_API rMaterialData{
public:
	rMaterialData();
	rMaterialData(const rString& path);
	rMaterialData(rIStream& stream);
	rMaterialData(const rString& shaderName, const rString& shaderPath, const rMaterialParameterDataMap& parameters);
	~rMaterialData();
	
	rContentError LoadFromPath(const rString& path);
	rContentError LoadFromStream(rIStream& stream);
	
	rContentError WriteToPath(const rString& path);
	rContentError WriteToStream(std::ostream& stream);
	
	void SetMaterialData(const rString& shaderName, const rString& shaderPath, const rMaterialParameterDataMap& parameters);
	
	void Clear();
	
public:	
	rString GetPath() const;
	void SetPath(const rString& path);

	void SetShader(const rString& shaderName, const rString& shaderPath);
	void SetParameter(rPropertyType type, const rString& name, const rString& value);
	void SetParameter(rPropertyType type, const rString& name, const rString& value, const rString& path);
	
	rString GetShaderName() const;
	rString GetShaderPath() const;
	
	size_t GetParameterCount() const;
	bool GetParameterData(const rString& name, rMaterialParameterData& data) const;
	void GetParameterNames(rArrayString& names) const;
	void GetParameterNamesForType(rArrayString& names, rPropertyType type) const;
	
	rContentError GetError() const;
	
private :

	rContentError LoadShaderData(const rXMLDocument& xml);
	rContentError LoadParameters(const rXMLDocument& xml);
	
	static rPropertyType GetParameterType(const rString& type);
	static rString GetParamterTypeName(rPropertyType type);
	
private:
	rString m_shaderName;
	rString m_shaderPath;
	
	rMaterialParameterDataMap m_parameters;
	
	rString m_path;
	rContentError m_error;
};

#endif
