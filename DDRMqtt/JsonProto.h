#ifndef JsonProto_h__
#define JsonProto_h__

#include <json/json.h>
#include <tuple>
#include <map>
#include <iostream>



class IJsonSerializable
{
public:
	virtual ~IJsonSerializable(void) {};
	virtual void Serialize(Json::Value& root) = 0;
	virtual void Deserialize(Json::Value& root) = 0;
};

class CJsonSerializer
{
public:
	static bool Serialize(IJsonSerializable* pObj, std::string& output);
	static bool Deserialize(IJsonSerializable* pObj, std::string& input);

private:
	CJsonSerializer(void) {};
};


class DDRStatus : public IJsonSerializable
{
public:
	virtual void Serialize(Json::Value& root);
	virtual void Deserialize(Json::Value& root);
public:
	std::string Name;
	float LineSpeed;
	float AngSpeed;
	std::vector<int> IntArray;

};

#endif // JsonProto_h__
