#pragma once
#include "ViewObject.h"
#include "FitTexturePlane.h"
#include "TexturedPlane.h"
#include "TransparentPlane.h"
#include "Water.h"
#include "SkyBox.h"

template <typename T>
class ViewGroup : public ViewBase
{
public:
	void AddObject(const std::shared_ptr<T>& object)
	{
		this->_objects.push_back(object);
	}

	std::shared_ptr<T> GetObject(const HandleT& objectID)
	{
		for (const auto& object : this->_objects)
		{
			if (object->GetHandle() == objectID)
				return object;
		}
		throw(std::string("No Such Object has ID = ") + std::to_string(objectID));
	}

	void RemoveObject(const HandleT& objectID)
	{
		
		for (auto it = this->_objects.begin(); it != this->_objects.end(); it++)
		{
			if ((*it)->GetHandle() == objectID)
			{
				this->_objects.erase(it);
				return;
			}
		}
		throw(std::string("No Such Object has ID = ") + std::to_string(objectID));
	}

	std::vector<std::shared_ptr<T>> GetObjectList()
	{
		return this->_objects;
	}

protected:
	std::vector<std::shared_ptr<T>> _objects;
};

