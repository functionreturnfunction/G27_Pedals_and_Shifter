namespace G27PedalsAndShifterConfigurator.Wpf
{
    public class ShifterCalibrator : CalibratorBase<ShifterCalibration>
    {
        public override ShifterCalibration Calibrate()
        {
            var calibration = new ShifterCalibration();

            Prompt("A series of instructions will now appear asing you to shift into different gears, and then push the shift lever against the gates to get the full thresholds involved. DO NOT push hard, only push firmly or you will break the plastic.");

            Prompt("Shift into first gear and then click OK");

            Prompt("Press the shift lever firmly into the upper-right of first gear and click OK");

            var state = _usbHelper.GetStatus().Shifter;

            calibration.Gate13 = state.XPosition;
            calibration.MaxY = state.YPosition;

            Prompt("Shift into second gear and then click OK");

            Prompt("Press the shift lever firmly into the lower-right of second gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate24 = state.XPosition;
            calibration.MinY = state.YPosition;

            Prompt("Shift into third gear and then click OK");

            Prompt("Press the shift lever firmly into the upper-left of third gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate13 = (state.XPosition + calibration.Gate13) / 2;
            calibration.MaxY = state.YPosition > calibration.MaxY ? state.YPosition : calibration.MaxY;

            Prompt("Shift into third gear and then click OK");

            Prompt("Press the shift lever firmly into the upper-right of third gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate35 = state.XPosition;
            calibration.MaxY = state.YPosition > calibration.MaxY ? state.YPosition : calibration.MaxY;

            Prompt("Shift into fourth gear and then click OK");

            Prompt("Press the shift lever firmly into the lower-left of fourth gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate24 = (state.XPosition + calibration.Gate24) / 2;
            calibration.MinY = state.YPosition < calibration.MinY ? state.YPosition : calibration.MinY;

            Prompt("Shift into fourth gear and then click OK");

            Prompt("Press the shift lever firmly into the lower-right of fourth gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate46 = state.XPosition;
            calibration.MinY = state.YPosition < calibration.MinY ? state.YPosition : calibration.MinY;

            Prompt("Shift into fifth gear and then click OK");

            Prompt("Press the shift lever firmly into the upper-left of fifth gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate35 = (state.XPosition + calibration.Gate35) / 2;
            calibration.MaxY = state.YPosition > calibration.MaxY ? state.YPosition : calibration.MaxY;

            Prompt("Shift into sixth gear and then click OK");

            Prompt("Press the shift lever firmly into the lower-left of sixth gear and click OK");

            state = _usbHelper.GetStatus().Shifter;

            calibration.Gate46 = (state.XPosition + calibration.Gate46) / 2;
            calibration.MinY = state.YPosition < calibration.MinY ? state.YPosition : calibration.MinY;

            return calibration;
        }

        public ShifterCalibrator(MainWindow window, UsbDeviceHelper usbHelper) : base(window, usbHelper) {}
    }
}