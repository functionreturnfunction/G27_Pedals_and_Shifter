using System.Windows;

namespace G27PedalsAndShifterConfigurator.Wpf
{
    public abstract class CalibratorBase<TCalibration>
    {
        protected readonly MainWindow _window;
        protected readonly UsbDeviceHelper _usbHelper;

        public CalibratorBase(MainWindow window, UsbDeviceHelper usbHelper)
        {
            _window = window;
            _usbHelper = usbHelper;
        }

        protected virtual string GetName()
        {
            return GetType().Name.Replace("Calibrator", string.Empty).ToLower();
        }

        protected virtual void Prompt(string prompt)
        {
            MessageBox.Show(_window, prompt, $"Calibrating {GetName()}");
        }

        public abstract TCalibration Calibrate();
    }
}