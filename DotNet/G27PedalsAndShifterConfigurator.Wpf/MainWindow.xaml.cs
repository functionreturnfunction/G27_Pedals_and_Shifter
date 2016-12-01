using System;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
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

        private int ScaleToPercent(int value)
        {
            double d = (double)value;
            return Convert.ToInt32(d * 100.0 / 1023.0);
        }

        private void SetShifterCalibration(ShifterCalibration calibration)
        {
            _calibration.Shifter = calibration;

            var minY = ScaleToPercent(calibration.MinY);
            var maxY = calibration.MaxY == 0 ? 100 : ScaleToPercent(calibration.MaxY);
            var upperY = ScaleToPercent(calibration.UpperY);
            var lowerY = ScaleToPercent(calibration.LowerY);

            sldTopGate.Value = calibration.UpperY;
            sldBottomGate.Value = calibration.LowerY;

            lneGate13.X1 = lneGate13.X2 = ScaleToPercent(calibration.Gate13);
            lneGate13.Y1 = minY;
            lneGate13.Y2 = upperY;

            lneGate24.X1 = lneGate24.X2 = ScaleToPercent(calibration.Gate24);
            lneGate24.Y1 = maxY;
            lneGate24.Y2 = lowerY;

            lneGate35.X1 = lneGate35.X2 = ScaleToPercent(calibration.Gate35);
            lneGate35.Y1 = minY;
            lneGate35.Y2 = upperY;

            lneGate46.X1 = lneGate46.X2 = ScaleToPercent(calibration.Gate46);
            lneGate46.Y1 = maxY;
            lneGate46.Y2 = lowerY;
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
                SetShifterCalibration(_calibration.Shifter);
            }

            InitTimer();
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

                pbActualThrottle.Value = ScaleToPercent(state.Pedals.Throttle);
                pbActualBrake.Value = ScaleToPercent(state.Pedals.Brake);
                pbActualClutch.Value = ScaleToPercent(state.Pedals.Clutch);

                Canvas.SetTop(rctShifterPosition, 100 - ScaleToPercent(state.Shifter.YPosition));
                Canvas.SetLeft(rctShifterPosition, ScaleToPercent(state.Shifter.XPosition));
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

        private void btnDisconnectDevice_Click(object sender, RoutedEventArgs e)
        {
            _usbHelper.Disconnect();
            _usbHelper.Dispose();
            _statusTimer.Enabled = false;
            _statusTimer.Dispose();

            btnDisconnectDevice.Visibility = Visibility.Hidden;
            btnConnectDevice.Visibility = Visibility.Visible;
        }

        private void btnCalibrateShifter_Click(object sender, RoutedEventArgs e)
        {
            SetShifterCalibration(new ShifterCalibrator(this, _usbHelper).Calibrate());
        }

        private void sldBottomGate_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (_calibration?.Shifter != null && e.NewValue != e.OldValue)
            {
                _calibration.Shifter.LowerY = (int)e.NewValue;
                SetShifterCalibration(_calibration.Shifter);
            }
        }

        private void sldTopGate_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (_calibration?.Shifter != null && e.NewValue != e.OldValue)
            {
                _calibration.Shifter.UpperY = (int)e.NewValue;
                SetShifterCalibration(_calibration.Shifter);
            }
        }

        private void btnWriteToDevice_Click(object sender, RoutedEventArgs e)
        {
            _usbHelper.SetCalibration(_calibration);
        }

        #endregion
    }
}
