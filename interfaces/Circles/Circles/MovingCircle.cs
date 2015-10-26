using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Circles
{
    class MovingCircle
    {
        // TODO: This shouldn't be public, that potentially breaks the "circleness"
        // I should write setters for things like brush or similar
        public Ellipse circle;

        private static Random Random = new Random();

        public Point Position { get; set; }
        public Vector Speed { get; set; }

        public double BounceFactor { get; set; }
        public double Mass { get; set; }

        // This is used to store the lineal momentum force when we calculate collisions
        // Since we just operate on each collision once (not once per ball involved). 
        public Vector PrecalculatedDeltaF { get; set; }

        public double Diameter {
            get
            {
                return this.Radius * 2;
            }
        }
        public Point Center
        {
            get {
                return new Point(this.Position.X + this.Radius, this.Position.Y + this.Radius);
            }
        }

        public double Radius { get; set; }

        // TODO: change speed -> initial speed?
        public MovingCircle(double radius, double mass, double bounceFactor, Point position, Vector speed) {
            this.circle = new Ellipse();

            circle.Height = circle.Width = radius * 2;
            circle.Stroke = Brushes.Black;
            // circle.Stroke = new SolidColorBrush(Color.FromRgb((byte)Random.Next(255), (byte)Random.Next(255), (byte)Random.Next(255)));
            circle.StrokeThickness = 2;

            circle.Fill = new SolidColorBrush(Color.FromRgb((byte)Random.Next(255), (byte)Random.Next(255), (byte)Random.Next(255)));

            this.Radius = radius;
            this.BounceFactor = bounceFactor;
            this.Speed = speed;
            this.Position = position;
            this.Mass = mass;
        }
    }
}
