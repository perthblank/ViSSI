#ifndef TYPE_MANAGER_H
#define TYPE_MANAGER_H

#include <vector>
#include <map>
#include <fstream>
#include <json\json.h>
#include <string>
#include <iostream>
using namespace std;

#define CONTINUOUS 1
#define DISCRETE 2
#define DEFAULT_MAIN_WINDOW_CONFIG_FILE "..\\src\\config\\typeConfig.json"

class ModelOption;
class ConfigTable;
 
typedef map<int, string> TypeMap;
typedef map<string, ModelOption> ModelMap;

typedef vector<string> LList;

class ModelOption
{
public:
	int methods = 0;
	int visuals = 0;
	int type = -1;

	void addMethod(int mt)
	{
		methods |= mt;
	}

	void addVisual(int vt)
	{
		visuals |= vt;
	}

	ModelOption(int t) :type(t)
	{}

	ModelOption()
	{}

};


class ConfigTable
{
public:
	ConfigTable(const char * filename)
	{
		parseTypeConfig(filename);
	}

	ConfigTable():ConfigTable(DEFAULT_MAIN_WINDOW_CONFIG_FILE)
	{}

	void setModelType(string mt)
	{}

	void setMethodType(string mt)
	{}

	void setVisualType(string mt)
	{}

	LList getModels(int type)
	{
		LList list;
		for (ModelMap::iterator iter = model_map.begin(); iter != model_map.end(); ++iter)
		{
			if(iter->second.type == type)
				list.push_back(iter->first);
		}
		return list;
	}

	LList getMethods(string mt)
	{
		LList list;

		ModelOption option = model_map[mt];
		int methods = option.methods;

		for (TypeMap::iterator iter = method_map.begin(); iter != method_map.end(); ++iter)
		{
			int v = iter->first;
			if (methods & v)
			{
				list.push_back(iter->second);
			}
		}
		return list;
	}

	LList getVisuals(string mt)
	{
		LList list;

		ModelOption option = model_map[mt];
		int visuals = option.visuals;

		for (TypeMap::iterator iter = visual_map.begin(); iter != visual_map.end(); ++iter)
		{
			int v = iter->first;
			if (visuals & v)
			{
				list.push_back(iter->second);
			}
		}
		return list;
	}
	

	void setMap(Json::Value& root, TypeMap& map)
	{
		int size = root.size();
		for (int i = 0; i < size; ++i)
		{
			map[root[i]["v"].asInt()] = root[i]["k"].asString();
		}
	}

	ModelOption modelOption(const Json::Value & current)
	{
		ModelOption mo(current["cod"].asInt());
		int mSize = current["method"].size();

		for(int i = 0; i<mSize; ++i)
		{
			mo.addMethod(current["method"][i].asInt());
		}

		int vSize = current["visual"].size();
		for(int i = 0; i<vSize; ++i)
		{
			mo.addVisual(current["visual"][i].asInt());
		}

		return mo;
	}

	int parseTypeConfig(const char* filename)
	{
		Json::Reader reader;
		Json::Value root;

		std::ifstream is;
		is.open(filename, std::ios::binary);

		if (reader.parse(is, root))
		{

			// construct type-number map
			setMap(root["map"]["method"], method_map);
			setMap(root["map"]["visual"], visual_map);
			setMap(root["map"]["cod"], cod_map);

			// construct models
			int cSize = root["content"].size();
			for (int i = 0; i < cSize; ++i)
			{
				Json::Value & current = root["content"][i];
				string model = current["model"].asString();
				model_map[model] = modelOption(current);
			}
			
		}
		else
		{
			cout << "cannot parse file "<< filename << endl;
		}
		is.close();

		return 0;
	}

	bool isValidConfig(string model, int mode, int method)
	{
		return model_map[model].methods&method && model_map[model].type==mode;
	}


	TypeMap method_map, visual_map, cod_map;

	ModelMap model_map;

};




#endif
