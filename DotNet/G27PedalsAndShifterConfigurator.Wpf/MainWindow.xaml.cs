using System;
using System.ComponentModel;
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

        private int ScaleInt(int value, int fromMin, int fromMax, int toMin, int toMax)
        {
//            return value*(toMax - toMin)/(fromMax - fromMin) + toMin;
            return (value - fromMin)*(toMax - toMin)/(fromMax - fromMin) + toMin;
        }

        private int Scale10Bit(int value, double max = 100)
        {
            return ScaleInt(value, 0, 1023, 0, (int)max);
        }

        private void SetShifterCalibration()
        {
            var minY = 0;
            var maxY = cvsShifter.Height;
            var upperY = Scale10Bit(_calibration.Shifter.UpperY, cvsShifter.Height);
            var lowerY = Scale10Bit(_calibration.Shifter.LowerY, cvsShifter.Height);

            sldTopGate.Value = _calibration.Shifter.UpperY;
            sldBottomGate.Value = _calibration.Shifter.LowerY;

            lneGate13.X1 = lneGate13.X2 = Scale10Bit(_calibration.Shifter.Gate13, cvsShifter.Height);
            lneGate13.Y1 = minY;
            lneGate13.Y2 = upperY;

            lneGate24.X1 = lneGate24.X2 = Scale10Bit(_calibration.Shifter.Gate24, cvsShifter.Height);
            lneGate24.Y1 = maxY;
            lneGate24.Y2 = lowerY;

            lneGate35.X1 = lneGate35.X2 = Scale10Bit(_calibration.Shifter.Gate35, cvsShifter.Height);
            lneGate35.Y1 = minY;
            lneGate35.Y2 = upperY;

            lneGate46.X1 = lneGate46.X2 = Scale10Bit(_calibration.Shifter.Gate46, cvsShifter.Height);
            lneGate46.Y1 = maxY;
            lneGate46.Y2 = lowerY;
        }

        private void SetPedalsCalibration()
        {
            rsThrottle.LowerValue = _calibration.Pedals.MinThrottle;
            rsThrottle.HigherValue = _calibration.Pedals.MaxThrottle;
            rsBrake.LowerValue = _calibration.Pedals.MinBrake;
            rsBrake.HigherValue = _calibration.Pedals.MaxBrake;
            rsClutch.LowerValue = _calibration.Pedals.MinClutch;
            rsClutch.HigherValue = _calibration.Pedals.MaxClutch;
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

                pbActualThrottle.Value = Scale10Bit(state.Pedals.Throttle);
                pbActualBrake.Value = Scale10Bit(state.Pedals.Brake);
                pbActualClutch.Value = Scale10Bit(state.Pedals.Clutch);

                if (!_calibration.Pedals.HasAnyUnset)
                {
                    pbVirtualThrottle.Value = ScaleInt(state.Pedals.Throttle, _calibration.Pedals.MinThrottle,
                        _calibration.Pedals.MaxThrottle, 0, 100);
                    pbVirtualBrake.Value = ScaleInt(state.Pedals.Brake, _calibration.Pedals.MinBrake,
                        _calibration.Pedals.MaxBrake, 0, 100);
                    pbVirtualClutch.Value = ScaleInt(state.Pedals.Clutch, _calibration.Pedals.MinClutch,
                        _calibration.Pedals.MaxClutch, 0, 100);
                }

                Canvas.SetTop(rctShifterPosition, cvsShifter.Height - Scale10Bit(state.Shifter.YPosition, cvsShifter.Height) - rctShifterPosition.Height / 2);
                Canvas.SetLeft(rctShifterPosition, Scale10Bit(state.Shifter.XPosition, cvsShifter.Width - rctShifterPosition.Width / 2));
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
            if (!_calibration.Shifter.HasAnyUnset)
            {
                SetShifterCalibration();
            }

            if (!_calibration.Pedals.HasAnyUnset)
            {
                SetPedalsCalibration();
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

        private void btnCalibratePedals_Click(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals = new PedalsCalibrator(this, _usbHelper).Calibrate();
            SetPedalsCalibration();
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
            MessageBox.Show(this, "Finished writing calibration data to device");
        }

        private void Window_Closing(object sender, CancelEventArgs e)
        {
            if (_usbHelper != null && _usbHelper.IsConncected)
            {
                DisconnectErthang();
            }
        }

        private void rsThrottle_LowerValueChanged(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals.MinThrottle = (int)rsThrottle.LowerValue;
            SetPedalsCalibration();
        }

        private void rsThrottle_HigherValueChanged(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals.MaxThrottle = (int)rsThrottle.HigherValue;
            SetPedalsCalibration();
        }

        private void rsBrake_LowerValueChanged(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals.MinBrake = (int)rsBrake.LowerValue;
            SetPedalsCalibration();
        }

        private void rsBrake_HigherValueChanged(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals.MaxBrake = (int)rsBrake.HigherValue;
            SetPedalsCalibration();
        }

        private void rsClutch_LowerValueChanged(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals.MinClutch = (int)rsClutch.LowerValue;
            SetPedalsCalibration();
        }

        private void rsClutch_HigherValueChanged(object sender, RoutedEventArgs e)
        {
            _calibration.Pedals.MaxClutch = (int)rsClutch.HigherValue;
            SetPedalsCalibration();
        }

        private void btnResetCalibration_OnClick(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show(this, "This will reset the calibration in both this app and your device.  Are you sure?",
                    "Reset Confirmation", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
            {
                _calibration = DeviceCalibration.Reset();
                if (_usbHelper != null && _usbHelper.IsConncected)
                {
                    _usbHelper.SetCalibration(_calibration);
                }
                MessageBox.Show(this, "Finished resetting calibration");
            }
        }

        #endregion
    }
}
