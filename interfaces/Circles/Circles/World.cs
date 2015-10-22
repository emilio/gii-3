using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;


namespace Circles
{
    class World
    {
        public List<MovingCircle> Objects;
        public double Width { get; }
        public double Height { get; }

        public World(double width, double height) {
            this.Objects = new List<MovingCircle>();
            this.Width = width;
            this.Height = height;
        }

        public void Tick(double timestep) {
            timestep /= 1000;
            foreach (MovingCircle obj in Objects) {
                Vector acceleration = force(obj) / obj.Mass;

                obj.Position += timestep * (obj.Speed + timestep * acceleration / 2);

                // Vector newAcceleration = force(obj) / obj.Mass;
                // obj.Speed += timestep * (acceleration + newAcceleration) / 2;
                obj.Speed += timestep * acceleration;
                Debug.WriteLine(obj.Speed);
            }
        }
        public Vector force(MovingCircle obj) {
            // Vector f = new Vector(0.0, 9.8) * obj.Mass; // Gravity
            // Actually the gravity is in px/s/s... Lol
            Vector f = new Vector(0.0, 300) * obj.Mass; // Gravity

            // if ( object touches boundary )
            //     f += normal; (f = 0);
            // TODO: Consider using < instead of <=
            if (obj.Position.Y <= 0) {
                Debug.Print("Collided with roof");
                // Do nothing
                // obj.Speed = new Vector(obj.Speed.X, Math.Abs(obj.Speed.Y) * obj.BounceFactor);
                // obj.Position = new Point(obj.Position.X, 0);
            } else if (obj.Position.Y + obj.Diameter >= this.Height) {
                Debug.Print("Collided with floor");
                obj.Speed = new Vector(obj.Speed.X, -Math.Abs(obj.Speed.Y) * obj.BounceFactor);
                obj.Position = new Point(obj.Position.X, this.Height - obj.Diameter);
                // Equivalent to apply the normal
                f.Y = 0;
                // Minus the force provoked by our lineal moment
                // f += new Vector(0, obj.Mass * -obj.Speed.Y);
            }

            if (obj.Position.X <= 0) {
                Debug.Print("Collided with left wall");
                obj.Position = new Point(0, obj.Position.Y);
                obj.Speed = new Vector(Math.Abs(obj.Speed.X) * obj.BounceFactor, obj.Speed.Y);
                // Lineal moment:
                // p = m * v
                // F = dp/dt
                f += new Vector(obj.Mass * -obj.Speed.X, 0);
            } else if (obj.Position.X + obj.Diameter >= this.Width) {
                obj.Position = new Point(this.Height - obj.Diameter, obj.Position.Y);
                obj.Speed = new Vector(-Math.Abs(obj.Speed.X) * obj.BounceFactor, obj.Speed.Y);
                // Lineal moment:
                // p = m * v
                // F = dp/dt
                f += new Vector(obj.Mass * -obj.Speed.X, 0);
            }

            int len = Objects.Count;
            bool detectingCollisions = false;
            // We only detect collisions for bodies after us in the list
            for (int i = 0; i < len; ++i)
            {
                MovingCircle other = Objects.ElementAt(i);

                if (ReferenceEquals(obj, other))
                {
                    detectingCollisions = true;
                    continue;
                }

                if (!detectingCollisions)
                    continue;

                Vector centerToCenter = (other.Center - obj.Center);

                // They collide
                if (centerToCenter.Length <= obj.Diameter)
                {
                    centerToCenter.Normalize();
                    f += new Vector(other.Speed.X * other.Mass, other.Speed.Y * other.Mass);
                    f += new Vector(-obj.Speed.X * obj.Mass, -obj.Speed.Y * obj.Mass);

                    // TODO: Either avoid entering here when the objects are running one after the other, or force the position around the collision point

                    // http://ericleong.me/research/circle-circle/#static-circle-circle-collision-detection
                    double p = 2 * (obj.Speed.X * centerToCenter.X + obj.Speed.Y * centerToCenter.Y - other.Speed.X * centerToCenter.X - other.Speed.Y * centerToCenter.Y) / (obj.Mass + other.Mass);
                    obj.Speed = new Vector((obj.Speed.X - p * obj.Mass * centerToCenter.X) * obj.BounceFactor, (obj.Speed.Y - p * obj.Mass * centerToCenter.Y) * obj.BounceFactor);
                    other.Speed = new Vector((other.Speed.X + p * other.Mass * centerToCenter.X) * other.BounceFactor, (other.Speed.Y + p * other.Mass * centerToCenter.Y) * other.BounceFactor);
                }
            }

            // if ( object collides with other ) 
            //      add normal force to both objects
            // TODO: Check if object collides with others and add the normal force

            return f;
        }
    }
}
