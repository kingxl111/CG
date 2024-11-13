using System;
using System.Collections.Generic;
using OpenTK;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Windowing.GraphicsLibraryFramework;

namespace Lab1
{
    public class Game : GameWindow
    {
        private float frameTime = 0.0f;
        private int fps = 0;
        private List<Vector2> controlPoints; // Контрольные точки

        private int vboVertex = 0;
        private int vaoId = 0;
        private int vboLine = 0;  // VBO для линий

        private int selectedPoint = -1; // Индекс выбранной точки
        private const int CircleSegments = 30; // Количество сегментов для окружности

        public Game(GameWindowSettings gameWindowSettings, NativeWindowSettings nativeWindowSettings)
            : base(gameWindowSettings, nativeWindowSettings)
        {
            controlPoints = new List<Vector2>
            {
                new Vector2(-0.5f, -0.5f),
                new Vector2(0.0f, 0.0f),
                new Vector2(0.5f, 0.5f)
            };
        }

        protected override void OnLoad()
        {
            base.OnLoad();
            GL.ClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Темный фон

            // Создаем VAO и VBO для точек и линий
            vaoId = CreateVAO();
        }

        protected override void OnResize(ResizeEventArgs e)
        {
            base.OnResize(e);
        }

        protected override void OnUpdateFrame(FrameEventArgs args)
        {
            base.OnUpdateFrame(args);
            frameTime += (float)args.Time;
            fps++;

            if (frameTime >= 1.0f)
            {
                Title = $"LearnOpenTK FPS - {fps}";
                frameTime = 0.0f;
                fps = 0;
            }

            var input = KeyboardState;

            // Перемещение выбранной точки
            if (selectedPoint >= 0 && selectedPoint < controlPoints.Count)
            {
                if (input.IsKeyDown(Keys.Up)) controlPoints[selectedPoint] += new Vector2(0, 0.01f);
                if (input.IsKeyDown(Keys.Down)) controlPoints[selectedPoint] -= new Vector2(0, 0.01f);
                if (input.IsKeyDown(Keys.Left)) controlPoints[selectedPoint] -= new Vector2(0.01f, 0);
                if (input.IsKeyDown(Keys.Right)) controlPoints[selectedPoint] += new Vector2(0.01f, 0);
            }

            // Переключение между точками с помощью Tab
            if (input.IsKeyPressed(Keys.Tab))
            {
                selectedPoint = (selectedPoint + 1) % controlPoints.Count;
            }

            // Добавление новой точки через Enter
            if (input.IsKeyPressed(Keys.Enter))
            {
                Vector2 newPoint = new Vector2(0.0f, 0.0f); // Новая точка в центре
                controlPoints.Add(newPoint);
            }

            // Удаление точки через Delete
            if (input.IsKeyPressed(Keys.Delete) && controlPoints.Count > 2)
            {
                controlPoints.RemoveAt(selectedPoint);
                selectedPoint = selectedPoint % controlPoints.Count;
            }

            // Обновляем VAO с новыми точками
            vaoId = CreateVAO();
        }

        private int CreateVBO(float[] data)
        {
            int vbo = GL.GenBuffer();
            GL.BindBuffer(BufferTarget.ArrayBuffer, vbo);
            GL.BufferData(BufferTarget.ArrayBuffer, data.Length * sizeof(float), data, BufferUsageHint.StaticDraw);
            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            return vbo;
        }

        private int CreateVAO()
        {
            int vao = GL.GenVertexArray();
            GL.BindVertexArray(vao);

            // Генерируем массив данных для отрисовки точек
            List<float> vertexData = new List<float>();
            List<float> lineData = new List<float>();

            // Заполняем данные для точек
            for (int i = 0; i < controlPoints.Count; i++)
            {
                vertexData.Add(controlPoints[i].X);
                vertexData.Add(controlPoints[i].Y);
            }

            // Заполняем данные для линий (соединяем точки)
            for (int i = 1; i < controlPoints.Count; i++)
            {
                lineData.Add(controlPoints[i - 1].X);
                lineData.Add(controlPoints[i - 1].Y);
                lineData.Add(controlPoints[i].X);
                lineData.Add(controlPoints[i].Y);
            }

            // Создаем VBO для точек и линий
            vboVertex = CreateVBO(vertexData.ToArray());
            vboLine = CreateVBO(lineData.ToArray());

            GL.BindVertexArray(0);
            return vao;
        }

        private void DrawPolyline()
        {
            GL.Color3(0.0f, 0.0f, 1.0f); // Синий цвет для линии
            GL.BindBuffer(BufferTarget.ArrayBuffer, vboLine);
            GL.EnableClientState(ArrayCap.VertexArray);
            GL.VertexPointer(2, VertexPointerType.Float, 0, 0);
            GL.DrawArrays(PrimitiveType.Lines, 0, controlPoints.Count - 1);
            GL.DisableClientState(ArrayCap.VertexArray);
            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
        }

        private void DrawCircle(float x, float y, float radius)
        {
            GL.Begin(PrimitiveType.TriangleFan);
            GL.Color3(1.0f, 0.0f, 0.0f);  // Красный цвет
            GL.Vertex2(x, y); // Центр окружности

            for (int i = 0; i <= CircleSegments; i++)
            {
                float angle = MathHelper.TwoPi * i / CircleSegments;
                float dx = (float)Math.Cos(angle) * radius;
                float dy = (float)Math.Sin(angle) * radius;
                GL.Vertex2(x + dx, y + dy);
            }
            GL.End();
        }

        private void DrawControlPoints()
        {
            const float radius = 0.05f; // Радиус окружности
            foreach (var point in controlPoints)
            {
                DrawCircle(point.X, point.Y, radius);
            }
        }

        protected override void OnRenderFrame(FrameEventArgs args)
        {
            GL.Clear(ClearBufferMask.ColorBufferBit);

            // Отображаем ломаную кривую (линии между точками)
            DrawPolyline();

            // Отображаем контрольные точки как окружности
            DrawControlPoints();

            SwapBuffers();
            base.OnRenderFrame(args);
        }

        protected override void OnUnload()
        {
            GL.DeleteBuffer(vboVertex);
            GL.DeleteBuffer(vboLine);  // Удаляем VBO для линий
            GL.DeleteVertexArray(vaoId);
            base.OnUnload();
        }

        static void Main(string[] args)
        {
            var nativeWinSettings = new NativeWindowSettings()
            {
                ClientSize = new Vector2i(800, 600),
                Location = new Vector2i(370, 300),
                WindowBorder = WindowBorder.Resizable,
                WindowState = WindowState.Normal,
                Title = "Polyline with Control Points",
                Flags = ContextFlags.Default,
                API = ContextAPI.OpenGL,
                Profile = ContextProfile.Compatability,
                APIVersion = new Version(3, 3)
            };

            using (Game game = new Game(GameWindowSettings.Default, nativeWinSettings))
            {
                game.Run();
            }
        }
    }
}
