#pragma once
#include "Mesh.h"
#include <mmsystem.h>

class MeshRendererTexturedModel : virtual public Mesh
{
	

	std::string modelpath;
public:
	MeshRendererTexturedModel();
	~MeshRendererTexturedModel();

	MeshRendererTexturedModel(std::string ModelPath);
	
};

