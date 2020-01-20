using IrrlichtLime;
using IrrlichtLime.Core;
using IrrlichtLime.Video;
using IrrlichtLime.Scene;
using IrrlichtLime.GUI;

namespace L17.Minesweeper
{
	class Program
	{
		const string TextStart = "PRESS [ESC] FOR GAME MENU";
		const string TextLost = "YOU LOST\n" + TextStart;
		const string TextWon = "YOU WON\n" + TextStart;

		static IrrlichtDevice device;
		static CameraSceneNode camera;
		static LightSceneNode light;
		static GUIStaticText text;
		static GUIWindow window;
		static Game game;

		static bool optionShadows = true;
		static bool optionBackground = true;
		static bool optionFPS = false;

		static void Main()
		{
			device = IrrlichtDevice.CreateDevice(DriverType.OpenGL, new Dimension2Di(1024, 768), 32, false, true);
			device.OnEvent += new IrrlichtDevice.EventHandler(device_OnEvent);
			device.SetWindowCaption("Minesweeper - Irrlicht Engine");

			VideoDriver driver = device.VideoDriver;
			SceneManager scene = device.SceneManager;
			GUIEnvironment gui = device.GUIEnvironment;

			device.FileSystem.AddFileArchive("../../media/minesweeper.media.files");
			game = new Game(device);

			// set up camera

			camera = scene.AddCameraSceneNode(); // Maya(null, -500, 50, 200);
			setupCameraPositionAndTarget();

			// set up background

			Mesh m = scene.GetMesh("back.obj");
			scene.MeshManipulator.Scale(m, new Vector3Df(80));
			scene.MeshManipulator.FlipSurfaces(m);
			scene.MeshManipulator.RecalculateNormals(m);
			scene.MeshManipulator.SetVertexColors(m, new Color(80, 80, 80));

			Material t = new Material();
			t.Type = MaterialType.Reflection2Layer;
			t.Lighting = false;
			t.SetTexture(0, device.VideoDriver.GetTexture("TEXTURE-ref.jpg"));
			t.SetTexture(1, device.VideoDriver.GetTexture("TEXTURE-ref.jpg"));

			MeshSceneNode n = scene.AddMeshSceneNode(m, null, 7777);
			n.SetMaterial(0, t);
			n.Position = game.CenterOfTheBoard;

			SceneNodeAnimator a = scene.CreateRotationAnimator(new Vector3Df(0.07f, 0.01f, 0.05f));
			n.AddAnimator(a);
			a.Drop();

			// set up light

			light = scene.AddLightSceneNode(null, game.CenterOfTheBoard, new Colorf(1, 1, 1), 200);

			m = scene.AddVolumeLightMesh("lvol1", 32, 32, new Color(5, 5, 5));
			scene.MeshManipulator.Scale(m, new Vector3Df(15, 30, 15));
			n = scene.AddMeshSceneNode(m, light);
			n.Position = new Vector3Df(0, -10, 0);
			n.Rotation = new Vector3Df(180, 0, 0);
			n.SetMaterialType(MaterialType.TransparentAddColor);

			m = scene.AddVolumeLightMesh("lvol2", 32, 32, new Color(5, 5, 5));
			scene.MeshManipulator.Scale(m, new Vector3Df(15, 30, 15));
			n = scene.AddMeshSceneNode(m, light);
			n.Position = new Vector3Df(0, -10, 0);
			n.SetMaterialType(MaterialType.TransparentAddColor);

			// add gui hint

			text = gui.AddStaticText(TextStart, new Recti(10, 10, 200, 40));
			text.OverrideColor = Color.SolidYellow;

			// main loop

			while (device.Run())
			{
				driver.BeginScene();
				scene.DrawAll();
				gui.DrawAll();

				if (optionFPS)
					gui.BuiltInFont.Draw(driver.FPS + " FPS", driver.ScreenSize.Width - 50, 10, Color.SolidWhite);

				driver.EndScene();
			}

			device.Drop();
		}

		static bool device_OnEvent(Event evnt)
		{
			if (evnt.Type == EventType.Mouse &&
				evnt.Mouse.Type == MouseEventType.Move)
			{
				Vector2Di m = new Vector2Di(evnt.Mouse.X, evnt.Mouse.Y);
				Line3Df l = device.SceneManager.SceneCollisionManager.GetRayFromScreenCoordinates(m);
				Plane3Df p = new Plane3Df(new Vector3Df(0, 0, 0), new Vector3Df(100, 0, 0), new Vector3Df(0, 0, 100));
				Vector3Df i = p.GetIntersectionWithLimitedLine(l.Start, l.End);
				if (i != null)
				{
					camera.Target = game.CenterOfTheBoard + new Vector3Df(
						(m.Y - device.VideoDriver.ScreenSize.Height / 2) / 100.0f,
						0,
						(m.X - device.VideoDriver.ScreenSize.Width / 2) / 100.0f);

					i.Y += 25; // we want light to be a little bit above
					light.Position = i;
				}
			}

			if (window == null &&
				evnt.Type == EventType.Mouse &&
				(evnt.Mouse.Type == MouseEventType.LeftDown || evnt.Mouse.Type == MouseEventType.RightDown))
			{
				text.Visible = false; // if user started to play - remove the gui text
				game.MouseClick(evnt.Mouse.X, evnt.Mouse.Y, evnt.Mouse.Type == MouseEventType.RightDown);

				if (game.StateOfTheGame != Game.State.Playing)
				{
					text.Visible = true;
					text.Text = game.StateOfTheGame == Game.State.Won ? TextWon : TextLost;
				}

				return true;
			}

			if (evnt.Type == EventType.Key &&
				evnt.Key.PressedDown &&
				evnt.Key.Key == KeyCode.Esc)
			{
				if (window != null)
				{
					window.Remove();
					window = null;
					return true;
				}

				GUIEnvironment gui = device.GUIEnvironment;

				window = gui.AddWindow(new Recti(100, 100, 400, 400), true, "GAME MENU");

				gui.AddButton(new Recti(20, 40, window.ClientRect.Width - 20, 60), window, 1510, "NEW GAME 5x5");
				gui.AddButton(new Recti(20, 60, window.ClientRect.Width - 20, 80), window, 1520, "NEW GAME 10x10");
				gui.AddButton(new Recti(20, 80, window.ClientRect.Width - 20, 100), window, 1530, "NEW GAME 15x15");
				gui.AddButton(new Recti(20, 100, window.ClientRect.Width - 20, 120), window, 1540, "NEW GAME 20x20");

				gui.AddCheckBox(optionShadows, new Recti(20, 140, window.ClientRect.Width - 20, 160), "SHOW REALTIME SHADOWS", window, 1710);
				gui.AddCheckBox(optionBackground, new Recti(20, 160, window.ClientRect.Width - 20, 180), "SHOW BACKGROUND", window, 1720);
				gui.AddCheckBox(optionFPS, new Recti(20, 180, window.ClientRect.Width - 20, 200), "SHOW FPS", window, 1730);

				gui.AddButton(new Recti(20, 260, window.ClientRect.Width - 20, 280), window, 1590, "EXIT GAME");

				return true;
			}

			if (window != null &&
				evnt.Type == EventType.GUI)
			{
				if (evnt.GUI.Caller == window &&
					evnt.GUI.Type == GUIEventType.ElementClosed)
				{
					window.Remove();
					window = null;
					return true;
				}

				if (evnt.GUI.Caller.ID == 1510 &&
					evnt.GUI.Type == GUIEventType.ButtonClicked)
				{
					window.Remove();
					window = null;
					game.NewGame(5, 5);
					setupCameraPositionAndTarget();
					return true;
				}

				if (evnt.GUI.Caller.ID == 1520 &&
					evnt.GUI.Type == GUIEventType.ButtonClicked)
				{
					window.Remove();
					window = null;
					game.NewGame(10, 10);
					setupCameraPositionAndTarget();
					return true;
				}

				if (evnt.GUI.Caller.ID == 1530 &&
					evnt.GUI.Type == GUIEventType.ButtonClicked)
				{
					window.Remove();
					window = null;
					game.NewGame(15, 15);
					setupCameraPositionAndTarget();
					return true;
				}

				if (evnt.GUI.Caller.ID == 1540 &&
					evnt.GUI.Type == GUIEventType.ButtonClicked)
				{
					window.Remove();
					window = null;
					game.NewGame(20, 20);
					setupCameraPositionAndTarget();
					return true;
				}

				if (evnt.GUI.Caller.ID == 1590 &&
					evnt.GUI.Type == GUIEventType.ButtonClicked)
				{
					device.Close();
					return true;
				}

				if (evnt.GUI.Caller.ID == 1710 &&
					evnt.GUI.Type == GUIEventType.CheckBoxChanged)
				{
					optionShadows = (evnt.GUI.Caller as GUICheckBox).Checked;
					light.CastShadows = optionShadows;
					return true;
				}

				if (evnt.GUI.Caller.ID == 1720 &&
					evnt.GUI.Type == GUIEventType.CheckBoxChanged)
				{
					optionBackground = (evnt.GUI.Caller as GUICheckBox).Checked;
					device.SceneManager.GetSceneNodeFromID(7777).Visible = optionBackground;
					return true;
				}

				if (evnt.GUI.Caller.ID == 1730 &&
					evnt.GUI.Type == GUIEventType.CheckBoxChanged)
				{
					optionFPS = (evnt.GUI.Caller as GUICheckBox).Checked;
					return true;
				}
			}

			return false;
		}

		private static void setupCameraPositionAndTarget()
		{
			camera.Position = game.CenterOfTheBoard + new Vector3Df(game.CenterOfTheBoard.X, game.CenterOfTheBoard.X * 1.5f, 0);
			camera.Target = game.CenterOfTheBoard;
		}
	}
}
