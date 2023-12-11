/*
Copyright (c) 2009-2010, Mathias Gottschlag, Christian Reiser

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKPHYSICS_TRIMESH_HPP_
#define _PEAKPHYSICS_TRIMESH_HPP_

#include "Shape.hpp"

#include <string>

class btTriangleMesh;

namespace peak
{
	/**
	 * Holds vertex and index data needed to build trimesh shapes.
	 *
	 * Please mind that it's nessecary to destroy objects via destroy() after use.
	 */
	class TrimeshData
	{
		public:
			TrimeshData();
			~TrimeshData();

			/**
			 * Creates the trimesh data from a .obj file.
			 * @param name The mesh's filename
			 */
			bool init(std::string name);

			/**
			 * Creates the trimesh data from user-defined data
			 * @param vertexcount The amount of vertices holded by the mesh
			 * @param indexcount The amount of indices holded by the mesh
			 */
			bool init(Vector3F *vertices, int *indices, int vertexcount, int indexcount);
			/**
			 * Manual object destruction is nessecary to secure multi-use of the same data
			*/
			bool destroy();

			Vector3F *getVertices() const;
			int *getIndices() const;
			int getVertexCount() const;
			int getIndexCount() const;

		private:
			Vector3F *vertices;
			int *indices;
			int vertexcount, indexcount;
	};

	/**
	 * Triangle mesh collision shape.
	 */
	class Trimesh : public Shape
	{
		public:
			Trimesh();
			~Trimesh();

			/**
			 * Creates the trimesh
			 * @param data The vertex and index data of the trimesh
			 * @param mass Mass of the trimesh (0 = static)
			 * @param buildhull Should be true if input mesh is concave and should result in a non-static shape
			 */
			bool init(TrimeshData data, float mass = 0.0f, bool buildhull = true);
			virtual bool destroy();

		private:
			btTriangleMesh *trimesh;

	};
}

#endif
