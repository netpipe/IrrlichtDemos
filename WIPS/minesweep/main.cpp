using System;

using IrrlichtLime;
using IrrlichtLime.Core;
using IrrlichtLime.Video;
using IrrlichtLime.Scene;

namespace L17.Minesweeper
{
	class Game
	{
		const float SIZE_OF_MESH = 10.0f;
		const float SIZE_OF_CELL = SIZE_OF_MESH * 1.15f;

		public enum State
		{
			Playing,
			Lost,
			Won
		}

		class Cell
		{
			public int i = 0, j = 0;
			public int number = 0; // -1=bomb, 0..8=number of bombs around
			public bool flagged = false;
			public bool revealed = false;
			public bool IsBomb { get { return number == -1; } }
		}

		Cell[] m_board;
		int m_boardDimWidth, m_boardDimHeight;
		State m_state;

		IrrlichtDevice m_device;
		SceneNode m_root;
		Mesh m_meshCell, m_meshFlag, m_meshBomb;

		public Vector3Df CenterOfTheBoard { get { return new Vector3Df(m_boardDimWidth * SIZE_OF_CELL / 2, 0, m_boardDimHeight * SIZE_OF_CELL / 2); } }

		public State StateOfTheGame { get { return m_state; } }

		public Game(IrrlichtDevice device)
		{
			m_device = device;
			m_root = m_device.SceneManager.AddEmptySceneNode();

			loadCellMesh();
			loadFlagMesh();
			loadBombMesh();

			NewGame(10, 10);
		}

		public void NewGame(int boardDimWidth, int boardDimHeight)
		{
			m_boardDimWidth = boardDimWidth;
			m_boardDimHeight = boardDimHeight;
			m_state = State.Playing;
			m_board = new Cell[m_boardDimWidth * m_boardDimHeight];

			m_root.RemoveChildren();

			// init board

			Random r = new Random();
			for (int j = 0; j < m_boardDimHeight; j++)
			{
				for (int i = 0; i < m_boardDimWidth; i++)
				{
					Cell c = new Cell();
					c.i = i;
					c.j = j;
					c.number = (r.Next() % 6) == 0 ? -1 : 0;
					c.flagged = false;
					c.revealed = false;

					m_board[i + j * m_boardDimWidth] = c;

					MeshSceneNode n = m_device.SceneManager.AddMeshSceneNode(m_meshCell, m_root, 0x10000 + i + j * m_boardDimWidth);
					n.Position = new Vector3Df(i * SIZE_OF_CELL, 0, j * SIZE_OF_CELL);

					// n must have at least 2 children, where #0 is a flag node and #1 is a bomb node
					// (that's why we add shadow volume nodes at the very end)

					MeshSceneNode f = m_device.SceneManager.AddMeshSceneNode(m_meshFlag, n, 0);
					f.Position = new Vector3Df(
						(n.BoundingBox.MaxEdge.X - m_meshFlag.BoundingBox.MaxEdge.X) / 2,
						n.BoundingBox.MaxEdge.Y,
						(n.BoundingBox.MaxEdge.Z - m_meshFlag.BoundingBox.MaxEdge.Z) / 2 + SIZE_OF_CELL / 6);
					f.Visible = false;

					MeshSceneNode b = m_device.SceneManager.AddMeshSceneNode(m_meshBomb, n, 0);
					b.Position = new Vector3Df(
						0,
						n.BoundingBox.MaxEdge.Y,
						0);
					b.Visible = false;

					n.AddShadowVolumeSceneNode(null, 0);
					f.AddShadowVolumeSceneNode(null, 0);
					b.AddShadowVolumeSceneNode(null, 0);
				}
			}

			// calc board numbers

			for (int j = 0; j < m_boardDimHeight; j++)
			{
				for (int i = 0; i < m_boardDimWidth; i++)
				{
					if (!m_board[i + j * m_boardDimWidth].IsBomb)
						continue;

					if (i - 1 >= 0 && !m_board[i - 1 + j * m_boardDimWidth].IsBomb) // left
						m_board[i - 1 + j * m_boardDimWidth].number++;

					if (i + 1 < m_boardDimWidth && !m_board[i + 1 + j * m_boardDimWidth].IsBomb) // right
						m_board[i + 1 + j * m_boardDimWidth].number++;

					if (j - 1 >= 0 && !m_board[i + (j - 1) * m_boardDimWidth].IsBomb) // top
						m_board[i + (j - 1) * m_boardDimWidth].number++;

					if (j + 1 < m_boardDimHeight && !m_board[i + (j + 1) * m_boardDimWidth].IsBomb) // bottom
						m_board[i + (j + 1) * m_boardDimWidth].number++;

					if (j - 1 >= 0 && i - 1 >= 0 && !m_board[i - 1 + (j - 1) * m_boardDimWidth].IsBomb) // top left
						m_board[i - 1 + (j - 1) * m_boardDimWidth].number++;

					if (j - 1 >= 0 && i + 1 < m_boardDimWidth && !m_board[i + 1 + (j - 1) * m_boardDimWidth].IsBomb) // top right
						m_board[i + 1 + (j - 1) * m_boardDimWidth].number++;

					if (j + 1 < m_boardDimHeight && i + 1 < m_boardDimWidth && !m_board[i + 1 + (j + 1) * m_boardDimWidth].IsBomb) // bottom right
						m_board[i + 1 + (j + 1) * m_boardDimWidth].number++;

					if (j + 1 < m_boardDimHeight && i - 1 >= 0 && !m_board[i - 1 + (j + 1) * m_boardDimWidth].IsBomb) // bottom left
						m_board[i - 1 + (j + 1) * m_boardDimWidth].number++;
				}
			}
		}

		public void MouseClick(int x, int y, bool isRight)
		{
			if (m_state != State.Playing)
				return;

			Vector2Di m = new Vector2Di(x, y);
			Line3Df l = m_device.SceneManager.SceneCollisionManager.GetRayFromScreenCoordinates(m);
			SceneNode n = m_device.SceneManager.SceneCollisionManager.GetSceneNodeFromRayBB(l, 0x10000, m_root);

			if (n != null && n.ID >= 0x10000)
			{
				int i = n.ID - 0x10000;

				if (isRight)
					flagCell(m_board[i]);
				else
					revealCell(m_board[i]);
			}
		}

		void checkVictory()
		{
			bool f = true;

			for (int i = 0; i < m_board.Length; i++)
			{
				Cell c = m_board[i];

				if (c.IsBomb && !c.flagged) // each bomb should be flagged
				{
					f = false;
					break;
				}

				if (!c.IsBomb && !c.revealed) // each number shoud be revealed
				{
					f = false;
					break;
				}
			}

			if (f)
			{
				m_state = State.Won;
				m_device.Logger.Log("game won");
			}
		}

		void flagCell(Cell cell)
		{
			if (cell.revealed)
				return;

			cell.flagged = !cell.flagged;
			m_root.Children[cell.i + cell.j * m_boardDimWidth].Children[0].Visible = cell.flagged;
			m_device.Logger.Log("flagCell: (" + cell.i + "," + cell.j + ") now " + (cell.flagged ? "flagged" : "unflagged"));

			checkVictory();
		}

		void revealCell(Cell cell)
		{
			if (cell.revealed || cell.flagged)
				return;

			cell.revealed = true;
			m_device.Logger.Log("revealCell: (" + cell.i + "," + cell.j + ") now revealed, the number is " + cell.number);

			// if its a bomb - end the game

			if (cell.IsBomb)
			{
				m_root.Children[cell.i + cell.j * m_boardDimWidth].Children[1].Visible = true;

				m_state = State.Lost;
				m_device.Logger.Log("game lost");

				// reveal all bomb cells
				for (int i = 0; i < m_board.Length; i++)
				{
					if (m_board[i].IsBomb)
					{
						m_root.Children[i].Children[1].Visible = true;
					}
				}

				return;
			}

			// this is normal cell

			m_root.Children[cell.i + cell.j * m_boardDimWidth].SetMaterialTexture(0, m_device.VideoDriver.GetTexture("TEXTURE-num-" + cell.number + ".jpg"));

			if (cell.number == 0)
			{
				if (cell.i - 1 >= 0) // left
					revealCell(m_board[cell.i - 1 + cell.j * m_boardDimWidth]);

				if (cell.i + 1 < m_boardDimWidth) // right
					revealCell(m_board[cell.i + 1 + cell.j * m_boardDimWidth]);

				if (cell.j - 1 >= 0) // top
					revealCell(m_board[cell.i + (cell.j - 1) * m_boardDimWidth]);

				if (cell.j + 1 < m_boardDimHeight) // bottom
					revealCell(m_board[cell.i + (cell.j + 1) * m_boardDimWidth]);

				if (cell.j - 1 >= 0 && cell.i - 1 >= 0) // top left
					revealCell(m_board[cell.i - 1 + (cell.j - 1) * m_boardDimWidth]);

				if (cell.j - 1 >= 0 && cell.i + 1 < m_boardDimWidth) // top right
					revealCell(m_board[cell.i + 1 + (cell.j - 1) * m_boardDimWidth]);

				if (cell.j + 1 < m_boardDimHeight && cell.i + 1 < m_boardDimWidth) // bottom right
					revealCell(m_board[cell.i + 1 + (cell.j + 1) * m_boardDimWidth]);

				if (cell.j + 1 < m_boardDimHeight && cell.i - 1 >= 0) // bottom left
					revealCell(m_board[cell.i - 1 + (cell.j + 1) * m_boardDimWidth]);
			}

			checkVictory();
		}

		void loadCellMesh()
		{
			MeshManipulator s = m_device.SceneManager.MeshManipulator;
			VideoDriver d = m_device.VideoDriver;

			m_meshCell = m_device.SceneManager.GetMesh("cell.obj");
			s.FlipSurfaces(m_meshCell); // i don't know why, but somehow this one OBJ exported by Blender has flipped faces when opened by Irrlicht
			fitMesh(m_meshCell);

			s.SetVertexColors(m_meshCell, Color.SolidWhite);
			s.MakePlanarTextureMapping(m_meshCell, 0.10f);
			
			Material m = new Material();
			m.Type = MaterialType.Reflection2Layer;
			m.SetTexture(0, d.GetTexture("TEXTURE-unk.jpg"));
			m.SetTexture(1, d.GetTexture("TEXTURE-ref.jpg"));
			m_meshCell.MeshBuffers[0].SetMaterial(m);

			s.Transform(m_meshCell, new Matrix(new Vector3Df(0), new Vector3Df(0, -90, 180)));
			s.RecalculateNormals(m_meshCell);
		}

		void loadFlagMesh()
		{
			m_meshFlag = m_device.SceneManager.GetMesh("flag.obj");
			fitMesh(m_meshFlag);
		}

		void loadBombMesh()
		{
			m_meshBomb = m_device.SceneManager.GetMesh("bomb.obj");
			fitMesh(m_meshBomb);
		}

		/// <summary>
		/// Prepares mesh to be "cell fit":
		/// 1) uniform scale the mesh to fit to SIZE_OF_MESH x SIZE_OF_MESH on XZ plane;
		/// 2) translates mesh to align its bounding box to center of coordiantes (0,0,0);
		/// </summary>
		void fitMesh(Mesh mesh)
		{
			MeshManipulator s = m_device.SceneManager.MeshManipulator;

			float f = Math.Max(mesh.BoundingBox.Extent.X, mesh.BoundingBox.Extent.Z);
			s.Scale(mesh, new Vector3Df(SIZE_OF_MESH / f));
			s.Transform(mesh, new Matrix(mesh.BoundingBox.MinEdge * -1));
			s.RecalculateNormals(mesh);
		}
	}
}
