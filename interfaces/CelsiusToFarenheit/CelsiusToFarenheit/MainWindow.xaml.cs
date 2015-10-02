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

namespace CelsiusToFarenheit
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            Name = "Default";
            InitializeComponent();
        }

        private void HandleConversion(object sender, RoutedEventArgs e)
        {
            string text = Input.Text.Trim();
            string type = (Type.SelectedItem as ComboBoxItem).Name;

            if (Result == null ||
                type == null ||
                text.Length == 0)
                return;

            double from;

            if (!double.TryParse(text, out from))
                return;

            double to;

            switch (type)
            {
                case "celsius":
                    to = FarenheitToCelsius(from);
                    break;
                case "farenheit":
                    to = CelsiusToFarenheit(from);
                    break;
                default:
                    return;
            }

            Result.Text = to.ToString();
        }

        private double CelsiusToFarenheit(double celsius)
        {
            return celsius * 1.8 + 32;
        }

        private double FarenheitToCelsius(double farenheit)
        {
            return (farenheit - 32) / 1.8;
        }

        private void Type_Loaded(object sender, RoutedEventArgs e)
        {
            (sender as ComboBox).SelectedIndex = 0;
        }
    }
}
