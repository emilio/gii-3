using System.Windows.Controls;
using System.Windows.Input;

namespace Circles
{
    class NumericTextBox: TextBox
    {
        public NumericTextBox(): base()
        {
            PreviewTextInput += RejectNonNumericInput;
        }

        private void RejectNonNumericInput(object sender, TextCompositionEventArgs obj)
        {
            double d;
            if ( ! double.TryParse(((NumericTextBox)sender).Text + obj.Text, out d) )
                obj.Handled = true;
        }
    }
}
