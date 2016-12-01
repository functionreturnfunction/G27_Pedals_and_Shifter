using System;

namespace G27PedalsAndShifterConfigurator.Wpf
{
    public class PedalsCalibrator : CalibratorBase<PedalsCalibration>
    {
        public PedalsCalibrator(MainWindow window, UsbDeviceHelper usbHelper) : base(window, usbHelper) {}

        public override PedalsCalibration Calibrate()
        {
            throw new NotImplementedException();
        }
    }
}