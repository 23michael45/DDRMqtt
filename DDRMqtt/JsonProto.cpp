#include "JsonProto.h"


bool CJsonSerializer::Serialize(IJsonSerializable* pObj, std::string& output)
{
	if (pObj == NULL)
		return false;

	Json::Value serializeRoot;
	pObj->Serialize(serializeRoot);

	Json::StyledWriter writer;
	output = writer.write(serializeRoot);

	return true;
}

bool CJsonSerializer::Deserialize(IJsonSerializable* pObj, std::string& input)
{
	if (pObj == NULL)
		return false;

	Json::Value deserializeRoot;
	Json::Reader reader;

	if (!reader.parse(input, deserializeRoot))
		return false;

	pObj->Deserialize(deserializeRoot);

	return true;
}

void DDRStatus::Serialize(Json::Value& root)
{
	root["Name"] = Name;
	root["LineSpeed"] = LineSpeed;
	root["AngSpeed"] = AngSpeed;


	Json::Value arrayObj;
	for (int i : IntArray)
	{
		Json::Value item;
		item = i;

		arrayObj.append(item);
	}

	root["IntArray"] = arrayObj;
}

void DDRStatus::Deserialize(Json::Value& root)
{
	Name = root["Name"].asString();
	LineSpeed = root["LineSpeed"].asFloat();
	AngSpeed = root["AngSpeed"].asFloat();

	auto arrayObj = root["IntArray"];

	IntArray.clear();
	for (int i = 0; i < arrayObj.size(); i++)
	{
		auto item = arrayObj[i];
		int ivalue = item.asInt();

		IntArray.push_back(ivalue);
	}

}