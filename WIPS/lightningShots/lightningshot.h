using System;
using System.Collections.Generic;

using IrrlichtLime.Scene;
using IrrlichtLime.Video;
using IrrlichtLime.Core;

namespace L18.LightningShots
{
	class LightningShot
	{
		SceneManager sceneManager;
		TriangleSelector worldTriangles;
		float worldInfinity;
		float shotSpeed;
		float shotRadius;

		IndexBuffer indexBuffer;
		VertexBuffer vertexBuffer;

		public int TotalShots { get { return shots.Count; } }
		public int TotalLightnings { get { return indexBuffer.Count / (2 * 10); } } // number of points for each line is 2, number of lines for each lightning is 10

		public LightningShot(SceneManager sceneManager, TriangleSelector worldTriangles, float worldInfinity = 5000, float shotSpeed = 0.1f, float shotRadius = 100)
		{
			this.sceneManager = sceneManager;
			this.worldTriangles = worldTriangles;
			this.worldInfinity = worldInfinity;
			this.shotSpeed = shotSpeed;
			this.shotRadius = shotRadius;

			indexBuffer = IndexBuffer.Create(IndexType._16Bit);
			indexBuffer.Reallocate(64000);
			for (int i = 0; i < indexBuffer.AllocatedCount - 1; i++)
				indexBuffer.Add(i);

			vertexBuffer = VertexBuffer.Create();
		}

		public void Fire(Vector3Df position, Vector3Df direction, uint time)
		{
			Shot s = new Shot();
			s.direction = direction;

			Vector3Df e = position + s.direction * worldInfinity;
			Line3Df l = new Line3Df(position, e);

			Vector3Df cv;
			Triangle3Df ct;
			SceneNode cn;
			if (sceneManager.SceneCollisionManager.GetCollisionPoint(l, worldTriangles, out cv, out ct, out cn))
				e = cv;

			s.deathTime = time + (uint)((e - position).Length / shotSpeed);

			s.node = sceneManager.AddSphereSceneNode(10);
			s.node.SetMaterialFlag(MaterialFlag.Lighting, false);
			sceneManager.MeshManipulator.SetVertexColors(((MeshSceneNode)s.node).Mesh, Color.SolidWhite);

			var animator = sceneManager.CreateFlyStraightAnimator(position, e, (s.deathTime - time) / 1000.0f);
			s.node.AddAnimator(animator);
			animator.Drop();

			sceneManager.AddLightSceneNode(s.node);

			shots.Add(s);
		}

		void removeDead(uint time)
		{
			List<int> deleteCandidates = new List<int>();

			for (int i = 0; i < shots.Count; i++)
				if (time >= shots[i].deathTime)
					deleteCandidates.Add(i);

			for (int i = deleteCandidates.Count - 1; i >= 0; i--)
			{
				int j = deleteCandidates[i];
				shots[j].node.Remove();
				shots.RemoveAt(j);
			}
		}

		public void Draw(uint time)
		{
			removeDead(time);

			Material mat = new Material();
			mat.Type = MaterialType.TransparentVertexAlpha;
			mat.Lighting = false;
			sceneManager.VideoDriver.SetMaterial(mat);
			sceneManager.VideoDriver.SetTransform(TransformationState.World, Matrix.Identity);

			Random random = new Random();

			Color[] colors = new Color[11]; // 11 colors (line count + 1, since each line has 2 points)
			for (int i = 0; i < colors.Length; i++)
				colors[i] = new Color(255, 255, 255, 255 - (10 - i) * 25);

			vertexBuffer.SetCount(0);

			foreach (var shot in shots)
			{
				Vector3Df pos = shot.node.Position;
				AABBox box = new AABBox(pos - new Vector3Df(shotRadius), pos + new Vector3Df(shotRadius));

				List<Triangle3Df> tris = worldTriangles.GetTriangles(box, 1000);
				if (tris.Count == 0)
					continue;

				Dictionary<float, Vector3Df> uniquePoints = new Dictionary<float, Vector3Df>();
				foreach (var t in tris)
				{
					Vector3Df p = t.GetClosestPointOnTriangle(pos);
					float k = p.X + p.Y * 1000 + p.Z * 1000000;
					uniquePoints[k] = p;
				}

				foreach (var point in uniquePoints.Values)
				{
					BillboardSceneNode n = sceneManager.AddBillboardSceneNode(null, new Dimension2Df(7.5f));
					n.SetMaterialFlag(MaterialFlag.Lighting, false);
					n.SetMaterialType(MaterialType.TransparentAddColor);
					n.SetMaterialTexture(0, sceneManager.VideoDriver.GetTexture("../../media/particlewhite.bmp"));
					n.Position = point;

					var animator = sceneManager.CreateDeleteAnimator(0.1f);
					n.AddAnimator(animator);
					animator.Drop();

					Vertex3D v1 = new Vertex3D(point);
					Vertex3D v2 = new Vertex3D();

					for (int i = 0; i < 10; i++)
					{
						v1.Color = colors[i];
						v2.Color = colors[i + 1];

						v2.Position = pos.GetInterpolated(point, i * 0.111);

						if (i != 0 && i != 9) // do not displace first and last points
							v2.Position += new Vector3Df((random.Next() % 10) - 5, (random.Next() % 10) - 5, (random.Next() % 10) - 5);

						vertexBuffer.Add(v1);
						vertexBuffer.Add(v2);

						v1.Position = v2.Position;
					}
				}
			}

			indexBuffer.SetCount(vertexBuffer.Count);
			sceneManager.VideoDriver.DrawVertexPrimitiveList(vertexBuffer, indexBuffer, PrimitiveType.Lines);
		}

		public void Drop()
		{
			indexBuffer.Drop();
			vertexBuffer.Drop();

			shots.ForEach(s => s.node.Remove());
			shots.Clear();
		}

		List<Shot> shots = new List<Shot>();

		class Shot
		{
			public SceneNode node;
			public Vector3Df direction;
			public uint deathTime;
		}
	}
}
