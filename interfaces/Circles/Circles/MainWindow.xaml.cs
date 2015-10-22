using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Circles
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private World World;
        private DispatcherTimer Timer;

        const double WORLD_WIDTH = 500.0;
        const double WORLD_HEIGHT = 500.0;

        public MainWindow()
        {
            InitializeComponent();
            this.World = new World(WORLD_WIDTH, WORLD_HEIGHT);
            this.Timer = new DispatcherTimer();
            // 1E9 / 60 = nanoseconds to 60fps
            this.Timer.Interval = new TimeSpan(((long)1E9 / 60 / 100));
            this.Timer.Tick += OnTick;
            this.Timer.Start();

            Canvas.Width = WORLD_WIDTH;
            Canvas.Height = WORLD_HEIGHT;
        }

        public void AddCircle(Point position) {
            World.Objects.Add(new MovingCircle(10, 10, .9, position, new Vector(0, 0)));
        }

        public void Draw() {
            Canvas.Children.Clear();
            foreach (MovingCircle obj in World.Objects)
            {
                Canvas.Children.Add(obj.circle);
                Canvas.SetLeft(obj.circle, obj.Position.X);
                Canvas.SetTop(obj.circle, obj.Position.Y);
            }
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            AddCircle(e.GetPosition(Canvas));
            Draw();
        }

        private void OnTick(object sender, EventArgs e) {
            World.Tick(this.Timer.Interval.TotalMilliseconds);
            Draw();
        }
    }
}
