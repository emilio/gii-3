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

namespace FigureRotation
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static double DELTA = 4.0;
        private static double ROTATION_DELTA = 4.0;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            Vector delta = new Vector();
            switch (e.Key)
            {
                case Key.Down:
                    delta.Y += DELTA;
                    break;
                case Key.Up:
                    delta.Y -= DELTA;
                    break;
                case Key.Right:
                    delta.X += DELTA;
                    break;
                case Key.Left:
                    delta.X -= DELTA;
                    break;
                default:
                    return;
            }

            MatrixTransform transform = figure.RenderTransform as MatrixTransform;
            if (transform == null) {
                transform = new MatrixTransform();
                figure.RenderTransform = transform;
                figure.RenderTransformOrigin = new Point(figure.Width / 2, figure.Height / 2);
            }
            Matrix m = transform.Matrix;
            m.OffsetX += delta.X;
            m.OffsetY += delta.Y;

            figure.RenderTransform = new MatrixTransform(m);
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            MatrixTransform transform = figure.RenderTransform as MatrixTransform;
            if (transform == null)
            {
                transform = new MatrixTransform();
                figure.RenderTransform = transform;
                figure.RenderTransformOrigin = new Point(figure.Width / 2, figure.Height / 2);
            }

            Matrix m = transform.Matrix;
            m.RotateAt(ROTATION_DELTA, m.OffsetX, m.OffsetY);
            figure.RenderTransform = new MatrixTransform(m);
        }
    }
}
