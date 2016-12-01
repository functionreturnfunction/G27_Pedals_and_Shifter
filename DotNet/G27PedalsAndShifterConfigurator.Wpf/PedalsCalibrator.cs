using System;

namespace G27PedalsAndShifterConfigurator.Wpf
{
    public class PedalsCalibrator : CalibratorBase<PedalsCalibration>
    {
        public PedalsCalibrator(MainWindow window, UsbDeviceHelper usbHelper) : base(window, usbHelper) {}

        public override PedalsCalibration Calibrate()
        {
            var calibration = new PedalsCalibration();

            Prompt("Release all pedals and then click OK");

            var state = _usbHelper.GetStatus().Pedals;
            calibration.MinThrottle = state.Throttle;
            calibration.MinBrake = state.Brake;
            calibration.MinClutch = state.Clutch;

            Prompt("Press the throttle all the way to the floor and click OK");
            calibration.MaxThrottle = _usbHelper.GetStatus().Pedals.Throttle;

            Prompt("Press the brake all the way to the floor and click OK");
            calibration.MaxBrake = _usbHelper.GetStatus().Pedals.Brake;

            Prompt("Press the clutch all the way to the floor and click OK");
            calibration.MaxClutch = _usbHelper.GetStatus().Pedals.Clutch;

            return calibration;
        }
    }
}