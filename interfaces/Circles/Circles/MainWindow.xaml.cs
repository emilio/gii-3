using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using System.Diagnostics;

namespace Circles
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ViewModel ViewModel;
        private World World;
        private DispatcherTimer Timer;
        private ConfigurationWindow ConfigurationWindow;

        const double WORLD_WIDTH = 500.0;
        const double WORLD_HEIGHT = 500.0;
        const uint PHYSICS_PER_SECOND = 120;
        const uint FRAMES_PER_SECOND = 120;
        const uint PHYSICS_PER_FRAME = PHYSICS_PER_SECOND / FRAMES_PER_SECOND;
        private static uint iters = 0;

        public MainWindow()
        {
            InitializeComponent();
            this.ViewModel = new ViewModel();
            this.World = new World(WORLD_WIDTH, WORLD_HEIGHT, ViewModel.Gravity);
            this.Timer = new DispatcherTimer();
            this.Timer.Interval = new TimeSpan((long)(1E9 / PHYSICS_PER_SECOND / 100));
            this.Timer.Tick += OnTick;
            this.Timer.Start();

            Canvas.Width = WORLD_WIDTH;
            Canvas.Height = WORLD_HEIGHT;
        }

        public void AddCircle(Point position) {
            World.Objects.Add(new MovingCircle(ViewModel.Radius, ViewModel.Mass, ViewModel.BounceFactor, position - new Vector(ViewModel.Radius, ViewModel.Radius), new Vector(0, 0)));
        }

        private void Draw() {
            Canvas.Children.Clear();
            foreach (MovingCircle obj in World.Objects)
            {
                Canvas.Children.Add(obj.circle);
                Canvas.SetLeft(obj.circle, obj.Position.X);
                Canvas.SetTop(obj.circle, obj.Position.Y);
            }
        }

        private void HandleMouseDown(object sender, MouseButtonEventArgs e)
        {
            Debug.Print("MouseDown");
            AddCircle(e.GetPosition(Canvas));
        }

        private void OnTick(object sender, EventArgs e) {
            World.Tick(this.Timer.Interval.TotalMilliseconds, ViewModel.Gravity);
            if ( iters % PHYSICS_PER_FRAME == 0 )
                Draw();

            iters++;
        }

        private void OpenConfigurationDialog(object sender, EventArgs e) {
            if (ConfigurationWindow == null || !ConfigurationWindow.IsVisible)
            {
                ConfigurationWindow = new ConfigurationWindow();
                ConfigurationWindow.DataContext = ViewModel;
                ConfigurationWindow.Show();
            }

            ConfigurationWindow.Focus();
        }
    }
}
