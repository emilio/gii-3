using System;
using System.Windows.Media;

namespace Circles
{
    using System.ComponentModel;

    public class ViewModel : INotifyPropertyChanged
    {
        private double _BounceFactor = 1.0;
        private double _Radius = 10.0;
        private double _BorderThickness = 2.0;
        private double _Mass = 10.0;
        private double _Gravity = 300.0;
        private Brush _BorderBrush = Brushes.Black;
        private Brush _FillBrush = Brushes.Transparent;

        public event PropertyChangedEventHandler PropertyChanged;

        public double Gravity {
            get { return _Gravity; }
            set { _Gravity = value; OnPropertyChanged("Gravity"); }
        }

        public double Mass {
            get { return _Mass; }
            set { _Mass = value; OnPropertyChanged("Mass"); }
        }

        public double BounceFactor {
            get { return _BounceFactor; }
            set { _BounceFactor = value; OnPropertyChanged("BounceFactor"); }
        }

        public double Radius {
            get { return _Radius;  }
            set { _Radius = value;  OnPropertyChanged("Radius");  }
        }

        public double BorderThickness {
            get { return _BorderThickness;  }
            set { _BorderThickness = value;  OnPropertyChanged("BorderThickness");  }
        }

        public Brush BorderBrush {
            get { return _BorderBrush;  }
            set { _BorderBrush = value;  OnPropertyChanged("BorderBrush");  }
        }

        public Brush FillBrush {
            get { return _FillBrush;  }
            set { _FillBrush = value;  OnPropertyChanged("FillBrush");  }
        }

        protected virtual void OnPropertyChanged(string propertyName)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}