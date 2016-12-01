using System;
using System.ComponentModel;
using System.Globalization;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;

namespace G27PedalsAndShifterConfigurator.Wpf
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Private Members

        private UsbDeviceHelper _usbHelper;
        private Timer _statusTimer;
        private DeviceCalibration _calibration;

        #endregion

        #region Properties

        public string ComPort => cmbComPort.SelectedValue?.ToString();

        #endregion

        #region Constructors

        public MainWindow()
        {
            InitializeComponent();
            // just a sample, may or may not be accurate.
            //DisplayShifterCalibration(new ShifterCalibration {
            //    Max1X = 267,
            //    Max2X = 254,
            //    Max3X = 548,
            //    Max4X = 558,
            //    Min3X = 327,
            //    Min4X = 295,
            //    Min5X = 634,
            //    Min6X = 609,
            //    MinY = 76,
            //    MaxY = 962
            //});
        }

        #endregion

        #region Private Methods

        private void InitTimer()
        {
            _statusTimer = new Timer {Interval = 50};
            _statusTimer.Elapsed += (sender, args) => _usbHelper.DisplayStatus();
            _statusTimer.Enabled = true;
        }

        private int ScaleInt(int value, double max = 100)
        {
            double d = (double)value;
            return Convert.ToInt32(d * max / 1023.0);
        }

        private void SetShifterCalibration()
        {
            var minY = 0;
            var maxY = cvsShifter.Height;
            var upperY = ScaleInt(_calibration.Shifter.UpperY, cvsShifter.Height);
            var lowerY = ScaleInt(_calibration.Shifter.LowerY, cvsShifter.Height);

            sldTopGate.Value = _calibration.Shifter.UpperY;
            sldBottomGate.Value = _calibration.Shifter.LowerY;

            lneGate13.X1 = lneGate13.X2 = ScaleInt(_calibration.Shifter.Gate13, cvsShifter.Height);
            lneGate13.Y1 = minY;
            lneGate13.Y2 = upperY;

            lneGate24.X1 = lneGate24.X2 = ScaleInt(_calibration.Shifter.Gate24, cvsShifter.Height);
            lneGate24.Y1 = maxY;
            lneGate24.Y2 = lowerY;

            lneGate35.X1 = lneGate35.X2 = ScaleInt(_calibration.Shifter.Gate35, cvsShifter.Height);
            lneGate35.Y1 = minY;
            lneGate35.Y2 = upperY;

            lneGate46.X1 = lneGate46.X2 = ScaleInt(_calibration.Shifter.Gate46, cvsShifter.Height);
            lneGate46.Y1 = maxY;
            lneGate46.Y2 = lowerY;
        }

        private void DisplayDeviceStatus(DeviceState state)
        {
            rtbOutput.Document.Blocks.Clear();
            string output = null;

            if (state == null)
            {
                output = "Error getting device status";
            }
            else
            {
                output = state.ToString();

                pbActualThrottle.Value = ScaleInt(state.Pedals.Throttle);
                pbActualBrake.Value = ScaleInt(state.Pedals.Brake);
                pbActualClutch.Value = ScaleInt(state.Pedals.Clutch);

                Canvas.SetTop(rctShifterPosition, cvsShifter.Height - ScaleInt(state.Shifter.YPosition, cvsShifter.Height) - rctShifterPosition.Height / 2);
                Canvas.SetLeft(rctShifterPosition, ScaleInt(state.Shifter.XPosition, cvsShifter.Width - rctShifterPosition.Width / 2));
                var gear = "N";

                if (_calibration.Shifter != null)
                {
                    if (state.Shifter.YPosition > _calibration.Shifter.LowerY)
                    {
                        if (state.Shifter.XPosition.Between(0, _calibration.Shifter.Gate13))
                        {
                            gear = "1";
                        }
                        else if (state.Shifter.XPosition.Between(_calibration.Shifter.Gate13, _calibration.Shifter.Gate35))
                        {
                            gear = "3";
                        }
                        else if (state.Shifter.XPosition.Between(_calibration.Shifter.Gate35, 1023))
                        {
                            gear = "5";
                        }
                    }
                    else if (state.Shifter.YPosition < _calibration.Shifter.UpperY)
                    {
                        if (state.Shifter.XPosition.Between(0, _calibration.Shifter.Gate24))
                        {
                            gear = "2";
                        }
                        else if (state.Shifter.XPosition.Between(_calibration.Shifter.Gate24, _calibration.Shifter.Gate46))
                        {
                            gear = "4";
                        }
                        else if (state.Shifter.XPosition.Between(_calibration.Shifter.Gate46, 1023))
                        {
                            gear = "6";
                        }
                    }
                }

                lblGear.Text = gear;
            }

            rtbOutput.Document.Blocks.Add(new Paragraph(new Run(output)));
        }

        private void DisconnectErthang()
        {
            _usbHelper.Disconnect();
            _usbHelper.Dispose();
            _statusTimer.Enabled = false;
            _statusTimer.Dispose();
        }

        #endregion

        #region Event Handlers

        private void btnConnectDevice_Click(object sender, RoutedEventArgs e)
        {
            string message;
            if (!ConnectDeviceValidator.IsValid(this, out message))
            {
                MessageBox.Show(this, message);
                return;
            }

            _usbHelper = new UsbDeviceHelper(ComPort,
                s => Dispatcher.Invoke(() => {
                    DisplayDeviceStatus(s);
                }));

            btnDisconnectDevice.Visibility = Visibility.Visible;
            btnConnectDevice.Visibility = Visibility.Hidden;

            _calibration = _usbHelper.GetCalibration();
            if (!_calibration.Shifter.IsAllZeroes)
            {
                SetShifterCalibration();
            }

            InitTimer();
        }

        private void btnDisconnectDevice_Click(object sender, RoutedEventArgs e)
        {
            DisconnectErthang();

            btnDisconnectDevice.Visibility = Visibility.Hidden;
            btnConnectDevice.Visibility = Visibility.Visible;
        }

        private void btnCalibrateShifter_Click(object sender, RoutedEventArgs e)
        {
            _calibration.Shifter = new ShifterCalibrator(this, _usbHelper).Calibrate();
            SetShifterCalibration();
        }

        private void sldBottomGate_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (_calibration?.Shifter != null && e.NewValue != e.OldValue)
            {
                _calibration.Shifter.LowerY = (int)e.NewValue;
                SetShifterCalibration();
            }
        }

        private void sldTopGate_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (_calibration?.Shifter != null && e.NewValue != e.OldValue)
            {
                _calibration.Shifter.UpperY = (int)e.NewValue;
                SetShifterCalibration();
            }
        }

        private void btnWriteToDevice_Click(object sender, RoutedEventArgs e)
        {
            _usbHelper.SetCalibration(_calibration);
        }

        private void Window_Closing(object sender, CancelEventArgs e)
        {
            if (_usbHelper != null && _usbHelper.IsConncected)
            {
                DisconnectErthang();
            }
        }

        #endregion
    }
}
