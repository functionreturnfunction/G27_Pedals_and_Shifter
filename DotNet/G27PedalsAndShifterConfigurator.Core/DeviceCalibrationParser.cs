using CommandMessenger;

namespace G27PedalsAndShifterConfigurator
{
    public class DeviceCalibrationParser
    {
        public static DeviceCalibration ParseCalibration(ReceivedCommand cmd)
        {
            var calibration = new DeviceCalibration();

            calibration.Pedals.MinThrottle = cmd.ReadInt16Arg();
            calibration.Pedals.MaxThrottle = cmd.ReadInt16Arg();
            calibration.Pedals.MinBrake = cmd.ReadInt16Arg();
            calibration.Pedals.MaxBrake = cmd.ReadInt16Arg();
            calibration.Pedals.MinClutch = cmd.ReadInt16Arg();
            calibration.Pedals.MaxClutch = cmd.ReadInt16Arg();

            calibration.Shifter.Gate13 = cmd.ReadInt16Arg();
            calibration.Shifter.Gate24 = cmd.ReadInt16Arg();
            calibration.Shifter.Gate35 = cmd.ReadInt16Arg();
            calibration.Shifter.Gate46 = cmd.ReadInt16Arg();
            calibration.Shifter.LowerY = cmd.ReadInt16Arg();
            calibration.Shifter.UpperY = cmd.ReadInt16Arg();

            return calibration;
        }
    }
}