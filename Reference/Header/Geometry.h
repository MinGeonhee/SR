#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

template<typename T>
class ENGINE_DLL Geometry
{
private:
	vector<T>			_vertices;
	vector<_uint32>		_indices;
public:
	Geometry() {}
	~Geometry() {}

	_uint32				GetVertexCount() { return static_cast<_uint32>(_vertices.size()); }
	void*				GetVertexData() { return _vertices.data(); }
	const vector<T>&	GetVertices() { return _vertices; }

	_uint32				GetIndexCount() { return static_cast<_uint32>(_indices.size()); }
	void*				GetIndexData() { return _indices.data(); }
	const vector<_uint32>& GetIndices() { return _indices; }

	void AddVertex(const T& vertex) { _vertices.push_back(vertex); }
	void AddVertices(const vector<T>& vertices) { _vertices.insert(_vertices.end(), vertices.begin(), vertices.end()); }
	void SetVertices(const vector<T>& vertices) { _vertices = vertices; }

	void AddIndex(_uint32 index) { _indices.push_back(index); }
	void AddIndices(const vector<_uint32>& indices) { _indices.insert(_indices.end(), indices.begin(), indices.end()); }
	void SetIndices(const vector<_uint32>& indices) { _indices = indices; }
};

END
