// ============================================================================
//			Copyright (c) 2014 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _MESH_TOOLS_INTERFACE_H_
#define _MESH_TOOLS_INTERFACE_H_

#include "MeshInterface.h"

// An interface to use various functionalities in Mesh Tools Library
class MeshToolsInterface
{
public:
	MeshToolsInterface();
	virtual ~MeshToolsInterface();

	// =======================================================================================================

	// Mesh creation and deletion

	// Create empty Mesh
	virtual MeshInterface* CreateMesh() = 0;

	// Delete existing mesh
	virtual void DeleteMesh(MeshInterface* &mesh) = 0;

	// =======================================================================================================

	// STL - Read and Write

	// To read mesh from STL file
	virtual int ReadSTLToGetMesh(const wchar_t* fileName, MeshInterface* &mesh, double multFactor = 1.0)const = 0;

	//To read triangles from STL file
	virtual int ReadSTLToGetTriangles(const wchar_t* fileName, double* &triangles, size_t& numTriangles, double* &normals, double multFactor = 1.0, bool applyNormal = false)const = 0;

	// To write mesh in STL file
	virtual int WriteMeshInSTL(const MeshInterface* mesh, const wchar_t* fileName, bool writeAscii = false)const = 0;

	// To read mesh from MSH file
	virtual int ReadMSHToGetMesh(const wchar_t* fileName, MeshInterface* &mesh, double multFactor = 1.0)const = 0;

	// To read mesh topology from MSH file
	virtual int ReadMSHToCreateMeshArrays(const wchar_t* fileName,
										  double* &vertices, size_t &numVertices,
										  int* &facetVertexIndices, size_t &numFacets)const = 0;

	// =======================================================================================================

	//Mesh Creation

	//This method adds facets given along with topology. Thus, if connectivity/topology is already known
	//then this method should be used as it is faster than AddFacet() which adds single facet at a time
	// If numVertices = 100, size of vertices array would be 300
	// If numFacets = 100, size of facetVertexIndices array would be 300
	//The two arrays - vertices and facetVertexIndices, are only used not owned by the library
	virtual int AddFacetsInMesh(MeshInterface* mesh, double* vertices, size_t numVertices, size_t* facetVertexIndices, size_t numFacets)const = 0;

	//This method prepares for adding single facet at a time in mesh
	//This must be called before adding a single facet using AddFacetInMesh()
	//This takes as input following information-
	//		a. Valid MeshInterface pointer in which facet is to be added
	//		b. Bounding extents of all facets to be added
	//		b. Number of facets to be added
	virtual int StartFacetInputForMesh(MeshInterface *mesh,
										double facetsMinX, double facetsMaxX,
										double facetsMinY, double facetsMaxY,
										double facetsMinZ, double facetsMaxZ,
										size_t numFacets)const = 0;

	//This method stops adding single facet at a time in mesh
	//This must be called after all facets are singly added in mesh using AddFacetInMesh()
	//This takes valid MeshInterface pointer as input
	virtual int EndFacetInputForMesh(MeshInterface *mesh)const = 0;

	//This method adds a single facet in mesh. This method works only if StartFacetInputForMesh() is called before.
	//Though only geometry of facet is taken as input, this method also makes sure that duplicate vertices
	//are not getting formed, thus creating a topology internally
	virtual int AddFacetInMesh(MeshInterface* mesh,
							   double ax, double ay, double az,			/* Vertex 1 */
							   double bx, double by, double bz,			/* Vertex 2 */
							   double cx, double cy, double cz,			/* Vertex 3 */
							   double nx, double ny, double nz)const = 0;	/* Normal   */

	// =======================================================================================================

	// Mesh Boolean

	// To unite two meshes
	virtual int UniteMeshes(MeshInterface* mesh1, MeshInterface* mesh2, MeshInterface* &outputMesh) = 0;

	// To subtract one mesh from other. Second mesh would be subtracted from first mesh 
	virtual int SubtractMeshes(MeshInterface* mesh1, MeshInterface* mesh2, MeshInterface* &outputMesh) = 0;

	// To intersect two meshes
	virtual int IntersectMeshes(MeshInterface* mesh1, MeshInterface* mesh2, MeshInterface* &outputMesh) = 0;

	// To Cut Mesh by another mesh. First mesh would be cut by second mesh
	virtual int CutMesh(MeshInterface* meshToCut, MeshInterface* cutterMesh, MeshInterface* &outputPart1, MeshInterface* &outputPart2) = 0;

	// =======================================================================================================

	// To get last error message
	virtual int GetLastError()const = 0;

	// To get problem triangles. This would return problem triangles only if PROBLEM_TRIANGLES_FOUND_IN_INPUT error occurs
	virtual int GetProblemTriangles(double* &triPointCoordinates, size_t &numTriangles) = 0;

	// To delete problem triangles, if any, found during last Boolean operation
	virtual void DeleteProblemTriangles() = 0;

	// =======================================================================================================
	
};

// To create a static instance of Mesh Tools Library
MeshToolsInterface* GetMeshToolsInstance();

// To delete the static instance of Mesh Tools Library
void DeleteMeshToolsInstance();

#endif
